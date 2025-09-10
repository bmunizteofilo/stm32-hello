#include "sim900_sms.h"
#include "sim900_utf.h"
#include <stdio.h>
#include <string.h>

extern bool sim900_send_cmd(const char *cmd, char *resp, size_t rs, uint32_t to);
extern bool sim900_send_cmd_ex(const char *cmd, const uint8_t *data, size_t len, char *resp, size_t rs, uint32_t to);

bool sim900_sms_set_text_mode(bool text)
{
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+CMGF=%d", text ? 1 : 0);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_sms_set_smsc(const char *num_e164)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CSCA=\"%s\"", num_e164);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_sms_send_text(const char *dst_e164, const char *msg_utf8, uint32_t timeout_ms)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"", dst_e164);
    return sim900_send_cmd_ex(cmd, (const uint8_t *)msg_utf8, strlen(msg_utf8), NULL, 0, timeout_ms);
}

bool sim900_sms_list(const char *which, void (*on_entry)(int idx, const char *stat, const char *oa, const char *time, const char *body))
{
    (void)on_entry;
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CMGL=\"%s\"", which);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_sms_read(int index, char *out_utf8, size_t outsz, char *oa, size_t oasz)
{
    (void)oa; (void)oasz;
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CMGR=%d", index);
    return sim900_send_cmd(cmd, out_utf8, outsz, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_sms_delete(int index)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CMGD=%d", index);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}
