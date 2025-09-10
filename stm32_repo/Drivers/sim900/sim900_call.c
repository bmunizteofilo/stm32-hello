#include "sim900_call.h"
#include <stdio.h>

extern bool sim900_send_cmd(const char *cmd, char *resp, size_t rs, uint32_t to);

bool sim900_call_dial(const char *dst_e164)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ATD%s;", dst_e164);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_call_hangup(void)
{
    return sim900_send_cmd("ATH", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_call_answer(void)
{
    return sim900_send_cmd("ATA", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_call_set_clip(bool enable)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CLIP=%d", enable ? 1 : 0);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}
