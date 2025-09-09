#ifndef TIM_H
#define TIM_H

#include <stdint.h>
#include <stdbool.h>

typedef struct TIM_TypeDef TIM_TypeDef;

#define TIM1  ((TIM_TypeDef *)0x40012C00u)
#define TIM2  ((TIM_TypeDef *)0x40000000u)
#define TIM3  ((TIM_TypeDef *)0x40000400u)
#define TIM14 ((TIM_TypeDef *)0x40002000u)
#define TIM15 ((TIM_TypeDef *)0x40014000u)
#define TIM16 ((TIM_TypeDef *)0x40014400u)
#define TIM17 ((TIM_TypeDef *)0x40014800u)

#define TIM_CR1_CEN   (1u << 0)
#define TIM_CR1_DIR   (1u << 4)
#define TIM_CR1_ARPE  (1u << 7)

#define TIM_DIER_UIE   (1u << 0)
#define TIM_DIER_CC1IE (1u << 1)
#define TIM_DIER_CC2IE (1u << 2)
#define TIM_DIER_CC3IE (1u << 3)
#define TIM_DIER_CC4IE (1u << 4)
#define TIM_DIER_UDE   (1u << 8)
#define TIM_DIER_CC1DE (1u << 9)
#define TIM_DIER_CC2DE (1u << 10)
#define TIM_DIER_CC3DE (1u << 11)
#define TIM_DIER_CC4DE (1u << 12)

#define TIM_SR_UIF   (1u << 0)
#define TIM_SR_CC1IF (1u << 1)
#define TIM_SR_CC2IF (1u << 2)
#define TIM_SR_CC3IF (1u << 3)
#define TIM_SR_CC4IF (1u << 4)

enum tim_counter_mode {
    TIM_COUNTER_UP,
    TIM_COUNTER_DOWN,
};

enum tim_oc_mode {
    TIM_OC_FROZEN = 0,
    TIM_OC_ACTIVE = 1,
    TIM_OC_INACTIVE = 2,
    TIM_OC_TOGGLE = 3,
    TIM_OC_FORCE_ACTIVE = 5,
    TIM_OC_FORCE_INACTIVE = 4,
    TIM_OC_PWM1 = 6,
    TIM_OC_PWM2 = 7,
};

enum tim_ic_pol {
    TIM_IC_RISING,
    TIM_IC_FALLING,
    TIM_IC_BOTH,
};

typedef struct {
    enum tim_counter_mode mode;
    uint16_t prescaler;
    uint16_t period;
    bool preload;
} tim_cfg_t;

typedef struct {
    enum tim_oc_mode mode;
    uint32_t compare;
    bool enable_irq;
} tim_oc_cfg_t;

typedef struct {
    enum tim_ic_pol polarity;
    bool enable_irq;
} tim_ic_cfg_t;

typedef void (*tim_cb_t)(void *ctx);

/** Initialize a timer with basic configuration. */
bool tim_init(TIM_TypeDef *tim, const tim_cfg_t *cfg);
/** Enable or disable a timer. */
void tim_enable(TIM_TypeDef *tim, bool en);

/** Configure an output compare channel. */
bool tim_config_output(TIM_TypeDef *tim, uint8_t ch, const tim_oc_cfg_t *cfg);
/** Update compare value for an output channel. */
void tim_set_compare(TIM_TypeDef *tim, uint8_t ch, uint32_t compare);

/** Configure an input capture channel. */
bool tim_config_input(TIM_TypeDef *tim, uint8_t ch, const tim_ic_cfg_t *cfg);
/** Read captured value from a channel. */
uint32_t tim_get_capture(TIM_TypeDef *tim, uint8_t ch);

/** Enable or disable DMA requests for selected events. */
void tim_enable_dma(TIM_TypeDef *tim, uint32_t mask, bool en);

/** Register callback for update interrupt. */
void tim_attach_update_irq(TIM_TypeDef *tim, tim_cb_t cb, void *ctx);
/** Register callback for capture/compare interrupt. */
void tim_attach_cc_irq(TIM_TypeDef *tim, uint8_t ch, tim_cb_t cb, void *ctx);
/** Detach update interrupt callback. */
void tim_detach_update_irq(TIM_TypeDef *tim);
/** Detach capture/compare interrupt callback. */
void tim_detach_cc_irq(TIM_TypeDef *tim, uint8_t ch);

extern volatile uint32_t tim_ic_last_period;

/* Exemplo: gera PWM no TIM3 canal 1 (PA6) */
/** Example: generate PWM on TIM3 channel 1. */
void tim_example_pwm(void);

/** Example: input capture on TIM3 channel 2. */
void tim_example_input_capture(void);

/** Example: periodic update interrupt (blink LED). */
void tim_example_update_irq(void);

/** Example: servo control using PWM on TIM3 channel 3. */
void tim_example_servo_pwm(void);

#endif /* TIM_H */

