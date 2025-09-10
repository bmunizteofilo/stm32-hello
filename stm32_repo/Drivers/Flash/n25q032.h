#ifndef N25Q032_H
#define N25Q032_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for N25Q032 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} n25q032_t;

/** Initialize a N25Q032 driver instance with the given SPI peripheral. */
bool n25q032_init(n25q032_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void n25q032_read_id(n25q032_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void n25q032_read(n25q032_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void n25q032_page_program(n25q032_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void n25q032_sector_erase(n25q032_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void n25q032_block_erase_32k(n25q032_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void n25q032_block_erase_64k(n25q032_t *flash, uint32_t addr);

/** Erase the entire chip. */
void n25q032_chip_erase(n25q032_t *flash);

/** Example demonstrating basic operations using SPI2. */
void n25q032_example(void);

#endif /* N25Q032_H */
