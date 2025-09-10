#ifndef SIM900_PORT_H
#define SIM900_PORT_H

/**
 * \file sim900_port.h
 * \brief Abstraction layer to integrate project UART and GPIOs.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Opaque port handle. */
typedef struct sim900_port sim900_port_t;

bool     sim900_port_init(sim900_port_t **out, uint32_t baud);
bool     sim900_port_write(sim900_port_t *p, const uint8_t *data, size_t len);
size_t   sim900_port_read(sim900_port_t *p, uint8_t *out, size_t maxlen, uint32_t timeout_ms);
uint32_t sim900_port_millis(void);

void    *sim900_port_mutex_create(void);
void     sim900_port_mutex_lock(void *m);
void     sim900_port_mutex_unlock(void *m);

bool sim900_port_set_pwrkey(bool level);
bool sim900_port_pulse_pwrkey(uint32_t ms);
bool sim900_port_pulse_reset(uint32_t ms);

void sim900_port_log(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* SIM900_PORT_H */
