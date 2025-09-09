#include <assert.h>
#include "usart.h"
#include "dma.h"
#include "rcc.h"

RCC_TypeDef rcc_regs;
FLASH_TypeDef flash_regs;

int main(void) {
    volatile uint32_t usart_mem[11] = {0};
    USART_TypeDef *usart = (USART_TypeDef *)usart_mem;
    usart_cfg_t cfg = {
        .baudrate = 115200u,
        .word_length = USART_WORD_8,
        .stop_bits = USART_STOP_1,
        .parity = USART_PARITY_NONE,
        .mode = USART_MODE_TX_RX,
        .hw_flow = USART_HW_NONE,
        .oversampling = USART_OVER16,
    };
    assert(!usart_init(usart, &cfg));
    assert(!usart_write_poll(usart, NULL, 1));
    assert(!usart_read_poll(usart, NULL, 1));
    DMA_Channel_TypeDef ch;
    assert(!usart_write_it_start(usart, NULL, 1, NULL, NULL));
    assert(!usart_read_it_start(usart, NULL, 1, NULL, NULL));
    assert(!usart_write_dma_start(usart, &ch, NULL, 1, NULL, NULL));
    assert(!usart_read_dma_start(usart, &ch, NULL, 1, NULL, NULL));
    return 0;
}
