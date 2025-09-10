#ifndef AT25SF041_H
#define AT25SF041_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for AT25SF041 flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} at25sf041_t;

/** Initialize a AT25SF041 driver instance with the given SPI peripheral. */
bool at25sf041_init(at25sf041_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void at25sf041_read_id(at25sf041_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void at25sf041_read(at25sf041_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void at25sf041_page_program(at25sf041_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void at25sf041_sector_erase(at25sf041_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void at25sf041_block_erase_32k(at25sf041_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void at25sf041_block_erase_64k(at25sf041_t *flash, uint32_t addr);

/** Erase the entire chip. */
void at25sf041_chip_erase(at25sf041_t *flash);

/** Example demonstrating basic operations using SPI1. */
void at25sf041_example(void);

#endif /* AT25SF041_H */
