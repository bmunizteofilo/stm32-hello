#include <assert.h>
#include "dma.h"

int main(void) {
    DMA_Channel_TypeDef ch;
    assert(dma_config_channel(&ch, DMA_CCR_MINC));
    dma_set_peripheral(&ch, (void*)0x40000000u);
    dma_set_memory(&ch, (void*)0x20000000u);
    dma_set_count(&ch, 1);
    dma_enable(&ch, true);
    dma_enable(&ch, false);
    return 0;
}
