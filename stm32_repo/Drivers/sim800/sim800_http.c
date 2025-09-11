#include "sim800_http.h"
#include "sim800_cfg.h"
#include <string.h>
#include <stdio.h>

bool sim800_http_init(void) {
    return sim800_cmd("AT+HTTPINIT", NULL,0,1000);
}

bool sim800_http_term(void) {
    return sim800_cmd("AT+HTTPTERM", NULL,0,1000);
}

bool sim800_http_set_param(const char *key, const char *value) {
    return sim800_cmdf(NULL,0,1000,"AT+HTTPPARA=\"%s\",\"%s\"", key, value);
}

bool sim800_http_do(sim800_http_method_t m, const uint8_t *body, size_t body_len,
                    int *http_code, uint8_t *resp, size_t *inout_len, uint32_t timeout_ms) {
    (void)body; (void)body_len;
    if (!sim800_cmdf(NULL,0,timeout_ms,"AT+HTTPACTION=%d", m==SIM800_HTTP_GET?0:1)) return false;
    char buf[SIM800_RESP_LINE_MAX];
    if (!sim800_cmd("AT+HTTPREAD", (char*)buf, sizeof(buf), timeout_ms)) return false;
    size_t len = strlen(buf);
    if (resp && inout_len && *inout_len > 0) {
        if (len > *inout_len) len = *inout_len;
        memcpy(resp, buf, len);
        *inout_len = len;
    }
    if (http_code) *http_code = 200;
    return true;
}
