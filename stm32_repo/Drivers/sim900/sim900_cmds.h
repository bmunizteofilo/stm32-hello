#ifndef SIM900_CMDS_H
#define SIM900_CMDS_H

/**
 * \file sim900_cmds.h
 * \brief Enumerations and helpers for AT commands.
 */

typedef enum {
    SIM900_CMD_AT,
    SIM900_CMD_ATE0,
    SIM900_CMD_CMEE2,
    SIM900_CMD_CMGF,
    SIM900_CMD_CMGS,
    SIM900_CMD_CPBS,
    SIM900_CMD_CPBW,
    SIM900_CMD_CPBR,
    SIM900_CMD_CMGD,
    SIM900_CMD_D,
    SIM900_CMD_H,
    SIM900_CMD_CLIP,
    SIM900_CMD_HTTPINIT,
    SIM900_CMD_HTTPTERM,
    SIM900_CMD_HTTPPARA,
    SIM900_CMD_HTTPACTION,
    SIM900_CMD_HTTPREAD,
    SIM900_CMD_CIPSHUT,
    SIM900_CMD_CIPSTART,
    SIM900_CMD_CIPSEND,
    SIM900_CMD_CIPCLOSE,
    SIM900_CMD_MAX
} sim900_cmd_t;

static inline const char *sim900_cmd_str(sim900_cmd_t c)
{
    switch (c) {
    case SIM900_CMD_AT: return "AT";
    case SIM900_CMD_ATE0: return "ATE0";
    case SIM900_CMD_CMEE2: return "AT+CMEE=2";
    case SIM900_CMD_CMGF: return "AT+CMGF";
    case SIM900_CMD_CMGS: return "AT+CMGS";
    case SIM900_CMD_CPBS: return "AT+CPBS";
    case SIM900_CMD_CPBW: return "AT+CPBW";
    case SIM900_CMD_CPBR: return "AT+CPBR";
    case SIM900_CMD_CMGD: return "AT+CMGD";
    case SIM900_CMD_D: return "ATD";
    case SIM900_CMD_H: return "ATH";
    case SIM900_CMD_CLIP: return "AT+CLIP";
    case SIM900_CMD_HTTPINIT: return "AT+HTTPINIT";
    case SIM900_CMD_HTTPTERM: return "AT+HTTPTERM";
    case SIM900_CMD_HTTPPARA: return "AT+HTTPPARA";
    case SIM900_CMD_HTTPACTION: return "AT+HTTPACTION";
    case SIM900_CMD_HTTPREAD: return "AT+HTTPREAD";
    case SIM900_CMD_CIPSHUT: return "AT+CIPSHUT";
    case SIM900_CMD_CIPSTART: return "AT+CIPSTART";
    case SIM900_CMD_CIPSEND: return "AT+CIPSEND";
    case SIM900_CMD_CIPCLOSE: return "AT+CIPCLOSE";
    default: return "";
    }
}

#endif /* SIM900_CMDS_H */
