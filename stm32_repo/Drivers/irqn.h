#ifndef IRQN_H
#define IRQN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* IRQ numbers for STM32F0 series (e.g. STM32F070). */
typedef enum {
    /* Cortex-M0 exceptions */
    NonMaskableInt_IRQn = -14,
    HardFault_IRQn      = -13,
    SVCall_IRQn         = -5,
    PendSV_IRQn         = -2,
    SysTick_IRQn        = -1,

    /* STM32F0 specific interrupt numbers */
    WWDG_IRQn           = 0,
    PVD_VDDIO2_IRQn     = 1,
    RTC_IRQn            = 2,
    FLASH_IRQn          = 3,
    RCC_IRQn            = 4,
    EXTI0_1_IRQn        = 5,
    EXTI2_3_IRQn        = 6,
    EXTI4_15_IRQn       = 7,
    TSC_IRQn            = 8,
    DMA1_Channel1_IRQn  = 9,
    DMA1_Channel2_3_IRQn = 10,
    DMA1_Channel4_5_IRQn = 11,
    ADC1_IRQn           = 12,
    TIM1_BRK_UP_TRG_COM_IRQn = 13,
    TIM1_CC_IRQn        = 14,
    TIM2_IRQn           = 15,
    TIM3_IRQn           = 16,
    TIM6_DAC_IRQn       = 17,
    TIM7_IRQn           = 18,
    TIM14_IRQn          = 19,
    TIM15_IRQn          = 20,
    TIM16_IRQn          = 21,
    TIM17_IRQn          = 22,
    I2C1_IRQn           = 23,
    I2C2_IRQn           = 24,
    SPI1_IRQn           = 25,
    SPI2_IRQn           = 26,
    USART1_IRQn         = 27,
    USART2_IRQn         = 28,
    USART3_4_IRQn       = 29,
    CEC_CAN_IRQn        = 30,
    USB_IRQn            = 31
} IRQn_Type;

#endif /* IRQN_H */
