#ifndef SIM900_CFG_H
#define SIM900_CFG_H

/**
 * \file sim900_cfg.h
 * \brief Driver configuration constants.
 */

#include <stdint.h>

#define SIM900_RX_BUF_SZ      2048u
#define SIM900_TX_BUF_SZ      512u
#define SIM900_CMD_TIMEOUT_MS 5000u
#define SIM900_BOOT_PULSE_MS  1200u
#define SIM900_RESP_LINE_MAX  512u

#ifndef SIM900_LOG_ENABLE
#define SIM900_LOG_ENABLE 1
#endif

#endif /* SIM900_CFG_H */
