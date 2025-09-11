#ifndef SIM800_CMDS_H
#define SIM800_CMDS_H

typedef enum {
    SIM800_CMD_AT = 0,
    SIM800_CMD_ATE0,
    SIM800_CMD_CMEE2,
    SIM800_CMD_CLIP1,
    SIM800_CMD_CMGF1,
    SIM800_CMD_CNMI,
    SIM800_CMD_MAX
} sim800_cmd_t;

static inline const char *sim800_cmd_str(sim800_cmd_t c) {
    static const char *tbl[] = {
        "AT",
        "ATE0",
        "AT+CMEE=2",
        "AT+CLIP=1",
        "AT+CMGF=1",
        "AT+CNMI=2,1,0,0,0"
    };
    return (c < SIM800_CMD_MAX) ? tbl[c] : "";
}

#endif /* SIM800_CMDS_H */
