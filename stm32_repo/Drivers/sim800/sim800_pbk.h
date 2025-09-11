#ifndef SIM800_PBK_H
#define SIM800_PBK_H

#include <stdbool.h>
#include <stddef.h>
#include "sim800.h"

bool sim800_pbk_set_storage(const char *mem);
bool sim800_pbk_write(int index, const char *name_utf8, const char *num_e164);
bool sim800_pbk_read(int index, char *name_utf8, size_t name_sz, char *num_e164, size_t num_sz);
bool sim800_pbk_delete(int index);

#endif /* SIM800_PBK_H */
