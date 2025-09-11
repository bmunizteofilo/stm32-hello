#ifndef SIM800_SOCK_H
#define SIM800_SOCK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sim800.h"

typedef int sim800_sock_t;

bool sim800_sock_attach(void);
bool sim800_sock_detach(void);
bool sim800_sock_open(sim800_sock_t *out, const char *proto, const char *host, int port, uint32_t timeout_ms);
bool sim800_sock_send(sim800_sock_t s, const uint8_t *data, size_t len, uint32_t *sent);
int  sim800_sock_recv(sim800_sock_t s, uint8_t *buf, size_t bufsz, uint32_t timeout_ms);
bool sim800_sock_close(sim800_sock_t s);

#endif /* SIM800_SOCK_H */
