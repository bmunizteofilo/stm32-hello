#ifndef SIM800_SSL_H
#define SIM800_SSL_H

#include <stdbool.h>
#include "sim800.h"

#if SIM800_ENABLE_SSL
bool sim800_ssl_enable(bool en);
#endif

#endif /* SIM800_SSL_H */
