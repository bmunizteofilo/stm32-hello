#ifndef IS25LP128_H
#define IS25LP128_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for IS25LP128 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} is25lp128_t;

/** Initialize a IS25LP128 driver instance with the given SPI peripheral. */
bool is25lp128_init(is25lp128_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void is25lp128_read_id(is25lp128_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void is25lp128_read(is25lp128_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void is25lp128_page_program(is25lp128_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void is25lp128_sector_erase(is25lp128_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void is25lp128_block_erase_32k(is25lp128_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void is25lp128_block_erase_64k(is25lp128_t *flash, uint32_t addr);

/** Erase the entire chip. */
void is25lp128_chip_erase(is25lp128_t *flash);

/** Example demonstrating basic operations using SPI1. */
void is25lp128_example(void);

#endif /* IS25LP128_H */
