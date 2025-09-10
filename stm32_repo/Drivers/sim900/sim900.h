#ifndef SIM900_H
#define SIM900_H

/**
 * \file sim900.h
 * \brief Core SIM900 driver API.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sim900_cfg.h"
#include "sim900_port.h"
#include "sim900_dbg.h"
#include "sim900_urc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    sim900_port_t *port;
    void *mutex;
} sim900_t;

bool sim900_init(sim900_port_t *port);
bool sim900_power_on(void);
bool sim900_power_off(void);
bool sim900_reset(void);

bool sim900_at_ping(uint32_t attempts);
bool sim900_get_imei(char *out, size_t outsz);
bool sim900_get_imsi(char *out, size_t outsz);
bool sim900_get_ccid(char *out, size_t outsz);
bool sim900_get_firmware(char *out, size_t outsz);
bool sim900_get_operator(char *out, size_t outsz);
bool sim900_get_signal(int *rssi_dbm, int *ber);
bool sim900_set_pin(const char *pin);
bool sim900_get_pin_reqs(bool *pin_needed);

typedef enum { SIM900_REG_UNKNOWN, SIM900_REG_HOME, SIM900_REG_SEARCHING, SIM900_REG_DENIED, SIM900_REG_ROAMING } sim900_reg_t;
bool sim900_get_reg(sim900_reg_t *cs_reg, sim900_reg_t *ps_reg);
bool sim900_wait_registered(uint32_t timeout_ms);

bool sim900_bearer_open(const char *apn, const char *user, const char *pass, uint32_t timeout_ms);
bool sim900_bearer_close(void);
bool sim900_get_ip(char *out, size_t outsz);

bool sim900_sync_time(bool use_ntp);
bool sim900_get_time(char *out, size_t outsz);

bool sim900_ussd_send(const char *code, char *resp_utf8, size_t resp_sz, int *dcs);

void sim900_poll(void);

typedef void (*sim900_on_sms_t)(int index, const char *mem);
typedef void (*sim900_on_call_t)(const char *number);
typedef void (*sim900_on_ussd_t)(const char *text_utf8, int dcs);

void sim900_set_on_sms(sim900_on_sms_t cb);
void sim900_set_on_call(sim900_on_call_t cb);
void sim900_set_on_ussd(sim900_on_ussd_t cb);

/* internal helpers for modules */
bool sim900_send_cmd(const char *cmd, char *resp, size_t resp_sz, uint32_t timeout_ms);
bool sim900_send_cmd_ex(const char *cmd, const uint8_t *data, size_t data_len, char *resp, size_t resp_sz, uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* SIM900_H */
