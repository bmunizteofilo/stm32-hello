#ifndef SIM800_DBG_H
#define SIM800_DBG_H

#include "sim800_port.h"

#define SIM800_LOGI(fmt, ...) sim800_port_log("I: " fmt, ##__VA_ARGS__)
#define SIM800_LOGW(fmt, ...) sim800_port_log("W: " fmt, ##__VA_ARGS__)
#define SIM800_LOGE(fmt, ...) sim800_port_log("E: " fmt, ##__VA_ARGS__)

#endif /* SIM800_DBG_H */
