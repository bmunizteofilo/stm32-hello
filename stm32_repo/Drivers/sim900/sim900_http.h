#ifndef SIM900_HTTP_H
#define SIM900_HTTP_H

/**
 * \file sim900_http.h
 * \brief HTTP helpers using SIM900 AT commands.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "sim900.h"

typedef enum {
    SIM900_HTTP_GET = 0,
    SIM900_HTTP_POST = 1
} sim900_http_method_t;

bool sim900_http_init(void);
bool sim900_http_term(void);
bool sim900_http_set_param(const char *key, const char *value);
bool sim900_http_do(sim900_http_method_t m, const uint8_t *body, size_t body_len, int *http_code, uint8_t *resp, size_t *inout_len, uint32_t timeout_ms);

#endif /* SIM900_HTTP_H */
