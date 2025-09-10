#ifndef A25LQ64_H
#define A25LQ64_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for A25LQ64 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} a25lq64_t;

/** Initialize a A25LQ64 driver instance with the given SPI peripheral. */
bool a25lq64_init(a25lq64_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void a25lq64_read_id(a25lq64_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void a25lq64_read(a25lq64_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void a25lq64_page_program(a25lq64_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void a25lq64_sector_erase(a25lq64_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void a25lq64_block_erase_32k(a25lq64_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void a25lq64_block_erase_64k(a25lq64_t *flash, uint32_t addr);

/** Erase the entire chip. */
void a25lq64_chip_erase(a25lq64_t *flash);

/** Example demonstrating basic operations using SPI2. */
void a25lq64_example(void);

#endif /* A25LQ64_H */
