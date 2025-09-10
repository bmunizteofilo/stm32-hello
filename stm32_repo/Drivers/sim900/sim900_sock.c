#include "sim900_sock.h"
#include <stdio.h>
#include <string.h>

extern bool sim900_send_cmd(const char *cmd, char *resp, size_t rs, uint32_t to);
extern bool sim900_send_cmd_ex(const char *cmd, const uint8_t *data, size_t len, char *resp, size_t rs, uint32_t to);

bool sim900_sock_attach(void)
{
    return sim900_send_cmd("AT+CIPSHUT", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_sock_detach(void)
{
    return sim900_send_cmd("AT+CIPSHUT", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}

bool sim900_sock_open(sim900_sock_t *out, const char *proto, const char *host, int port, uint32_t timeout_ms)
{
    (void)out;
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT+CIPSTART=\"%s\",\"%s\",%d", proto, host, port);
    return sim900_send_cmd(cmd, NULL, 0, timeout_ms);
}

bool sim900_sock_send(sim900_sock_t s, const uint8_t *data, size_t len, uint32_t *sent)
{
    (void)s;
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CIPSEND=%u", (unsigned)len);
    if (!sim900_send_cmd_ex(cmd, data, len, NULL, 0, SIM900_CMD_TIMEOUT_MS)) return false;
    if (sent) *sent = len;
    return true;
}

int sim900_sock_recv(sim900_sock_t s, uint8_t *buf, size_t bufsz, uint32_t timeout_ms)
{
    (void)s;
    (void)timeout_ms;
    if (!buf || !bufsz) return -1;
    // in real driver use CIPRXGET
    buf[0] = 0; return 0;
}

bool sim900_sock_close(sim900_sock_t s)
{
    (void)s;
    return sim900_send_cmd("AT+CIPCLOSE", NULL, 0, SIM900_CMD_TIMEOUT_MS);
}
