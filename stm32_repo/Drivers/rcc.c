#include "rcc.h"

#if defined(STM32F0_FIRMWARE)
static uint32_t sysclk_hz = 8000000u;

bool rcc_sysclk_config(enum rcc_sysclk_cfg cfg) {
    switch (cfg) {
    case RCC_SYSCLK_HSI8:
        /* enable HSI and select as SYSCLK */
        RCC->CR |= (1u << 0); /* HSION */
        while ((RCC->CR & (1u << 1)) == 0u) {}
        RCC->CFGR &= ~(3u << 0);
        sysclk_hz = 8000000u;
        return true;
    case RCC_SYSCLK_PLL_HSI_48MHZ:
        /* use HSI/2 * 12 = 48MHz */
        RCC->CR |= (1u << 0); /* HSION */
        while ((RCC->CR & (1u << 1)) == 0u) {}
        RCC->CFGR &= ~(3u << 0); /* HSI as base */
        RCC->CFGR |= (12u << 18); /* PLL multiplier 12 */
        RCC->CR |= (1u << 24); /* PLLON */
        while ((RCC->CR & (1u << 25)) == 0u) {}
        RCC->CFGR |= (2u << 0); /* PLL as SYSCLK */
        sysclk_hz = 48000000u;
        return true;
    default:
        return false;
    }
}

bool rcc_sysclk_config_hse(uint32_t hse_hz, uint32_t desired_sysclk) {
    if (hse_hz == 0u || desired_sysclk == 0u) {
        return false;
    }

    /* Enable external oscillator */
    RCC->CR |= (1u << 16); /* HSEON */
    while ((RCC->CR & (1u << 17)) == 0u) {}

    if (desired_sysclk == hse_hz) {
        /* use HSE directly */
        RCC->CFGR &= ~(3u << 0);
        RCC->CFGR |= (1u << 0); /* HSE as SYSCLK */
        sysclk_hz = hse_hz;
        return true;
    }

    uint32_t mul = desired_sysclk / hse_hz;
    if ((desired_sysclk % hse_hz) != 0u || mul < 2u || mul > 16u) {
        return false;
    }

    /* configure PLL source as HSE and multiplier */
    RCC->CFGR &= ~((1u << 16) | (15u << 18));
    RCC->CFGR |= (1u << 16); /* PLLSRC = HSE */
    RCC->CFGR |= ((mul - 2u) << 18); /* PLLMUL */
    RCC->CR |= (1u << 24); /* PLLON */
    while ((RCC->CR & (1u << 25)) == 0u) {}
    RCC->CFGR &= ~(3u << 0);
    RCC->CFGR |= (2u << 0); /* PLL as SYSCLK */
    sysclk_hz = desired_sysclk;
    return true;
}

uint32_t rcc_sysclk_hz(void) {
    return sysclk_hz;
}

void rcc_ahb_enable(uint32_t mask) {
    RCC->AHBENR |= mask;
}

void rcc_apb1_enable(uint32_t mask) {
    RCC->APB1ENR |= mask;
}

void rcc_apb2_enable(uint32_t mask) {
    RCC->APB2ENR |= mask;
}
#else
static uint32_t sysclk_hz = 8000000u;

bool rcc_sysclk_config(enum rcc_sysclk_cfg cfg) {
    (void)cfg; return true;
}

bool rcc_sysclk_config_hse(uint32_t hse_hz, uint32_t desired_sysclk) {
    (void)hse_hz; sysclk_hz = desired_sysclk; return true;
}

uint32_t rcc_sysclk_hz(void) { return sysclk_hz; }

void rcc_ahb_enable(uint32_t mask) { (void)mask; }
void rcc_apb1_enable(uint32_t mask) { (void)mask; }
void rcc_apb2_enable(uint32_t mask) { (void)mask; }
#endif
