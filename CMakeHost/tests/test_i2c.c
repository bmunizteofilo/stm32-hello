#include <assert.h>
#include "i2c.h"
#include "dma.h"

int main(void) {
    i2c_cfg_t cfg = {
        .addressing_mode = I2C_ADDR_7BIT,
        .timing = 0u,
        .own_address = 0x32u,
    };
    assert(i2c_init((I2C_TypeDef *)0x40005400u, &cfg));
    assert(!i2c_init(NULL, &cfg));
    uint8_t buf[4] = {0};
    DMA_Channel_TypeDef ch;
    assert(i2c_write_poll((I2C_TypeDef *)0x40005400u, 0x50u, buf, 4u));
    assert(i2c_read_poll((I2C_TypeDef *)0x40005400u, 0x50u, buf, 4u));
    assert(i2c_write_it_start((I2C_TypeDef *)0x40005400u, 0x50u, buf, 4u, NULL, NULL));
    assert(i2c_read_it_start((I2C_TypeDef *)0x40005400u, 0x50u, buf, 4u, NULL, buf));
    assert(i2c_write_dma_start((I2C_TypeDef *)0x40005400u, &ch, 0x50u, buf, 4u, NULL, NULL));
    assert(i2c_read_dma_start((I2C_TypeDef *)0x40005400u, &ch, 0x50u, buf, 4u, NULL, NULL));
    return 0;
}
