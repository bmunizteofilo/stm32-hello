#ifndef SIM900_PBK_H
#define SIM900_PBK_H

/**
 * \file sim900_pbk.h
 * \brief Phonebook helpers.
 */

#include <stdbool.h>
#include <stddef.h>

#include "sim900.h"

bool sim900_pbk_set_storage(const char *mem);
bool sim900_pbk_write(int index, const char *name_utf8, const char *num_e164);
bool sim900_pbk_read(int index, char *name_utf8, size_t name_sz, char *num_e164, size_t num_sz);
bool sim900_pbk_delete(int index);

#endif /* SIM900_PBK_H */
