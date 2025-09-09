/*
 * Timer driver for STM32F0 without HAL.
 */

#include "tim.h"
#include "rcc.h"
#include "cm0.h"
#include "gpio.h"
#include "gpio_af.h"

struct TIM_TypeDef {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
};

typedef struct { tim_cb_t cb; void *ctx; } tim_cb_entry_t;

static tim_cb_entry_t tim_update_cb[7];
static tim_cb_entry_t tim_cc_cb[7][4];

/** Map timer pointer to index for callback arrays. */
static uint8_t tim_index(TIM_TypeDef *tim) {
    if (tim == TIM1) return 0u;
    if (tim == TIM2) return 1u;
    if (tim == TIM3) return 2u;
    if (tim == TIM14) return 3u;
    if (tim == TIM15) return 4u;
    if (tim == TIM16) return 5u;
    if (tim == TIM17) return 6u;
    return 0xFFu;
}

/** Determine NVIC IRQ number for a timer. */
static IRQn_Type tim_irqn(TIM_TypeDef *tim) {
    if (tim == TIM1) return TIM1_BRK_UP_TRG_COM_IRQn;
    if (tim == TIM2) return TIM2_IRQn;
    if (tim == TIM3) return TIM3_IRQn;
    if (tim == TIM14) return TIM14_IRQn;
    if (tim == TIM15) return TIM15_IRQn;
    if (tim == TIM16) return TIM16_IRQn;
    if (tim == TIM17) return TIM17_IRQn;
    return (IRQn_Type)0;
}

/** Enable RCC clock for the given timer. */
static bool tim_rcc_enable(TIM_TypeDef *tim) {
    if (tim == TIM1) { rcc_apb2_enable(RCC_APB2ENR_TIM1); return true; }
    if (tim == TIM2) { rcc_apb1_enable(RCC_APB1ENR_TIM2); return true; }
    if (tim == TIM3) { rcc_apb1_enable(RCC_APB1ENR_TIM3); return true; }
    if (tim == TIM14) { rcc_apb1_enable(RCC_APB1ENR_TIM14); return true; }
    if (tim == TIM15) { rcc_apb2_enable(RCC_APB2ENR_TIM15); return true; }
    if (tim == TIM16) { rcc_apb2_enable(RCC_APB2ENR_TIM16); return true; }
    if (tim == TIM17) { rcc_apb2_enable(RCC_APB2ENR_TIM17); return true; }
    return false;
}

/** Initialize timer with basic configuration. */
bool tim_init(TIM_TypeDef *tim, const tim_cfg_t *cfg) {
    if (!tim || !cfg) return false;
    if (!tim_rcc_enable(tim)) return false;
    uint32_t cr1 = 0u;
    if (cfg->mode == TIM_COUNTER_DOWN) cr1 |= TIM_CR1_DIR;
    if (cfg->preload) cr1 |= TIM_CR1_ARPE;
    tim->CR1 = cr1;
    tim->PSC = cfg->prescaler;
    tim->ARR = cfg->period;
    tim->EGR = 1u;
    return true;
}

/** Enable or disable timer counting. */
void tim_enable(TIM_TypeDef *tim, bool en) {
    if (!tim) return;
    if (en) tim->CR1 |= TIM_CR1_CEN; else tim->CR1 &= ~TIM_CR1_CEN;
}

/** Return pointer to CCR register for a channel. */
static volatile uint32_t *tim_ccr(TIM_TypeDef *tim, uint8_t ch) {
    return &(&tim->CCR1)[ch - 1u];
}

/** Configure an output compare channel. */
bool tim_config_output(TIM_TypeDef *tim, uint8_t ch, const tim_oc_cfg_t *cfg) {
    if (!tim || !cfg || ch == 0u || ch > 4u) return false;
    volatile uint32_t *ccmr = (ch <= 2u) ? &tim->CCMR1 : &tim->CCMR2;
    uint32_t shift = ((ch - 1u) % 2u) * 8u;
    *ccmr &= ~(0xFFu << shift);
    *ccmr |= ((uint32_t)cfg->mode & 0x7u) << (shift + 4u);
    tim->CCER |= 1u << ((ch - 1u) * 4u);
    tim_set_compare(tim, ch, cfg->compare);
    if (cfg->enable_irq) tim->DIER |= (1u << ch); else tim->DIER &= ~(1u << ch);
    return true;
}

/** Update compare value of a channel. */
void tim_set_compare(TIM_TypeDef *tim, uint8_t ch, uint32_t compare) {
    if (!tim || ch == 0u || ch > 4u) return;
    *tim_ccr(tim, ch) = compare;
}

/** Configure an input capture channel. */
bool tim_config_input(TIM_TypeDef *tim, uint8_t ch, const tim_ic_cfg_t *cfg) {
    if (!tim || !cfg || ch == 0u || ch > 4u) return false;
    volatile uint32_t *ccmr = (ch <= 2u) ? &tim->CCMR1 : &tim->CCMR2;
    uint32_t shift = ((ch - 1u) % 2u) * 8u;
    *ccmr &= ~(0xFFu << shift);
    *ccmr |= (1u << shift);
    uint32_t ccer_shift = (ch - 1u) * 4u;
    uint32_t ccer = tim->CCER & ~(5u << ccer_shift);
    if (cfg->polarity == TIM_IC_FALLING) {
        ccer |= (1u << (ccer_shift + 1u));
    } else if (cfg->polarity == TIM_IC_BOTH) {
        ccer |= (1u << (ccer_shift + 1u)) | (1u << (ccer_shift + 3u));
    }
    tim->CCER = ccer | (1u << ccer_shift);
    if (cfg->enable_irq) tim->DIER |= (1u << ch); else tim->DIER &= ~(1u << ch);
    return true;
}

/** Read captured value. */
uint32_t tim_get_capture(TIM_TypeDef *tim, uint8_t ch) {
    if (!tim || ch == 0u || ch > 4u) return 0u;
    return *tim_ccr(tim, ch);
}

/** Enable or disable DMA requests. */
void tim_enable_dma(TIM_TypeDef *tim, uint32_t mask, bool en) {
    if (!tim) return;
    if (en) tim->DIER |= mask; else tim->DIER &= ~mask;
}

/** Register callback for update events. */
void tim_attach_update_irq(TIM_TypeDef *tim, tim_cb_t cb, void *ctx) {
    uint8_t idx = tim_index(tim);
    if (idx == 0xFFu) return;
    tim_update_cb[idx].cb = cb;
    tim_update_cb[idx].ctx = ctx;
    if (cb) {
        tim->DIER |= TIM_DIER_UIE;
        cm0_nvic_enable(tim_irqn(tim));
    } else {
        tim->DIER &= ~TIM_DIER_UIE;
    }
}

/** Register callback for capture/compare events. */
void tim_attach_cc_irq(TIM_TypeDef *tim, uint8_t ch, tim_cb_t cb, void *ctx) {
    if (ch == 0u || ch > 4u) return;
    uint8_t idx = tim_index(tim);
    if (idx == 0xFFu) return;
    tim_cc_cb[idx][ch - 1u].cb = cb;
    tim_cc_cb[idx][ch - 1u].ctx = ctx;
    if (cb) {
        tim->DIER |= (1u << ch);
        cm0_nvic_enable(tim_irqn(tim));
    } else {
        tim->DIER &= ~(1u << ch);
    }
}

/** Detach callback for update events. */
void tim_detach_update_irq(TIM_TypeDef *tim) {
    uint8_t idx = tim_index(tim);
    if (idx == 0xFFu) return;
    tim_update_cb[idx].cb = NULL;
    tim_update_cb[idx].ctx = NULL;
    tim->DIER &= ~TIM_DIER_UIE;
}

/** Detach callback for capture/compare events. */
void tim_detach_cc_irq(TIM_TypeDef *tim, uint8_t ch) {
    if (ch == 0u || ch > 4u) return;
    uint8_t idx = tim_index(tim);
    if (idx == 0xFFu) return;
    tim_cc_cb[idx][ch - 1u].cb = NULL;
    tim_cc_cb[idx][ch - 1u].ctx = NULL;
    tim->DIER &= ~(1u << ch);
}

/** Dispatch timer interrupts to registered callbacks. */
static void tim_irq_dispatch(TIM_TypeDef *tim) {
    uint8_t idx = tim_index(tim);
    if (idx == 0xFFu) return;
    uint32_t sr = tim->SR;
    if ((sr & TIM_SR_UIF) && tim_update_cb[idx].cb) {
        tim->SR &= ~TIM_SR_UIF;
        tim_update_cb[idx].cb(tim_update_cb[idx].ctx);
    }
    for (uint8_t ch = 1u; ch <= 4u; ++ch) {
        uint32_t mask = TIM_SR_CC1IF << (ch - 1u);
        if ((sr & mask) && tim_cc_cb[idx][ch - 1u].cb) {
            tim->SR &= ~mask;
            tim_cc_cb[idx][ch - 1u].cb(tim_cc_cb[idx][ch - 1u].ctx);
        }
    }
}

/** TIM1 update and trigger interrupt handler. */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void) { tim_irq_dispatch(TIM1); }
/** TIM2 global interrupt handler. */
void TIM2_IRQHandler(void) { tim_irq_dispatch(TIM2); }
/** TIM3 global interrupt handler. */
void TIM3_IRQHandler(void) { tim_irq_dispatch(TIM3); }
/** TIM14 global interrupt handler. */
void TIM14_IRQHandler(void) { tim_irq_dispatch(TIM14); }
/** TIM15 global interrupt handler. */
void TIM15_IRQHandler(void) { tim_irq_dispatch(TIM15); }
/** TIM16 global interrupt handler. */
void TIM16_IRQHandler(void) { tim_irq_dispatch(TIM16); }
/** TIM17 global interrupt handler. */
void TIM17_IRQHandler(void) { tim_irq_dispatch(TIM17); }

/* Último período medido pelo exemplo de captura (em contagens do timer) */
volatile uint32_t tim_ic_last_period;

/* Armazena o período entre capturas sucessivas e pisca PC9 */
/** Callback computing period between captures and toggling LED. */
static void tim_capture_cb(void *ctx) {
    (void)ctx;
    static uint32_t prev;
    uint32_t now = tim_get_capture(TIM3, 2u);
    tim_ic_last_period = now - prev;
    prev = now;
    gpio_toggle(GPIOC, 9);
}

/* Pisca PC8 a cada evento de atualização */
/** Callback toggling LED on update events. */
static void tim_blink_cb(void *ctx) {
    (void)ctx;
    gpio_toggle(GPIOC, 8);
}

/*
 * Exemplo: gera PWM no TIM3 canal 1 (PA6, AF1).
 * PSC = 799 e ARR = 999 resultam em aproximadamente 60 Hz quando o clock do
 * timer é 48 MHz. O valor de compare em 500 produz duty cycle de 50%, ideal
 * para controlar brilho de LED ou a velocidade de uma ventoinha.
 * Conecte um LED com resistor a PA6 para visualizar o PWM.
 */
/** Example: generate PWM on TIM3 channel 1. */
void tim_example_pwm(void) {
    gpio_config(GPIOA, 6, &(gpio_cfg_t){
        .mode = GPIO_MODE_AF,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_HIGH,
        .alternate = PA6_AF1_TIM3_CH1,
    });

    tim_init(TIM3, &(tim_cfg_t){
        .mode = TIM_COUNTER_UP,
        .prescaler = 799u,
        .period = 999u,
        .preload = true,
    });
    /* Usa o recurso de Output Compare em modo PWM1 */
    tim_config_output(TIM3, 1u, &(tim_oc_cfg_t){
        .mode = TIM_OC_PWM1,
        .compare = 500u, /* 50% de duty */
    });
    tim_enable(TIM3, true);
}

/*
 * Exemplo: captura de entradas usando TIM3 canal 2 (PA7, AF1).
 * O contador sobe até 0xFFFF com PSC = 799, oferecendo resolução de
 * aproximadamente 13 µs para clock de 48 MHz. Cada borda de subida gera uma
 * interrupção que calcula o período entre pulsos e armazena o valor em
 * `tim_ic_last_period`. Além disso, o callback pisca o LED em PC9 a cada
 * captura. Para testar, alimente PA7 com um sinal quadrado de 0--3,3 V
 * (por exemplo, saída do gerador de funções ou do exemplo de PWM).
*/
/** Example: input capture using TIM3 channel 2. */
void tim_example_input_capture(void) {
    gpio_config(GPIOC, 9, &(gpio_cfg_t){
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    });

    gpio_config(GPIOA, 7, &(gpio_cfg_t){
        .mode = GPIO_MODE_AF,
        .pull = GPIO_PULL_DOWN,
        .alternate = PA7_AF1_TIM3_CH2,
    });

    tim_init(TIM3, &(tim_cfg_t){
        .mode = TIM_COUNTER_UP,
        .prescaler = 799u,
        .period = 0xFFFFu,
    });
    tim_config_input(TIM3, 2u, &(tim_ic_cfg_t){
        .polarity = TIM_IC_RISING,
        .enable_irq = true,
    });
    tim_attach_cc_irq(TIM3, 2u, tim_capture_cb, NULL);
    tim_enable(TIM3, true);
}

/*
 * Exemplo: interrupção periódica por evento de atualização.
 * Com PSC = 7999 e ARR = 9999, o TIM3 gera uma interrupção a cada ~1,6 s
 * (clock de 48 MHz). O callback pisca o LED conectado em PC8, mostrando como
 * usar o timer para tarefas periódicas.
 */
/** Example: periodic update interrupt toggling PC8. */
void tim_example_update_irq(void) {
    gpio_config(GPIOC, 8, &(gpio_cfg_t){
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_LOW,
    });

    tim_init(TIM3, &(tim_cfg_t){
        .mode = TIM_COUNTER_UP,
        .prescaler = 7999u,
        .period = 9999u,
    });
    tim_attach_update_irq(TIM3, tim_blink_cb, NULL);
    tim_enable(TIM3, true);
}

/*
 * Exemplo: PWM de 50 Hz para servo no TIM3 canal 3 (PB0, AF1).
 * A base de tempo de 10 kHz (PSC = 4799) gera período de 20 ms com ARR = 199.
 * O compare em 15 produz pulso de 1,5 ms (posição central). Ajuste entre 10 e
 * 20 para mover o servo.
 */
/** Example: 50 Hz PWM for servo control. */
void tim_example_servo_pwm(void) {
    gpio_config(GPIOB, 0, &(gpio_cfg_t){
        .mode = GPIO_MODE_AF,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_HIGH,
        .alternate = PB0_AF1_TIM3_CH3,
    });

    tim_init(TIM3, &(tim_cfg_t){
        .mode = TIM_COUNTER_UP,
        .prescaler = 4799u,
        .period = 199u,
        .preload = true,
    });
    tim_config_output(TIM3, 3u, &(tim_oc_cfg_t){
        .mode = TIM_OC_PWM1,
        .compare = 15u,
    });
    tim_enable(TIM3, true);
}

