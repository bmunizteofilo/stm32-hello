#include "sim800.h"
#include "sim800_cfg.h"
#include "sim800_cmds.h"
#include "sim800_dbg.h"
#include "sim800_utf.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

static sim800_port_t *g_port = NULL;
static void *g_mutex = NULL;
static sim800_state_t g_state = SIM800_OFF;

static sim800_on_sms_t g_sms_cb = NULL;
static sim800_on_call_t g_call_cb = NULL;
static sim800_on_ussd_t g_ussd_cb = NULL;

static bool wait_line(char *buf, size_t bufsz, uint32_t timeout_ms) {
    size_t idx = 0;
    uint32_t start = sim800_port_millis();
    while (sim800_port_millis() - start < timeout_ms && idx + 1 < bufsz) {
        size_t n = sim800_port_read(g_port, (uint8_t*)&buf[idx], 1, 10);
        if (n == 0) continue;
        if (buf[idx] == '\n') {
            buf[idx+1] = '\0';
            return true;
        }
        idx += n;
    }
    if (idx < bufsz) buf[idx] = '\0';
    return false;
}

bool sim800_cmd(const char *cmd, char *resp, size_t resp_sz, uint32_t timeout_ms) {
    if (!g_port) return false;
    size_t len = strlen(cmd);
    char tx[SIM800_TX_BUF_SZ];
    if (len + 2 >= sizeof(tx)) return false;
    strcpy(tx, cmd);
    tx[len++] = '\r';
    tx[len] = '\0';
    sim800_port_mutex_lock(g_mutex);
    if (!sim800_port_write(g_port, (const uint8_t*)tx, len)) {
        sim800_port_mutex_unlock(g_mutex);
        return false;
    }
    char line[SIM800_RESP_LINE_MAX];
    bool ok = false;
    uint32_t start = sim800_port_millis();
    while (sim800_port_millis() - start < timeout_ms) {
        if (!wait_line(line, sizeof(line), timeout_ms)) break;
        if (strcmp(line, "\r\n") == 0) continue;
        if (strcmp(line, "OK\r\n") == 0) { ok = true; break; }
        if (strcmp(line, "ERROR\r\n") == 0) { ok = false; break; }
        if (resp && resp_sz > 0) {
            strncat(resp, line, resp_sz - strlen(resp) - 1);
        }
    }
    sim800_port_mutex_unlock(g_mutex);
    return ok;
}

bool sim800_cmdf(char *resp, size_t resp_sz, uint32_t timeout_ms, const char *fmt, ...) {
    char buf[SIM800_TX_BUF_SZ];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (resp && resp_sz) resp[0] = '\0';
    return sim800_cmd(buf, resp, resp_sz, timeout_ms);
}

static void default_handshake(void) {
    char dummy[64];
    for (int i=0;i<5;i++) {
        if (sim800_cmd("AT", dummy, sizeof(dummy), 1000)) break;
    }
    sim800_cmd(sim800_cmd_str(SIM800_CMD_ATE0), NULL, 0, 1000);
    sim800_cmd(sim800_cmd_str(SIM800_CMD_CMEE2), NULL, 0, 1000);
    sim800_cmd(sim800_cmd_str(SIM800_CMD_CLIP1), NULL, 0, 1000);
    sim800_cmd(sim800_cmd_str(SIM800_CMD_CMGF1), NULL, 0, 1000);
    sim800_cmd(sim800_cmd_str(SIM800_CMD_CNMI), NULL, 0, 1000);
}

bool sim800_init(sim800_port_t *port) {
    if (!port) return false;
    g_port = port;
    g_mutex = sim800_port_mutex_create();
    default_handshake();
    g_state = SIM800_READY;
    return true;
}

bool sim800_power_on(void) {
    g_state = SIM800_BOOTING;
    sim800_port_pulse_pwrkey(SIM800_BOOT_PULSE_MS);
    for (uint32_t t=0; t<5000; t+=100) {
        if (sim800_at_ping(1)) { g_state = SIM800_READY; return true; }
    }
    return false;
}

bool sim800_power_off(void) {
    return sim800_cmd("AT+CPOWD=1", NULL, 0, 5000);
}

bool sim800_reset(void) {
    if (sim800_port_pulse_reset(100)) return true;
    return sim800_cmd("AT+CFUN=1,1", NULL, 0, 5000);
}

bool sim800_at_ping(uint32_t attempts) {
    char dummy[32];
    for (uint32_t i=0;i<attempts;i++) {
        if (sim800_cmd("AT", dummy, sizeof(dummy), 1000)) return true;
    }
    return false;
}

bool sim800_get_imei(char *out, size_t outsz) { return sim800_cmd("AT+GSN", out, outsz, 1000); }
bool sim800_get_imsi(char *out, size_t outsz) { return sim800_cmd("AT+CIMI", out, outsz, 1000); }
bool sim800_get_ccid(char *out, size_t outsz) { return sim800_cmd("AT+CCID", out, outsz, 1000); }
bool sim800_get_firmware(char *out, size_t outsz) { return sim800_cmd("ATI", out, outsz, 1000); }
bool sim800_get_operator(char *out, size_t outsz) { return sim800_cmd("AT+COPS?", out, outsz, 1000); }

bool sim800_get_signal(int *rssi_dbm, int *ber) {
    char buf[64];
    if (!sim800_cmd("AT+CSQ", buf, sizeof(buf), 1000)) return false;
    int rssi=0, b=0; if (sscanf(buf, "\r\n+CSQ: %d,%d", &rssi,&b)==2) {
        if (rssi==99) { if(rssi_dbm) *rssi_dbm=-113; } else if (rssi_dbm) { *rssi_dbm = -113 + 2*rssi; }
        if (ber) *ber = b;
        return true;
    }
    return false;
}

bool sim800_set_pin(const char *pin) {
    return sim800_cmdf(NULL,0,1000,"AT+CPIN=%s",pin);
}

bool sim800_get_pin_reqs(bool *pin_needed) {
    char buf[64];
    if (!sim800_cmd("AT+CPIN?", buf, sizeof(buf), 1000)) return false;
    if (pin_needed) *pin_needed = (strstr(buf, "SIM PIN")!=NULL);
    return true;
}

bool sim800_get_reg(sim800_reg_t *cs_reg, sim800_reg_t *ps_reg) {
    char buf[64];
    if (sim800_cmd("AT+CREG?", buf, sizeof(buf), 1000)) {
        int n,stat; if (sscanf(buf, "\r\n+CREG: %d,%d", &n,&stat)==2 && cs_reg) *cs_reg=(sim800_reg_t)stat;
    }
    if (sim800_cmd("AT+CGREG?", buf, sizeof(buf), 1000)) {
        int n,stat; if (sscanf(buf, "\r\n+CGREG: %d,%d", &n,&stat)==2 && ps_reg) *ps_reg=(sim800_reg_t)stat;
    }
    return true;
}

bool sim800_wait_registered(uint32_t timeout_ms) {
    uint32_t start = sim800_port_millis();
    sim800_reg_t cs,ps;
    while (sim800_port_millis() - start < timeout_ms) {
        sim800_get_reg(&cs,&ps);
        if ((cs==SIM800_REG_HOME || cs==SIM800_REG_ROAMING) && (ps==SIM800_REG_HOME || ps==SIM800_REG_ROAMING)) return true;
    }
    return false;
}

bool sim800_bearer_open(const char *apn, const char *user, const char *pass, uint32_t timeout_ms) {
#if SIM800_USE_SAPBR
    sim800_cmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", NULL,0,1000);
    sim800_cmdf(NULL,0,1000,"AT+SAPBR=3,1,\"APN\",\"%s\"",apn);
    if (user && *user) sim800_cmdf(NULL,0,1000,"AT+SAPBR=3,1,\"USER\",\"%s\"",user);
    if (pass && *pass) sim800_cmdf(NULL,0,1000,"AT+SAPBR=3,1,\"PWD\",\"%s\"",pass);
    return sim800_cmdf(NULL,0,timeout_ms,"AT+SAPBR=1,1");
#else
    sim800_cmdf(NULL,0,1000,"AT+CSTT=\"%s\",\"%s\",\"%s\"",apn,user?user:"",pass?pass:""
    );
    if (!sim800_cmd("AT+CIICR", NULL,0,timeout_ms)) return false;
    return true;
#endif
}

bool sim800_bearer_close(void) {
#if SIM800_USE_SAPBR
    return sim800_cmd("AT+SAPBR=0,1", NULL,0,5000);
#else
    return sim800_cmd("AT+CIPSHUT", NULL,0,5000);
#endif
}

bool sim800_get_ip(char *out, size_t outsz) {
#if SIM800_USE_SAPBR
    return sim800_cmd("AT+SAPBR=2,1", out,outsz,1000);
#else
    return sim800_cmd("AT+CIFSR", out,outsz,1000);
#endif
}

bool sim800_sync_time(bool use_ntp) {
    if (use_ntp) {
        return sim800_cmd("AT+CLTS=1", NULL,0,1000);
    }
    return sim800_cmd("AT+CLTS=0", NULL,0,1000);
}

bool sim800_get_time(char *out, size_t outsz) {
    return sim800_cmd("AT+CCLK?", out,outsz,1000);
}

bool sim800_ussd_send(const char *code, char *resp_utf8, size_t resp_sz, int *dcs) {
    char buf[SIM800_RESP_LINE_MAX];
    if (!sim800_cmdf(buf,sizeof(buf),SIM800_CMD_TIMEOUT_MS,"AT+CUSD=1,\"%s\"",code)) return false;
    if (resp_utf8 && resp_sz) resp_utf8[0] = '\0';
    if (dcs) *dcs = 0;
    return true;
}

void sim800_poll(void) {
    char line[SIM800_RESP_LINE_MAX];
    while (sim800_port_read(g_port, (uint8_t*)line, 1, 10) > 0) {
        static size_t idx = 0; static char buf[SIM800_RESP_LINE_MAX];
        buf[idx++] = line[0];
        if (idx >= sizeof(buf)) idx = 0;
        if (line[0] == '\n') {
            buf[idx] = '\0';
            sim800_urc_parse(buf);
            idx = 0;
        }
    }
}

/* URC management */
void sim800_set_on_sms(sim800_on_sms_t cb) { g_sms_cb = cb; }
void sim800_set_on_call(sim800_on_call_t cb) { g_call_cb = cb; }
void sim800_set_on_ussd(sim800_on_ussd_t cb) { g_ussd_cb = cb; }

void sim800_urc_set_sms(sim800_on_sms_t cb) { g_sms_cb = cb; }
void sim800_urc_set_call(sim800_on_call_t cb) { g_call_cb = cb; }
void sim800_urc_set_ussd(sim800_on_ussd_t cb) { g_ussd_cb = cb; }

void sim800_urc_parse(const char *line) {
    if (strncmp(line, "RING", 4) == 0) {
        if (g_call_cb) g_call_cb("?");
    } else if (strncmp(line, "+CLIP:", 6) == 0) {
        if (g_call_cb) {
            const char *p = strchr(line, '\"');
            if (p) {
                char num[32];
                p++; const char *q = strchr(p, '\"');
                if (q) { size_t n=q-p; if(n>=sizeof(num)) n=sizeof(num)-1; memcpy(num,p,n); num[n]='\0'; g_call_cb(num); }
            }
        }
    } else if (strncmp(line, "+CMTI:", 6) == 0) {
        if (g_sms_cb) {
            int idx=0; char mem[16];
            if (sscanf(line, "+CMTI: %15[^,],%d", mem,&idx)==2) g_sms_cb(idx, mem);
        }
    } else if (strncmp(line, "+CUSD:",6)==0) {
        if (g_ussd_cb) {
            char txt[128]; int dcs=0;
            if (sscanf(line, "+CUSD: %d,\"%127[^\"]\"", &dcs, txt)==2) {
                char utf8[128]; sim800_ucs2_hex_to_utf8(txt, utf8, sizeof(utf8)); g_ussd_cb(utf8,dcs);
            }
        }
    }
}
