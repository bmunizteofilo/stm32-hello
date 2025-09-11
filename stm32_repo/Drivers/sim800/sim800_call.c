#include "sim800_call.h"
#include <stdio.h>

bool sim800_call_dial(const char *dst_e164) {
    return sim800_cmdf(NULL,0,10000,"ATD%s;", dst_e164);
}

bool sim800_call_hangup(void) {
    return sim800_cmd("ATH", NULL,0,1000);
}

bool sim800_call_answer(void) {
    return sim800_cmd("ATA", NULL,0,1000);
}

bool sim800_call_set_clip(bool enable) {
    return sim800_cmdf(NULL,0,1000,"AT+CLIP=%d", enable?1:0);
}
