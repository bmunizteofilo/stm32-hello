#include "sim900.h"
#include <string.h>
#include <stdio.h>

static sim900_t g_mod;
static sim900_on_sms_t  g_on_sms;
static sim900_on_call_t g_on_call;
static sim900_on_ussd_t g_on_ussd;

static void handle_urc(const char *line)
{
    if (!line) return;
    if (!strcmp(line, "RING")) {
        if (g_on_call) g_on_call(NULL);
    } else if (!strncmp(line, "+CLIP:", 6)) {
        const char *p = strchr(line, '"');
        if (p) {
            char num[32];
            sscanf(p, "\"%31[^\"]", num);
            if (g_on_call) g_on_call(num);
        }
    } else if (!strncmp(line, "+CMTI:", 6)) {
        char mem[8]; int idx;
        if (sscanf(line, "+CMTI: \"%7[^\"]\",%d", mem, &idx) == 2) {
            if (g_on_sms) g_on_sms(idx, mem);
        }
    } else if (!strncmp(line, "+CUSD:", 6)) {
        int dcs = 0; char txt[SIM900_RESP_LINE_MAX];
        if (sscanf(line, "+CUSD: %*d,\"%511[^\"]\",%d", txt, &dcs) >= 1) {
            if (g_on_ussd) g_on_ussd(txt, dcs);
        }
    }
}

static bool read_line(char *out, size_t outsz, uint32_t timeout_ms)
{
    size_t pos = 0;
    uint32_t start = sim900_port_millis();
    while (sim900_port_millis() - start < timeout_ms) {
        uint8_t ch;
        size_t r = sim900_port_read(g_mod.port, &ch, 1, timeout_ms);
        if (r == 0) continue;
        if (ch == '\r') continue;
        if (ch == '\n') {
            if (pos == 0) continue;
            out[pos] = '\0';
            return true;
        }
        if (pos + 1 < outsz) out[pos++] = (char)ch;
    }
    return false;
}

bool sim900_send_cmd(const char *cmd, char *resp, size_t resp_sz, uint32_t timeout_ms)
{
    char buf[SIM900_RESP_LINE_MAX];
    size_t len = strlen(cmd);
    sim900_port_mutex_lock(g_mod.mutex);
    sim900_port_write(g_mod.port, (const uint8_t *)cmd, len);
    sim900_port_write(g_mod.port, (const uint8_t *)"\r", 1);
    bool ok = false;
    while (read_line(buf, sizeof(buf), timeout_ms)) {
        if (!strcmp(buf, "OK")) { ok = true; break; }
        if (!strcmp(buf, "ERROR")) { ok = false; break; }
        handle_urc(buf);
        if (resp && resp_sz) {
            strncpy(resp, buf, resp_sz - 1);
            resp[resp_sz - 1] = '\0';
        }
    }
    sim900_port_mutex_unlock(g_mod.mutex);
    return ok;
}

bool sim900_send_cmd_ex(const char *cmd, const uint8_t *data, size_t data_len, char *resp, size_t resp_sz, uint32_t timeout_ms)
{
    char buf[SIM900_RESP_LINE_MAX];
    size_t len = strlen(cmd);
    sim900_port_mutex_lock(g_mod.mutex);
    sim900_port_write(g_mod.port, (const uint8_t *)cmd, len);
    sim900_port_write(g_mod.port, (const uint8_t *)"\r", 1);
    if (!read_line(buf, sizeof(buf), timeout_ms) || buf[0] != '>') {
        sim900_port_mutex_unlock(g_mod.mutex);
        return false;
    }
    if (data && data_len) sim900_port_write(g_mod.port, data, data_len);
    uint8_t ctrl_z = 0x1A;
    sim900_port_write(g_mod.port, &ctrl_z, 1);
    bool ok = false;
    while (read_line(buf, sizeof(buf), timeout_ms)) {
        if (!strcmp(buf, "OK")) { ok = true; break; }
        if (!strcmp(buf, "ERROR")) { ok = false; break; }
        handle_urc(buf);
        if (resp && resp_sz) {
            strncpy(resp, buf, resp_sz - 1);
            resp[resp_sz - 1] = '\0';
        }
    }
    sim900_port_mutex_unlock(g_mod.mutex);
    return ok;
}

bool sim900_init(sim900_port_t *port)
{
    memset(&g_mod, 0, sizeof(g_mod));
    g_mod.port = port;
    g_mod.mutex = sim900_port_mutex_create();
    return true;
}

bool sim900_power_on(void)
{
    sim900_port_pulse_pwrkey(SIM900_BOOT_PULSE_MS);
    return sim900_at_ping(5);
}

bool sim900_power_off(void)
{
    return sim900_send_cmd("AT+CGATT=0", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_reset(void)
{
    if (sim900_port_pulse_reset(100)) return true;
    return sim900_send_cmd("AT+CFUN=1,1", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_at_ping(uint32_t attempts)
{
    char resp[32];
    while (attempts--) {
        if (sim900_send_cmd("AT", resp, sizeof(resp), SIM900_CMD_TIMEOUT_MS)) return true;
    }
    return false;
}

bool sim900_get_imei(char *out, size_t outsz)
{ return sim900_send_cmd("AT+CGSN", out, outsz, SIM900_CMD_TIMEOUT_MS); }

bool sim900_get_imsi(char *out, size_t outsz)
{ return sim900_send_cmd("AT+CIMI", out, outsz, SIM900_CMD_TIMEOUT_MS); }

bool sim900_get_ccid(char *out, size_t outsz)
{ return sim900_send_cmd("AT+CCID", out, outsz, SIM900_CMD_TIMEOUT_MS); }

bool sim900_get_firmware(char *out, size_t outsz)
{ return sim900_send_cmd("ATI", out, outsz, SIM900_CMD_TIMEOUT_MS); }

bool sim900_get_operator(char *out, size_t outsz)
{ return sim900_send_cmd("AT+COPS?", out, outsz, SIM900_CMD_TIMEOUT_MS); }

bool sim900_get_signal(int *rssi_dbm, int *ber)
{
    char resp[32];
    if (!sim900_send_cmd("AT+CSQ", resp, sizeof(resp), SIM900_CMD_TIMEOUT_MS)) return false;
    int rssi, b; if (sscanf(resp, "+CSQ: %d,%d", &rssi, &b) == 2) { if (rssi_dbm) *rssi_dbm = rssi; if (ber) *ber = b; return true; }
    return false;
}

bool sim900_set_pin(const char *pin)
{
    char cmd[32]; snprintf(cmd, sizeof(cmd), "AT+CPIN=\"%s\"", pin); return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_get_pin_reqs(bool *pin_needed)
{
    char resp[32];
    if (!sim900_send_cmd("AT+CPIN?", resp, sizeof(resp), SIM900_CMD_TIMEOUT_MS)) return false;
    if (pin_needed) *pin_needed = strstr(resp, "READY") == NULL;
    return true;
}

bool sim900_get_reg(sim900_reg_t *cs_reg, sim900_reg_t *ps_reg)
{
    char resp[32]; int n;
    if (cs_reg) {
        if (sim900_send_cmd("AT+CREG?", resp, sizeof(resp), SIM900_CMD_TIMEOUT_MS) && sscanf(resp, "+CREG: %*d,%d", &n) == 1)
            *cs_reg = (sim900_reg_t)n; else *cs_reg = SIM900_REG_UNKNOWN;
    }
    if (ps_reg) {
        if (sim900_send_cmd("AT+CGREG?", resp, sizeof(resp), SIM900_CMD_TIMEOUT_MS) && sscanf(resp, "+CGREG: %*d,%d", &n) == 1)
            *ps_reg = (sim900_reg_t)n; else *ps_reg = SIM900_REG_UNKNOWN;
    }
    return true;
}

bool sim900_wait_registered(uint32_t timeout_ms)
{
    uint32_t start = sim900_port_millis();
    sim900_reg_t c, p;
    while (sim900_port_millis() - start < timeout_ms) {
        sim900_get_reg(&c, &p);
        if ((c == SIM900_REG_HOME || c == SIM900_REG_ROAMING) &&
            (p == SIM900_REG_HOME || p == SIM900_REG_ROAMING)) return true;
    }
    return false;
}

bool sim900_bearer_open(const char *apn, const char *user, const char *pass, uint32_t timeout_ms)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+SAPBR=3,1,\"APN\",\"%s\"", apn);
    if (!sim900_send_cmd(cmd, NULL, 0, timeout_ms)) return false;
    if (user && *user) { snprintf(cmd, sizeof(cmd), "AT+SAPBR=3,1,\"USER\",\"%s\"", user); sim900_send_cmd(cmd, NULL, 0, timeout_ms); }
    if (pass && *pass) { snprintf(cmd, sizeof(cmd), "AT+SAPBR=3,1,\"PWD\",\"%s\"", pass); sim900_send_cmd(cmd, NULL, 0, timeout_ms); }
    return sim900_send_cmd("AT+SAPBR=1,1", NULL, 0, timeout_ms);
}

bool sim900_bearer_close(void)
{
    return sim900_send_cmd("AT+SAPBR=0,1", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_get_ip(char *out, size_t outsz)
{
    return sim900_send_cmd("AT+SAPBR=2,1", out, outsz, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_sync_time(bool use_ntp)
{
    (void)use_ntp;
    return sim900_send_cmd("AT+CLTS=1", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_get_time(char *out, size_t outsz)
{
    return sim900_send_cmd("AT+CCLK?", out, outsz, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_ussd_send(const char *code, char *resp_utf8, size_t resp_sz, int *dcs)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CUSD=1,\"%s\"", code);
    char resp[SIM900_RESP_LINE_MAX];
    if (!sim900_send_cmd(cmd, resp, sizeof(resp), SIM900_CMD_TIMEOUT_MS)) return false;
    if (sscanf(resp, "+CUSD: %*d,\"%511[^\"]\",%d", resp_utf8, dcs) >= 1) return true;
    if (resp_utf8 && resp_sz) { strncpy(resp_utf8, resp, resp_sz - 1); resp_utf8[resp_sz - 1] = '\0'; }
    return true;
}

void sim900_poll(void)
{
    char line[SIM900_RESP_LINE_MAX];
    while (read_line(line, sizeof(line), 1)) {
        handle_urc(line);
    }
}

void sim900_set_on_sms(sim900_on_sms_t cb) { g_on_sms = cb; }
void sim900_set_on_call(sim900_on_call_t cb) { g_on_call = cb; }
void sim900_set_on_ussd(sim900_on_ussd_t cb) { g_on_ussd = cb; }
