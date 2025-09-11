#include "sim800_sock.h"
#include <string.h>
#include <stdio.h>

bool sim800_sock_attach(void) {
    sim800_cmd("AT+CIPSHUT", NULL,0,5000);
    return sim800_cmd("AT+CIPMUX=0", NULL,0,1000);
}

bool sim800_sock_detach(void) {
    return sim800_cmd("AT+CIPSHUT", NULL,0,5000);
}

bool sim800_sock_open(sim800_sock_t *out, const char *proto, const char *host, int port, uint32_t timeout_ms) {
    (void)timeout_ms;
    if (!sim800_cmdf(NULL,0,1000,"AT+CIPSTART=\"%s\",\"%s\",%d", proto, host, port)) return false;
    if (out) *out = 0;
    return true;
}

bool sim800_sock_send(sim800_sock_t s, const uint8_t *data, size_t len, uint32_t *sent) {
    (void)s;
    if (!sim800_cmdf(NULL,0,1000,"AT+CIPSEND=%u", (unsigned)len)) return false;
    sim800_port_write(NULL, data, len);
    sim800_port_write(NULL, (const uint8_t*)"\x1A", 1);
    if (sent) *sent = (uint32_t)len;
    return true;
}

int sim800_sock_recv(sim800_sock_t s, uint8_t *buf, size_t bufsz, uint32_t timeout_ms) {
    (void)s;
    char cmd[32]; snprintf(cmd,sizeof(cmd),"AT+CIPRXGET=2,%u", (unsigned)bufsz);
    if (!sim800_cmd(cmd, (char*)buf, bufsz, timeout_ms)) return -1;
    return (int)strlen((char*)buf);
}

bool sim800_sock_close(sim800_sock_t s) {
    (void)s;
    return sim800_cmd("AT+CIPCLOSE", NULL,0,5000);
}
