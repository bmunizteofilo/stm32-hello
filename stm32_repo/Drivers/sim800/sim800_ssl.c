#include "sim800_ssl.h"
#if SIM800_ENABLE_SSL
#include <stdio.h>

bool sim800_ssl_enable(bool en) {
    return sim800_cmdf(NULL,0,1000,"AT+HTTPSSL=%d", en?1:0);
}
#endif
