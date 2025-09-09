#include <assert.h>
#include "i2c.h"
#include "dma.h"
#include "rcc.h"

int main(void) {
    volatile uint32_t i2c_mem[11] = {0};
    i2c_mem[6] = 0xFFFFFFFFu; /* ISR flags ready */
    I2C_TypeDef *i2c = (I2C_TypeDef *)i2c_mem;
    i2c_cfg_t cfg = {
        .addressing_mode = I2C_ADDR_7BIT,
        .timing = 0u,
        .own_address = 0x32u,
    };
    assert(!i2c_init(i2c, &cfg));
    assert(!i2c_init(NULL, &cfg));
    uint8_t buf[4] = {0};
    DMA_Channel_TypeDef ch;
    assert(i2c_write_poll(i2c, 0x50u, buf, 4u));
    assert(i2c_read_poll(i2c, 0x50u, buf, 4u));
    assert(!i2c_write_it_start(i2c, 0x50u, buf, 4u, NULL, NULL));
    assert(!i2c_read_it_start(i2c, 0x50u, buf, 4u, NULL, buf));
    assert(!i2c_write_dma_start(i2c, &ch, 0x50u, buf, 4u, NULL, NULL));
    assert(!i2c_read_dma_start(i2c, &ch, 0x50u, buf, 4u, NULL, NULL));
    return 0;
}
