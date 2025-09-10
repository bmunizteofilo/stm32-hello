#ifndef SIM900_UTF_H
#define SIM900_UTF_H

/**
 * \file sim900_utf.h
 * \brief UCS2/UTF-8 helper functions.
 */

#include <stddef.h>
#include <stdint.h>

size_t sim900_utf8_to_ucs2_hex(const char *utf8, char *hex, size_t hexsz);
size_t sim900_ucs2_hex_to_utf8(const char *hex, char *utf8, size_t utf8sz);

#endif /* SIM900_UTF_H */
