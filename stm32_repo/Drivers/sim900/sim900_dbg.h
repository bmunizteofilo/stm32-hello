#ifndef SIM900_DBG_H
#define SIM900_DBG_H

/**
 * \file sim900_dbg.h
 * \brief Lightweight logging macros.
 */

#include "sim900_cfg.h"

#if SIM900_LOG_ENABLE
void sim900_dbg_log(const char *tag, const char *fmt, ...);
#define SIM900_LOGI(fmt, ...) sim900_dbg_log("I", fmt, ##__VA_ARGS__)
#define SIM900_LOGW(fmt, ...) sim900_dbg_log("W", fmt, ##__VA_ARGS__)
#define SIM900_LOGE(fmt, ...) sim900_dbg_log("E", fmt, ##__VA_ARGS__)
#else
#define SIM900_LOGI(...)
#define SIM900_LOGW(...)
#define SIM900_LOGE(...)
#endif

#endif /* SIM900_DBG_H */
