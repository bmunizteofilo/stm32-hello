#ifndef SIM800_SMS_H
#define SIM800_SMS_H

#include <stdbool.h>
#include <stddef.h>
#include "sim800.h"

bool sim800_sms_set_text_mode(bool text);
bool sim800_sms_set_smsc(const char *num_e164);
bool sim800_sms_send_text(const char *dst_e164, const char *msg_utf8, uint32_t timeout_ms);
bool sim800_sms_list(const char *which, void (*on_entry)(int idx, const char *stat, const char *oa, const char *time, const char *body));
bool sim800_sms_read(int index, char *out_utf8, size_t outsz, char *oa, size_t oasz);
bool sim800_sms_delete(int index);

#endif /* SIM800_SMS_H */
