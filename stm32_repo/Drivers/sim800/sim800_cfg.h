#ifndef SIM800_CFG_H
#define SIM800_CFG_H

/** Buffer sizes and default parameters for the SIM800 driver. */
#define SIM800_RX_BUF_SZ          2048
#define SIM800_TX_BUF_SZ           512
#define SIM800_CMD_TIMEOUT_MS     5000
#define SIM800_BOOT_PULSE_MS      1200
#define SIM800_RESP_LINE_MAX       512
#define SIM800_USE_SAPBR           1
#define SIM800_ENABLE_SSL          1
#define SIM800_ENABLE_FTP          0
#define SIM800_LOG_ENABLE          1

#endif /* SIM800_CFG_H */
