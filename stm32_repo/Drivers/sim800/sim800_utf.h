#ifndef SIM800_UTF_H
#define SIM800_UTF_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool sim800_utf8_to_ucs2_hex(const char *utf8, char *hex, size_t hexsz);
bool sim800_ucs2_hex_to_utf8(const char *hex, char *utf8, size_t utf8sz);

#endif /* SIM800_UTF_H */
