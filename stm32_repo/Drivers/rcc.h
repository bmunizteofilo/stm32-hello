#ifndef RCC_H
#define RCC_H

#include <stdint.h>
#include <stdbool.h>

#define RCC_BASE 0x40021000u

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

#define RCC ((RCC_TypeDef *)RCC_BASE)

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
#define RCC_APB2ENR_SPI1   (1u << 12)
#define RCC_APB2ENR_USART1 (1u << 14)

/* APB1ENR bits */
#define RCC_APB1ENR_TIM3   (1u << 1)
#define RCC_APB1ENR_SPI2   (1u << 14)
#define RCC_APB1ENR_USART2 (1u << 17)
#define RCC_APB1ENR_I2C1   (1u << 21)
#define RCC_APB1ENR_I2C2   (1u << 22)

/* Predefined system clock configurations */
enum rcc_sysclk_cfg {
    RCC_SYSCLK_HSI8,
    RCC_SYSCLK_PLL_HSI_48MHZ,
};

bool rcc_sysclk_config(enum rcc_sysclk_cfg cfg);
uint32_t rcc_sysclk_hz(void);

void rcc_ahb_enable(uint32_t mask);
void rcc_apb1_enable(uint32_t mask);
void rcc_apb2_enable(uint32_t mask);

#endif /* RCC_H */
