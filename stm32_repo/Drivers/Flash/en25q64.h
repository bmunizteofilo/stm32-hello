#ifndef EN25Q64_H
#define EN25Q64_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for EN25Q64 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} en25q64_t;

/** Initialize a EN25Q64 driver instance with the given SPI peripheral. */
bool en25q64_init(en25q64_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void en25q64_read_id(en25q64_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void en25q64_read(en25q64_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void en25q64_page_program(en25q64_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void en25q64_sector_erase(en25q64_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void en25q64_block_erase_32k(en25q64_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void en25q64_block_erase_64k(en25q64_t *flash, uint32_t addr);

/** Erase the entire chip. */
void en25q64_chip_erase(en25q64_t *flash);

/** Example demonstrating basic operations using SPI1. */
void en25q64_example(void);

#endif /* EN25Q64_H */
