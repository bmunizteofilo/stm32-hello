#include <assert.h>
#include "rcc.h"

int main(void) {
    assert(rcc_sysclk_config(RCC_SYSCLK_HSI8));
    assert(rcc_sysclk_config(RCC_SYSCLK_PLL_HSI_48MHZ));
    assert(rcc_sysclk_hz() == 8000000u);
    return 0;
}
