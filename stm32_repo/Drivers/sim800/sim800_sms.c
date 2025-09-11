#include "sim800_sms.h"
#include "sim800_utf.h"
#include "sim800_cfg.h"
#include <string.h>
#include <stdio.h>

bool sim800_sms_set_text_mode(bool text) {
    return sim800_cmdf(NULL,0,1000,"AT+CMGF=%d", text?1:0);
}

bool sim800_sms_set_smsc(const char *num_e164) {
    return sim800_cmdf(NULL,0,1000,"AT+CSCA=\"%s\"", num_e164);
}

bool sim800_sms_send_text(const char *dst_e164, const char *msg_utf8, uint32_t timeout_ms) {
    if (!sim800_cmdf(NULL,0,1000,"AT+CMGS=\"%s\"", dst_e164)) return false;
    char buf[SIM800_TX_BUF_SZ];
    size_t len = strlen(msg_utf8);
    if (len+2 < sizeof(buf)) {
        strcpy(buf, msg_utf8);
        buf[len++] = 0x1A; /* CTRL+Z */
        buf[len] = '\0';
        return sim800_cmd(buf, NULL, 0, timeout_ms);
    }
    return false;
}

bool sim800_sms_list(const char *which, void (*on_entry)(int idx, const char *stat, const char *oa, const char *time, const char *body)) {
    char resp[SIM800_RESP_LINE_MAX];
    if (!sim800_cmdf(resp,sizeof(resp),SIM800_CMD_TIMEOUT_MS,"AT+CMGL=\"%s\"",which)) return false;
    const char *p = resp;
    while ((p = strstr(p, "+CMGL:")) != NULL) {
        int idx; char stat[16]; char oa[32]; char time[32];
        if (sscanf(p, "+CMGL: %d,%15[^,],\"%31[^\"]\",,\"%31[^\"]\"", &idx, stat, oa, time)==4) {
            p = strstr(p, "\n");
            if (!p) break;
            p++;
            const char *end = strstr(p, "\r\n");
            if (!end) break;
            char body[161]; size_t n = end - p; if (n>=sizeof(body)) n=sizeof(body)-1; memcpy(body,p,n); body[n]='\0';
            if (on_entry) on_entry(idx, stat, oa, time, body);
            p = end;
        } else break;
    }
    return true;
}

bool sim800_sms_read(int index, char *out_utf8, size_t outsz, char *oa, size_t oasz) {
    char resp[SIM800_RESP_LINE_MAX];
    if (!sim800_cmdf(resp,sizeof(resp),SIM800_CMD_TIMEOUT_MS,"AT+CMGR=%d", index)) return false;
    char stat[16]; char number[32]; char time[32];
    if (sscanf(resp, "\r\n+CMGR: %15[^,],\"%31[^\"]\",,\"%31[^\"]\"", stat, number, time) >= 2) {
        if (oa && oasz) { strncpy(oa, number, oasz-1); oa[oasz-1]='\0'; }
        char *body = strstr(resp, "\n");
        if (body && out_utf8 && outsz) {
            strncpy(out_utf8, body+1, outsz-1); out_utf8[outsz-1]='\0';
        }
        return true;
    }
    return false;
}

bool sim800_sms_delete(int index) {
    return sim800_cmdf(NULL,0,1000,"AT+CMGD=%d", index);
}
