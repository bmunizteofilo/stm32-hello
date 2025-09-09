#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "dma.h"

/* Forward declaration for host builds */
typedef struct I2C_TypeDef I2C_TypeDef;

#define I2C1 ((I2C_TypeDef *)0x40005400u)
#define I2C2 ((I2C_TypeDef *)0x40005800u)

enum i2c_addr_mode {
    I2C_ADDR_7BIT,
    I2C_ADDR_10BIT,
};

typedef struct {
    enum i2c_addr_mode addressing_mode;
    uint32_t timing;
    uint16_t own_address;
} i2c_cfg_t;

/** Callback type for asynchronous I2C operations. */
typedef void (*i2c_cb_t)(void *ctx);

/** Initialize an I2C peripheral. */
bool i2c_init(I2C_TypeDef *i2c, const i2c_cfg_t *cfg);
/** Enable or disable an I2C peripheral. */
void i2c_enable(I2C_TypeDef *i2c, bool en);

/** Perform a blocking write transaction. */
bool i2c_write_poll(I2C_TypeDef *i2c, uint8_t addr, const uint8_t *data, size_t len);
/** Perform a blocking read transaction. */
bool i2c_read_poll(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data, size_t len);

/** Start an interrupt-driven write transaction. */
bool i2c_write_it_start(I2C_TypeDef *i2c, uint8_t addr, const uint8_t *data, size_t len, i2c_cb_t cb, void *ctx);
/** Start an interrupt-driven read transaction. */
bool i2c_read_it_start(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data, size_t len, i2c_cb_t cb, void *ctx);

/** Start a DMA write transaction. */
bool i2c_write_dma_start(I2C_TypeDef *i2c, DMA_Channel_TypeDef *tx_ch, uint8_t addr, const uint8_t *data, size_t len, i2c_cb_t cb, void *ctx);
/** Start a DMA read transaction. */
bool i2c_read_dma_start(I2C_TypeDef *i2c, DMA_Channel_TypeDef *rx_ch, uint8_t addr, uint8_t *data, size_t len, i2c_cb_t cb, void *ctx);

/** Example: polling write transaction. */
void i2c_example_write_poll(void);
/** Example: polling read transaction. */
void i2c_example_read_poll(void);
/** Example: interrupt-driven write transaction. */
void i2c_example_write_it(void);
/** Example: interrupt-driven read transaction. */
void i2c_example_read_it(void);
/** Example: DMA write transaction. */
void i2c_example_write_dma(void);
/** Example: DMA read transaction. */
void i2c_example_read_dma(void);

#endif /* I2C_H */
