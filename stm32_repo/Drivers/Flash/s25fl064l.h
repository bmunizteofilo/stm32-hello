#ifndef S25FL064L_H
#define S25FL064L_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for S25FL064L flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} s25fl064l_t;

/** Initialize a S25FL064L driver instance with the given SPI peripheral. */
bool s25fl064l_init(s25fl064l_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void s25fl064l_read_id(s25fl064l_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void s25fl064l_read(s25fl064l_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void s25fl064l_page_program(s25fl064l_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void s25fl064l_sector_erase(s25fl064l_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void s25fl064l_block_erase_32k(s25fl064l_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void s25fl064l_block_erase_64k(s25fl064l_t *flash, uint32_t addr);

/** Erase the entire chip. */
void s25fl064l_chip_erase(s25fl064l_t *flash);

/** Example demonstrating basic operations using SPI1. */
void s25fl064l_example(void);

#endif /* S25FL064L_H */
