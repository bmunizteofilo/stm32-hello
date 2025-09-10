#include "sim900_pbk.h"
#include <stdio.h>
#include <string.h>

extern bool sim900_send_cmd(const char *cmd, char *resp, size_t rs, uint32_t to);

bool sim900_pbk_set_storage(const char *mem)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CPBS=\"%s\"", mem);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_pbk_write(int index, const char *name_utf8, const char *num_e164)
{
    char cmd[160];
    snprintf(cmd, sizeof(cmd), "AT+CPBW=%d,\"%s\",129,\"%s\"", index, num_e164, name_utf8);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_pbk_read(int index, char *name_utf8, size_t name_sz, char *num_e164, size_t num_sz)
{
    (void)name_utf8; (void)name_sz; (void)num_e164; (void)num_sz;
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CPBR=%d", index);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_pbk_delete(int index)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CPBW=%d", index);
    return sim900_send_cmd(cmd, NULL, 0, SIM900_CMD_TIMEOUT_MS);
}
