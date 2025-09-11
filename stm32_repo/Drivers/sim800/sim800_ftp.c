#include "sim800_ftp.h"
#if SIM800_ENABLE_FTP
#include <string.h>
#include <stdio.h>

bool sim800_ftp_set_server(const char *host, const char *user, const char *pass) {
    sim800_cmdf(NULL,0,1000,"AT+FTPCID=1");
    sim800_cmdf(NULL,0,1000,"AT+FTPSERV=\"%s\"", host);
    if (user) sim800_cmdf(NULL,0,1000,"AT+FTPUN=\"%s\"", user);
    if (pass) sim800_cmdf(NULL,0,1000,"AT+FTPPW=\"%s\"", pass);
    return true;
}

bool sim800_ftp_put(const char *path, const uint8_t *data, size_t len) {
    sim800_cmdf(NULL,0,1000,"AT+FTPPUTPATH=\"/\"");
    sim800_cmdf(NULL,0,1000,"AT+FTPPUTNAME=\"%s\"", path);
    sim800_cmd("AT+FTPPUT=1", NULL,0,1000);
    sim800_port_write(NULL, data, len);
    sim800_cmd("AT+FTPPUT=2,0", NULL,0,1000);
    return true;
}

bool sim800_ftp_get(const char *path, uint8_t *data, size_t *len) {
    sim800_cmdf(NULL,0,1000,"AT+FTPGETPATH=\"/\"");
    sim800_cmdf(NULL,0,1000,"AT+FTPGETNAME=\"%s\"", path);
    sim800_cmd("AT+FTPGET=1", NULL,0,1000);
    if (data && len) {
        *len = sim800_port_read(NULL, data, *len, 5000);
    }
    sim800_cmd("AT+FTPGET=2,0", NULL,0,1000);
    return true;
}
#endif
