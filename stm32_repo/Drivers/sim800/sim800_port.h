#ifndef SIM800_PORT_H
#define SIM800_PORT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sim800_port;
typedef struct sim800_port sim800_port_t;

bool     sim800_port_init(sim800_port_t **out, uint32_t baud);
bool     sim800_port_write(sim800_port_t *p, const uint8_t *data, size_t len);
size_t   sim800_port_read(sim800_port_t *p, uint8_t *out, size_t maxlen, uint32_t timeout_ms);
uint32_t sim800_port_millis(void);

void *sim800_port_mutex_create(void);
void  sim800_port_mutex_lock(void *m);
void  sim800_port_mutex_unlock(void *m);

bool sim800_port_set_pwrkey(bool level);
bool sim800_port_pulse_pwrkey(uint32_t ms);
bool sim800_port_pulse_reset(uint32_t ms);

void sim800_port_log(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* SIM800_PORT_H */
