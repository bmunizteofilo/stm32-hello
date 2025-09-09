#include <assert.h>
#include "rcc.h"

RCC_TypeDef rcc_regs;
FLASH_TypeDef flash_regs;

int main(void) {
    rcc_regs.CR = (1u << 1) | (1u << 17) | (1u << 25);
    assert(rcc_sysclk_config(RCC_SYSCLK_HSI8));
    assert(rcc_sysclk_config(RCC_SYSCLK_PLL_HSI_48MHZ));
    assert(rcc_sysclk_config_hse(8000000u, 24000000u));
    assert(rcc_sysclk_hz() == 24000000u);
    rcc_regs.CFGR = 0u;
    rcc_mco_enable_sysclk();
    assert((rcc_regs.CFGR & RCC_CFGR_MCO_MASK) == RCC_CFGR_MCO_SYSCLK);
    return 0;
}
