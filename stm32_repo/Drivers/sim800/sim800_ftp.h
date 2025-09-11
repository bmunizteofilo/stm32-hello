#ifndef SIM800_FTP_H
#define SIM800_FTP_H

#include <stdbool.h>
#include <stddef.h>
#include "sim800.h"

#if SIM800_ENABLE_FTP
bool sim800_ftp_set_server(const char *host, const char *user, const char *pass);
bool sim800_ftp_put(const char *path, const uint8_t *data, size_t len);
bool sim800_ftp_get(const char *path, uint8_t *data, size_t *len);
#endif

#endif /* SIM800_FTP_H */
