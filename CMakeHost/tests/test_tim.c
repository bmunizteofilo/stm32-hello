#include <assert.h>
#include <stddef.h>
#include "tim.h"
#include "rcc.h"

int main(void) {
    volatile uint32_t tim_mem[20] = {0};
    typedef struct {
        volatile uint32_t CR1, CR2, SMCR, DIER, SR, EGR, CCMR1, CCMR2,
                         CCER, CNT, PSC, ARR, RCR, CCR1, CCR2, CCR3, CCR4,
                         BDTR, DCR, DMAR;
    } tim_regs_t;
    tim_regs_t *tim = (tim_regs_t *)tim_mem;

    tim_cfg_t cfg = {
        .mode = TIM_COUNTER_UP,
        .prescaler = 0u,
        .period = 100u,
        .preload = false,
    };
    assert(!tim_init((TIM_TypeDef *)tim, &cfg));

    tim_oc_cfg_t oc = {
        .mode = TIM_OC_TOGGLE,
        .compare = 10u,
        .enable_irq = true,
    };
    assert(tim_config_output((TIM_TypeDef *)tim, 1u, &oc));
    assert(tim->CCR1 == 10u);
    assert((tim->CCMR1 & (0x7u << 4)) == (TIM_OC_TOGGLE << 4));
    assert(tim->DIER & TIM_DIER_CC1IE);
    tim_set_compare((TIM_TypeDef *)tim, 1u, 20u);
    assert(tim->CCR1 == 20u);

    tim_ic_cfg_t ic = {
        .polarity = TIM_IC_RISING,
        .enable_irq = true,
    };
    assert(tim_config_input((TIM_TypeDef *)tim, 2u, &ic));
    assert((tim->CCMR1 & (0xFFu << 8)) == (1u << 8));
    tim->CCR2 = 123u;
    assert(tim_get_capture((TIM_TypeDef *)tim, 2u) == 123u);

    tim_enable_dma((TIM_TypeDef *)tim, TIM_DIER_UDE | TIM_DIER_CC1DE, true);
    assert(tim->DIER & TIM_DIER_UDE);
    tim_enable_dma((TIM_TypeDef *)tim, TIM_DIER_UDE, false);
    assert((tim->DIER & TIM_DIER_UDE) == 0u);

    assert(!tim_config_output(NULL, 1u, &oc));
    assert(!tim_config_input(NULL, 1u, &ic));
    return 0;
}

