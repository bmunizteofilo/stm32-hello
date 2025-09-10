#ifndef SIM900_CALL_H
#define SIM900_CALL_H

/**
 * \file sim900_call.h
 * \brief Voice call helpers.
 */

#include <stdbool.h>
#include "sim900.h"

bool sim900_call_dial(const char *dst_e164);
bool sim900_call_hangup(void);
bool sim900_call_answer(void);
bool sim900_call_set_clip(bool enable);

#endif /* SIM900_CALL_H */
