#ifndef SIM800_HTTP_H
#define SIM800_HTTP_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sim800.h"

typedef enum { SIM800_HTTP_GET, SIM800_HTTP_POST } sim800_http_method_t;

bool sim800_http_init(void);
bool sim800_http_term(void);
bool sim800_http_set_param(const char *key, const char *value);
bool sim800_http_do(sim800_http_method_t m, const uint8_t *body, size_t body_len,
                    int *http_code, uint8_t *resp, size_t *inout_len, uint32_t timeout_ms);

#endif /* SIM800_HTTP_H */
