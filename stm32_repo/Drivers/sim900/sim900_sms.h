#ifndef SIM900_SMS_H
#define SIM900_SMS_H

/**
 * \file sim900_sms.h
 * \brief SMS helper functions.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "sim900.h"

bool sim900_sms_set_text_mode(bool text);
bool sim900_sms_set_smsc(const char *num_e164);
bool sim900_sms_send_text(const char *dst_e164, const char *msg_utf8, uint32_t timeout_ms);
bool sim900_sms_list(const char *which, void (*on_entry)(int idx, const char *stat, const char *oa, const char *time, const char *body));
bool sim900_sms_read(int index, char *out_utf8, size_t outsz, char *oa, size_t oasz);
bool sim900_sms_delete(int index);

#endif /* SIM900_SMS_H */
