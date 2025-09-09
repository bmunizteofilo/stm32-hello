#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "dma.h"

typedef struct {
    volatile uint32_t ISR;
    volatile uint32_t IER;
    volatile uint32_t CR;
    volatile uint32_t CFGR1;
    volatile uint32_t CFGR2;
    volatile uint32_t SMPR;
    volatile uint32_t TR;
    volatile uint32_t CHSELR;
    volatile uint32_t DR;
} ADC_TypeDef;

#define ADC1_BASE 0x40012400u
#ifdef STM32F0_FIRMWARE
#define ADC1 ((ADC_TypeDef *)ADC1_BASE)
#else
extern ADC_TypeDef adc1_regs;
#define ADC1 (&adc1_regs)
#endif

/* ADC interrupt flags */
#define ADC_ISR_ADRDY (1u << 0)
#define ADC_ISR_EOC   (1u << 2)

/* ADC interrupt enable bits */
#define ADC_IER_EOCIE (1u << 2)

/* ADC control register bits */
#define ADC_CR_ADEN    (1u << 0)
#define ADC_CR_ADSTART (1u << 2)

/* ADC CFGR1 fields */
#define ADC_CFGR1_RES_SHIFT    3u
#define ADC_CFGR1_RES_MASK     (3u << ADC_CFGR1_RES_SHIFT)
#define ADC_CFGR1_EXTSEL_SHIFT 6u
#define ADC_CFGR1_EXTSEL_MASK  (0xFu << ADC_CFGR1_EXTSEL_SHIFT)
#define ADC_CFGR1_EXTEN_SHIFT  10u
#define ADC_CFGR1_EXTEN_MASK   (3u << ADC_CFGR1_EXTEN_SHIFT)
#define ADC_CFGR1_DMAEN        (1u << 0)

/* ADC SMPR fields */
#define ADC_SMPR_SMP_SHIFT 0u
#define ADC_SMPR_SMP_MASK  (7u << ADC_SMPR_SMP_SHIFT)

enum adc_resolution {
    ADC_RES_12BIT = 0,
    ADC_RES_10BIT = 1,
    ADC_RES_8BIT  = 2,
    ADC_RES_6BIT  = 3,
};

enum adc_sample_time {
    ADC_SMP_1_5 = 0,
    ADC_SMP_7_5,
    ADC_SMP_13_5,
    ADC_SMP_28_5,
    ADC_SMP_41_5,
    ADC_SMP_55_5,
    ADC_SMP_71_5,
    ADC_SMP_239_5,
};

enum adc_trigger {
    ADC_TRG_SOFTWARE = 0,
    ADC_TRG_TIM1_TRGO,
    ADC_TRG_TIM3_TRGO,
    ADC_TRG_EXTI11,
};

enum adc_trigger_edge {
    ADC_TRG_EDGE_NONE = 0,
    ADC_TRG_EDGE_RISING,
    ADC_TRG_EDGE_FALLING,
    ADC_TRG_EDGE_BOTH,
};

typedef struct {
    enum adc_resolution resolution;
    enum adc_sample_time sample_time;
    enum adc_trigger trigger;
    enum adc_trigger_edge trigger_edge;
} adc_cfg_t;

typedef void (*adc_cb_t)(void *ctx, uint16_t value);

bool adc_init(ADC_TypeDef *adc, const adc_cfg_t *cfg);
bool adc_read_poll(ADC_TypeDef *adc, uint8_t channel, uint16_t *value);
bool adc_start_it(ADC_TypeDef *adc, uint8_t channel, adc_cb_t cb, void *ctx);
bool adc_start_dma(ADC_TypeDef *adc, DMA_Channel_TypeDef *dma_ch, uint8_t channel,
                   uint16_t *buf, size_t len);

void ADC1_IRQHandler(void);

/* Usage examples (for reference only) */
void adc_example_poll_sw(void);
void adc_example_poll_hw(void);
void adc_example_it_sw(void);
void adc_example_it_hw(void);
void adc_example_dma_sw(void);
void adc_example_dma_hw(void);

#endif /* ADC_H */
