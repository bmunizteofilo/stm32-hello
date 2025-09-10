#ifndef GD25Q32_H
#define GD25Q32_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for GD25Q32 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} gd25q32_t;

/** Initialize a GD25Q32 driver instance with the given SPI peripheral. */
bool gd25q32_init(gd25q32_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void gd25q32_read_id(gd25q32_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void gd25q32_read(gd25q32_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void gd25q32_page_program(gd25q32_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void gd25q32_sector_erase(gd25q32_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void gd25q32_block_erase_32k(gd25q32_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void gd25q32_block_erase_64k(gd25q32_t *flash, uint32_t addr);

/** Erase the entire chip. */
void gd25q32_chip_erase(gd25q32_t *flash);

/** Example demonstrating basic operations using SPI2. */
void gd25q32_example(void);

#endif /* GD25Q32_H */
