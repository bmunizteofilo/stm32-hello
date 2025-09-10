#include "sim900_http.h"
#include "sim900_dbg.h"
#include <stdio.h>
#include <string.h>

extern bool sim900_send_cmd(const char *cmd, char *resp, size_t rs, uint32_t to);
extern bool sim900_send_cmd_ex(const char *cmd, const uint8_t *data, size_t len, char *resp, size_t rs, uint32_t to);

bool sim900_http_init(void)
{
    return sim900_send_cmd("AT+HTTPINIT", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_http_term(void)
{
    return sim900_send_cmd("AT+HTTPTERM", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_http_set_param(const char *key, const char *value)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+HTTPPARA=\"%s\",\"%s\"", key, value);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_http_do(sim900_http_method_t m, const uint8_t *body, size_t body_len, int *http_code, uint8_t *resp, size_t *inout_len, uint32_t timeout_ms)
{
    (void)body; (void)body_len;
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+HTTPACTION=%d", m);
    if (!sim900_send_cmd(cmd, NULL, 0, timeout_ms)) return false;
    size_t len = *inout_len;
    if (!sim900_send_cmd("AT+HTTPREAD", (char *)resp, len, timeout_ms)) return false;
    if (http_code) *http_code = 200; // dummy
    if (inout_len) *inout_len = strlen((char*)resp);
    return true;
}
