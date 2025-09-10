#ifndef SIM900_URC_H
#define SIM900_URC_H

/**
 * \file sim900_urc.h
 * \brief URC parsing helpers.
 */

#include "sim900_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SIM900_URC_RING,
    SIM900_URC_CLIP,
    SIM900_URC_CMTI,
    SIM900_URC_CUSD,
    SIM900_URC_UNKNOWN
} sim900_urc_type_t;

static inline const char *sim900_urc_type_str(sim900_urc_type_t t)
{
    switch (t) {
    case SIM900_URC_RING: return "RING";
    case SIM900_URC_CLIP: return "+CLIP";
    case SIM900_URC_CMTI: return "+CMTI";
    case SIM900_URC_CUSD: return "+CUSD";
    default: return "";
    }
}

#ifdef __cplusplus
}
#endif

#endif /* SIM900_URC_H */
