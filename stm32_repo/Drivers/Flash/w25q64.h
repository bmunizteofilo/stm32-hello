#ifndef W25Q64_H
#define W25Q64_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for W25Q64 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} w25q64_t;

/** Initialize a W25Q64 driver instance with the given SPI peripheral. */
bool w25q64_init(w25q64_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void w25q64_read_id(w25q64_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void w25q64_read(w25q64_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void w25q64_page_program(w25q64_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void w25q64_sector_erase(w25q64_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void w25q64_block_erase_32k(w25q64_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void w25q64_block_erase_64k(w25q64_t *flash, uint32_t addr);

/** Erase the entire chip. */
void w25q64_chip_erase(w25q64_t *flash);

/** Example demonstrating basic operations using SPI1. */
void w25q64_example(void);

#endif /* W25Q64_H */
