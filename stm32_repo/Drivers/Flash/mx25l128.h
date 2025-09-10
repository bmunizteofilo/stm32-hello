#ifndef MX25L128_H
#define MX25L128_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for MX25L128 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} mx25l128_t;

/** Initialize a MX25L128 driver instance with the given SPI peripheral. */
bool mx25l128_init(mx25l128_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void mx25l128_read_id(mx25l128_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void mx25l128_read(mx25l128_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void mx25l128_page_program(mx25l128_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void mx25l128_sector_erase(mx25l128_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void mx25l128_block_erase_32k(mx25l128_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void mx25l128_block_erase_64k(mx25l128_t *flash, uint32_t addr);

/** Erase the entire chip. */
void mx25l128_chip_erase(mx25l128_t *flash);

/** Example demonstrating basic operations using SPI2. */
void mx25l128_example(void);

#endif /* MX25L128_H */
