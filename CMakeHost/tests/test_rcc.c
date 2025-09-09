#include <assert.h>
#include "rcc.h"

int main(void) {
    assert(rcc_sysclk_config(RCC_SYSCLK_HSI8));
    assert(rcc_sysclk_hz() == 8000000u);
    assert(rcc_flash_latency_ws() == 0u);

    assert(rcc_sysclk_config(RCC_SYSCLK_HSE8));
    assert(rcc_sysclk_hz() == 8000000u);
    assert(rcc_flash_latency_ws() == 0u);

    assert(rcc_sysclk_config(RCC_SYSCLK_PLL_HSI_24MHZ));
    assert(rcc_sysclk_hz() == 24000000u);
    assert(rcc_flash_latency_ws() == 0u);

    assert(rcc_sysclk_config(RCC_SYSCLK_PLL_HSI_48MHZ));
    assert(rcc_sysclk_hz() == 48000000u);
    assert(rcc_flash_latency_ws() == 1u);

    assert(rcc_sysclk_config(RCC_SYSCLK_PLL_HSE_24MHZ));
    assert(rcc_sysclk_hz() == 24000000u);
    assert(rcc_flash_latency_ws() == 0u);

    assert(rcc_sysclk_config(RCC_SYSCLK_PLL_HSE_48MHZ));
    assert(rcc_sysclk_hz() == 48000000u);
    assert(rcc_flash_latency_ws() == 1u);
    return 0;
}
