#ifndef SIM800_URC_H
#define SIM800_URC_H

#include <stddef.h>

/* Callback types for URCs */
typedef void (*sim800_on_sms_t)(int idx, const char *mem);
typedef void (*sim800_on_call_t)(const char *number);
typedef void (*sim800_on_ussd_t)(const char *text_utf8, int dcs);

void sim800_urc_set_sms(sim800_on_sms_t cb);
void sim800_urc_set_call(sim800_on_call_t cb);
void sim800_urc_set_ussd(sim800_on_ussd_t cb);

void sim800_urc_parse(const char *line);

#endif /* SIM800_URC_H */
