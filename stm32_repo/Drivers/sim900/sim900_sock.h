#ifndef SIM900_SOCK_H
#define SIM900_SOCK_H

/**
 * \file sim900_sock.h
 * \brief TCP/UDP socket helpers.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sim900.h"

typedef int sim900_sock_t;

bool sim900_sock_attach(void);
bool sim900_sock_detach(void);
bool sim900_sock_open(sim900_sock_t *out, const char *proto, const char *host, int port, uint32_t timeout_ms);
bool sim900_sock_send(sim900_sock_t s, const uint8_t *data, size_t len, uint32_t *sent);
int  sim900_sock_recv(sim900_sock_t s, uint8_t *buf, size_t bufsz, uint32_t timeout_ms);
bool sim900_sock_close(sim900_sock_t s);

#endif /* SIM900_SOCK_H */
