#ifndef RCC_H
#define RCC_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t AHBRSTR;
    volatile uint32_t CFGR2;
    volatile uint32_t CFGR3;
    volatile uint32_t CR2;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t ACR;
} FLASH_TypeDef;

#define RCC_BASE   0x40021000u
#define FLASH_BASE 0x40022000u
#ifdef STM32F0_FIRMWARE
#define RCC   ((RCC_TypeDef *)RCC_BASE)
#define FLASH ((FLASH_TypeDef *)FLASH_BASE)
#else
extern RCC_TypeDef rcc_regs;
extern FLASH_TypeDef flash_regs;
#define RCC   (&rcc_regs)
#define FLASH (&flash_regs)
#endif


/* AHBENR bits */
#define RCC_AHBENR_DMA1   (1u << 0)
#define RCC_AHBENR_GPIOA  (1u << 17)
#define RCC_AHBENR_GPIOB  (1u << 18)
#define RCC_AHBENR_GPIOC  (1u << 19)
#define RCC_AHBENR_GPIOD  (1u << 20)
#define RCC_AHBENR_GPIOE  (1u << 21)
#define RCC_AHBENR_GPIOF  (1u << 22)

/* APB2ENR bits */
#define RCC_APB2ENR_SYSCFG (1u << 0)
#define RCC_APB2ENR_ADC1   (1u << 9)
#define RCC_APB2ENR_TIM1   (1u << 11)
#define RCC_APB2ENR_SPI1   (1u << 12)
#define RCC_APB2ENR_USART1 (1u << 14)
#define RCC_APB2ENR_TIM15  (1u << 16)
#define RCC_APB2ENR_TIM16  (1u << 17)
#define RCC_APB2ENR_TIM17  (1u << 18)

/* APB1ENR bits */
#define RCC_APB1ENR_TIM2   (1u << 0)
#define RCC_APB1ENR_TIM3   (1u << 1)
#define RCC_APB1ENR_TIM6   (1u << 4)
#define RCC_APB1ENR_TIM7   (1u << 5)
#define RCC_APB1ENR_TIM14  (1u << 8)
#define RCC_APB1ENR_SPI2   (1u << 14)
#define RCC_APB1ENR_USART2 (1u << 17)
#define RCC_APB1ENR_USART3 (1u << 18)
#define RCC_APB1ENR_USART4 (1u << 19)
#define RCC_APB1ENR_I2C1   (1u << 21)
#define RCC_APB1ENR_I2C2   (1u << 22)

/* Predefined system clock configurations */
enum rcc_sysclk_cfg {
    RCC_SYSCLK_HSI8,
    RCC_SYSCLK_HSE8,
    RCC_SYSCLK_PLL_HSI_24MHZ,
    RCC_SYSCLK_PLL_HSI_48MHZ,
    RCC_SYSCLK_PLL_HSE_24MHZ,
    RCC_SYSCLK_PLL_HSE_48MHZ,
};

/* MCO (Microcontroller Clock Output) configuration */
#define RCC_CFGR_MCO_SHIFT   24u
#define RCC_CFGR_MCO_MASK    (7u << RCC_CFGR_MCO_SHIFT)
#define RCC_CFGR_MCO_SYSCLK  (4u << RCC_CFGR_MCO_SHIFT)

/** Configure system clock using predefined configuration. */
bool rcc_sysclk_config(enum rcc_sysclk_cfg cfg);
/** Configure system clock based on external HSE frequency. */
bool rcc_sysclk_config_hse(uint32_t hse_hz, uint32_t sysclk_hz);
/** Get current system clock frequency. */
uint32_t rcc_sysclk_hz(void);
/** Get flash latency wait states. */
uint32_t rcc_flash_latency_ws(void);

/** Enable AHB peripheral clocks using mask. */
void rcc_ahb_enable(uint32_t mask);
/** Enable APB1 peripheral clocks using mask. */
void rcc_apb1_enable(uint32_t mask);
/** Enable APB2 peripheral clocks using mask. */
void rcc_apb2_enable(uint32_t mask);

/** Output system clock on MCO pin. */
void rcc_mco_enable_sysclk(void);

#endif /* RCC_H */
