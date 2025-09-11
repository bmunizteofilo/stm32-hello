#include "sim800_pbk.h"
#include "sim800_utf.h"
#include "sim800_cfg.h"
#include <string.h>
#include <stdio.h>

bool sim800_pbk_set_storage(const char *mem) {
    return sim800_cmdf(NULL,0,1000,"AT+CPBS=\"%s\"", mem);
}

bool sim800_pbk_write(int index, const char *name_utf8, const char *num_e164) {
    char name_hex[128];
    sim800_utf8_to_ucs2_hex(name_utf8, name_hex, sizeof(name_hex));
    return sim800_cmdf(NULL,0,1000,"AT+CPBW=%d,\"%s\",129,\"%s\"", index, num_e164, name_hex);
}

bool sim800_pbk_read(int index, char *name_utf8, size_t name_sz, char *num_e164, size_t num_sz) {
    char resp[SIM800_RESP_LINE_MAX];
    if (!sim800_cmdf(resp,sizeof(resp),SIM800_CMD_TIMEOUT_MS,"AT+CPBR=%d", index)) return false;
    int idx; char num[32]; char name_hex[128];
    if (sscanf(resp, "\r\n+CPBR: %d,\"%31[^\"]\",129,\"%127[^\"]\"", &idx, num, name_hex)==3) {
        if (num_e164 && num_sz) { strncpy(num_e164,num,num_sz-1); num_e164[num_sz-1]='\0'; }
        if (name_utf8 && name_sz) sim800_ucs2_hex_to_utf8(name_hex, name_utf8, name_sz);
        return true;
    }
    return false;
}

bool sim800_pbk_delete(int index) {
    return sim800_cmdf(NULL,0,1000,"AT+CPBW=%d", index);
}
