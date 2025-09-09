#include <assert.h>
#include "adc.h"

extern ADC_TypeDef adc1_regs;

static void cb(void *ctx, uint16_t val) {
    (void)ctx;
    *(uint16_t *)ctx = val;
}

int main(void) {
    adc1_regs.ISR = 0u;
    adc1_regs.IER = 0u;
    adc1_regs.CR = 0u;
    adc1_regs.CFGR1 = 0u;
    adc1_regs.SMPR = 0u;
    ADC_TypeDef *adc = &adc1_regs;
    adc_cfg_t cfg = {
        .resolution = ADC_RES_12BIT,
        .sample_time = ADC_SMP_1_5,
        .trigger = ADC_TRG_SOFTWARE,
        .trigger_edge = ADC_TRG_EDGE_NONE,
    };
    assert(adc_init(adc, &cfg));
    adc1_regs.DR = 0x123u;
    adc1_regs.ISR |= ADC_ISR_EOC;
    uint16_t value = 0;
    assert(adc_read_poll(adc, 3u, &value));
    assert(value == 0x123u);

    adc1_regs.DR = 0x55u;
    adc1_regs.ISR |= ADC_ISR_EOC;
    uint16_t it_val = 0;
    assert(adc_start_it(adc, 2u, cb, &it_val));
    ADC1_IRQHandler();
    assert(it_val == 0x55u);

    return 0;
}
