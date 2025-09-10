#ifndef SST26VF064B_H
#define SST26VF064B_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"

/** Driver context for SST26VF064B flash memory. */
typedef struct {
    SPI_TypeDef *spi; /**< SPI peripheral used for communication */
} sst26vf064b_t;

/** Initialize a SST26VF064B driver instance with the given SPI peripheral. */
bool sst26vf064b_init(sst26vf064b_t *flash, SPI_TypeDef *spi);

/** Read JEDEC manufacturer and device IDs. */
void sst26vf064b_read_id(sst26vf064b_t *flash, uint8_t *manuf, uint16_t *dev);

/** Read @p len bytes starting at @p addr into @p data. */
void sst26vf064b_read(sst26vf064b_t *flash, uint32_t addr, uint8_t *data, size_t len);

/** Program up to 256 bytes starting at @p addr. */
void sst26vf064b_page_program(sst26vf064b_t *flash, uint32_t addr, const uint8_t *data, size_t len);

/** Erase 4KB sector containing @p addr. */
void sst26vf064b_sector_erase(sst26vf064b_t *flash, uint32_t addr);

/** Erase 32KB block containing @p addr. */
void sst26vf064b_block_erase_32k(sst26vf064b_t *flash, uint32_t addr);

/** Erase 64KB block containing @p addr. */
void sst26vf064b_block_erase_64k(sst26vf064b_t *flash, uint32_t addr);

/** Erase the entire chip. */
void sst26vf064b_chip_erase(sst26vf064b_t *flash);

/** Example demonstrating basic operations using SPI2. */
void sst26vf064b_example(void);

#endif /* SST26VF064B_H */
