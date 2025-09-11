#ifndef SIM800_H
#define SIM800_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "sim800_port.h"
#include "sim800_urc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { SIM800_OFF=0, SIM800_BOOTING, SIM800_READY } sim800_state_t;

bool sim800_init(sim800_port_t *port);
bool sim800_power_on(void);
bool sim800_power_off(void);
bool sim800_reset(void);

bool sim800_at_ping(uint32_t attempts);
bool sim800_get_imei(char *out, size_t outsz);
bool sim800_get_imsi(char *out, size_t outsz);
bool sim800_get_ccid(char *out, size_t outsz);
bool sim800_get_firmware(char *out, size_t outsz);
bool sim800_get_operator(char *out, size_t outsz);
bool sim800_get_signal(int *rssi_dbm, int *ber);
bool sim800_set_pin(const char *pin);
bool sim800_get_pin_reqs(bool *pin_needed);

typedef enum { SIM800_REG_UNKNOWN, SIM800_REG_HOME, SIM800_REG_SEARCHING, SIM800_REG_DENIED, SIM800_REG_ROAMING } sim800_reg_t;
bool sim800_get_reg(sim800_reg_t *cs_reg, sim800_reg_t *ps_reg);
bool sim800_wait_registered(uint32_t timeout_ms);

bool sim800_bearer_open(const char *apn, const char *user, const char *pass, uint32_t timeout_ms);
bool sim800_bearer_close(void);
bool sim800_get_ip(char *out, size_t outsz);

bool sim800_sync_time(bool use_ntp);
bool sim800_get_time(char *out, size_t outsz);

bool sim800_ussd_send(const char *code, char *resp_utf8, size_t resp_sz, int *dcs);

void sim800_poll(void);

bool sim800_cmd(const char *cmd, char *resp, size_t resp_sz, uint32_t timeout_ms);
bool sim800_cmdf(char *resp, size_t resp_sz, uint32_t timeout_ms, const char *fmt, ...);

void sim800_set_on_sms(sim800_on_sms_t cb);
void sim800_set_on_call(sim800_on_call_t cb);
void sim800_set_on_ussd(sim800_on_ussd_t cb);

#ifdef __cplusplus
}
#endif

#endif /* SIM800_H */
