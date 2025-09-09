/*
 * ADC driver for STM32F0 without HAL.
 * Supports polling, interrupt and DMA modes with basic configuration.
 */

#include "adc.h"
#include "rcc.h"
#include "cm0.h"

static adc_cb_t adc_cb;
static void *adc_ctx;

ADC_TypeDef adc1_regs;

bool adc_init(ADC_TypeDef *adc, const adc_cfg_t *cfg) {
    if (!adc || !cfg) {
        return false;
    }
    rcc_apb2_enable(RCC_APB2ENR_ADC1);

    adc->CR = 0u;
    adc->CFGR1 &= ~(ADC_CFGR1_RES_MASK | ADC_CFGR1_EXTSEL_MASK | ADC_CFGR1_EXTEN_MASK);
    adc->CFGR1 |= ((uint32_t)cfg->resolution << ADC_CFGR1_RES_SHIFT);
    if (cfg->trigger != ADC_TRG_SOFTWARE) {
        adc->CFGR1 |= ((uint32_t)cfg->trigger << ADC_CFGR1_EXTSEL_SHIFT);
        adc->CFGR1 |= ((uint32_t)cfg->trigger_edge << ADC_CFGR1_EXTEN_SHIFT);
    }
    adc->SMPR = ((uint32_t)cfg->sample_time << ADC_SMPR_SMP_SHIFT);

    adc->CR |= ADC_CR_ADEN;
    adc->ISR |= ADC_ISR_ADRDY; /* ready immediately in host build */
    return true;
}

bool adc_read_poll(ADC_TypeDef *adc, uint8_t channel, uint16_t *value) {
    if (!adc || !value) {
        return false;
    }
    adc->CHSELR = (1u << channel);
    adc->CR |= ADC_CR_ADSTART;
    while ((adc->ISR & ADC_ISR_EOC) == 0u) {}
    *value = (uint16_t)adc->DR;
    adc->ISR &= ~ADC_ISR_EOC;
    return true;
}

bool adc_start_it(ADC_TypeDef *adc, uint8_t channel, adc_cb_t cb, void *ctx) {
    if (!adc || !cb) {
        return false;
    }
    adc_cb = cb;
    adc_ctx = ctx;
    adc->CHSELR = (1u << channel);
    adc->IER |= ADC_IER_EOCIE;
    cm0_nvic_enable(ADC1_IRQn);
    adc->CR |= ADC_CR_ADSTART;
    return true;
}

bool adc_start_dma(ADC_TypeDef *adc, DMA_Channel_TypeDef *dma_ch, uint8_t channel,
                   uint16_t *buf, size_t len) {
    if (!adc || !dma_ch || !buf || len == 0u) {
        return false;
    }
    adc->CHSELR = (1u << channel);
    adc->CFGR1 |= ADC_CFGR1_DMAEN;
    dma_set_peripheral(dma_ch, (const void *)&adc->DR);
    dma_set_memory(dma_ch, buf);
    dma_set_count(dma_ch, (uint16_t)len);
    dma_enable(dma_ch, true);
    adc->CR |= ADC_CR_ADSTART;
    return true;
}

void ADC1_IRQHandler(void) {
    if ((ADC1->ISR & ADC_ISR_EOC) && adc_cb) {
        uint16_t val = (uint16_t)ADC1->DR;
        ADC1->ISR &= ~ADC_ISR_EOC;
        adc_cb(adc_ctx, val);
    }
}

/* Example functions demonstrating usage */
static void adc_dummy_cb(void *ctx, uint16_t v) {
    (void)ctx;
    (void)v;
}

void adc_example_poll_sw(void) {
    adc_cfg_t cfg = {
        .resolution = ADC_RES_12BIT,
        .sample_time = ADC_SMP_7_5,
        .trigger = ADC_TRG_SOFTWARE,
        .trigger_edge = ADC_TRG_EDGE_NONE,
    };
    adc_init(ADC1, &cfg);
    uint16_t value;
    adc_read_poll(ADC1, 5u, &value);
}

void adc_example_poll_hw(void) {
    adc_cfg_t cfg = {
        .resolution = ADC_RES_12BIT,
        .sample_time = ADC_SMP_7_5,
        .trigger = ADC_TRG_TIM1_TRGO,
        .trigger_edge = ADC_TRG_EDGE_RISING,
    };
    adc_init(ADC1, &cfg);
    uint16_t value;
    adc_read_poll(ADC1, 3u, &value);
}

void adc_example_it_sw(void) {
    adc_cfg_t cfg = {
        .resolution = ADC_RES_8BIT,
        .sample_time = ADC_SMP_1_5,
        .trigger = ADC_TRG_SOFTWARE,
        .trigger_edge = ADC_TRG_EDGE_NONE,
    };
    adc_init(ADC1, &cfg);
    adc_start_it(ADC1, 0u, adc_dummy_cb, NULL);
}

void adc_example_it_hw(void) {
    adc_cfg_t cfg = {
        .resolution = ADC_RES_10BIT,
        .sample_time = ADC_SMP_13_5,
        .trigger = ADC_TRG_TIM3_TRGO,
        .trigger_edge = ADC_TRG_EDGE_BOTH,
    };
    adc_init(ADC1, &cfg);
    adc_start_it(ADC1, 1u, adc_dummy_cb, NULL);
}

void adc_example_dma_sw(void) {
    uint16_t buffer[8];
    adc_cfg_t cfg = {
        .resolution = ADC_RES_12BIT,
        .sample_time = ADC_SMP_55_5,
        .trigger = ADC_TRG_SOFTWARE,
        .trigger_edge = ADC_TRG_EDGE_NONE,
    };
    adc_init(ADC1, &cfg);
    DMA_Channel_TypeDef ch;
    adc_start_dma(ADC1, &ch, 2u, buffer, 8u);
}

void adc_example_dma_hw(void) {
    uint16_t buffer[4];
    adc_cfg_t cfg = {
        .resolution = ADC_RES_12BIT,
        .sample_time = ADC_SMP_71_5,
        .trigger = ADC_TRG_EXTI11,
        .trigger_edge = ADC_TRG_EDGE_FALLING,
    };
    adc_init(ADC1, &cfg);
    DMA_Channel_TypeDef ch;
    adc_start_dma(ADC1, &ch, 4u, buffer, 4u);
}

