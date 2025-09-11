#ifndef SIM800_CALL_H
#define SIM800_CALL_H

#include <stdbool.h>
#include "sim800.h"

bool sim800_call_dial(const char *dst_e164);
bool sim800_call_hangup(void);
bool sim800_call_answer(void);
bool sim800_call_set_clip(bool enable);

#endif /* SIM800_CALL_H */
