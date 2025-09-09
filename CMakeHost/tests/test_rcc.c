#include <assert.h>
#include "rcc.h"

int main(void) {
    assert(rcc_sysclk_config(RCC_SYSCLK_HSI8));
    assert(rcc_sysclk_config(RCC_SYSCLK_PLL_HSI_48MHZ));
    assert(rcc_sysclk_config_hse(8000000u, 24000000u));
    assert(rcc_sysclk_hz() == 24000000u);
    return 0;
}
