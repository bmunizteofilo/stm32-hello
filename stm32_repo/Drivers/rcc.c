#include "rcc.h"

#ifndef STM32F0_FIRMWARE
RCC_TypeDef rcc_regs;
FLASH_TypeDef flash_regs;
#endif

static uint32_t sysclk_hz = 8000000u;

static void flash_latency_cfg(uint32_t hz) {
    if (hz > 24000000u) {
        FLASH->ACR = 1u; /* 1 wait state */
    } else {
        FLASH->ACR = 0u;
    }
}

bool rcc_sysclk_config(enum rcc_sysclk_cfg cfg) {
    switch (cfg) {
    case RCC_SYSCLK_HSI8:
        /* enable HSI and select as SYSCLK */
        RCC->CR |= (1u << 0); /* HSION */
        while ((RCC->CR & (1u << 1)) == 0u) {}
        RCC->CFGR &= ~(3u << 0);
        flash_latency_cfg(8000000u);
        RCC->CFGR = (RCC->CFGR & ~3u);
        sysclk_hz = 8000000u;
        return true;
    case RCC_SYSCLK_HSE8:
        RCC->CR |= (1u << 16); /* HSEON */
        while ((RCC->CR & (1u << 17)) == 0u) {}
        flash_latency_cfg(8000000u);
        RCC->CFGR = (RCC->CFGR & ~3u) | 1u; /* HSE as SYSCLK */
        sysclk_hz = 8000000u;
        return true;
    case RCC_SYSCLK_PLL_HSI_24MHZ:
        RCC->CR |= (1u << 0); /* HSION */
        while ((RCC->CR & (1u << 1)) == 0u) {}
        RCC->CFGR &= ~(1u << 16); /* PLL source HSI/2 */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (6u << 18); /* multiplier 6 */
        flash_latency_cfg(24000000u);
        RCC->CR |= (1u << 24); /* PLLON */
        while ((RCC->CR & (1u << 25)) == 0u) {}
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u; /* PLL as SYSCLK */
        sysclk_hz = 24000000u;
        return true;
    case RCC_SYSCLK_PLL_HSI_48MHZ:
        /* use HSI/2 * 12 = 48MHz */
        RCC->CR |= (1u << 0); /* HSION */
        while ((RCC->CR & (1u << 1)) == 0u) {}
        RCC->CFGR &= ~(3u << 0); /* HSI as base */
        RCC->CFGR |= (12u << 18); /* PLL multiplier 12 */
        RCC->CFGR &= ~(1u << 16); /* PLL source HSI/2 */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (12u << 18); /* multiplier 12 */
        flash_latency_cfg(48000000u);
        RCC->CR |= (1u << 24); /* PLLON */
        while ((RCC->CR & (1u << 25)) == 0u) {}
        RCC->CFGR |= (2u << 0); /* PLL as SYSCLK */
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u;
        sysclk_hz = 48000000u;
        return true;
    case RCC_SYSCLK_PLL_HSE_24MHZ:
        RCC->CR |= (1u << 16); /* HSEON */
        while ((RCC->CR & (1u << 17)) == 0u) {}
        RCC->CFGR |= (1u << 16); /* PLL source HSE */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (3u << 18); /* multiplier 3 */
        flash_latency_cfg(24000000u);
        RCC->CR |= (1u << 24);
        while ((RCC->CR & (1u << 25)) == 0u) {}
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u;
        sysclk_hz = 24000000u;
        return true;
    case RCC_SYSCLK_PLL_HSE_48MHZ:
        RCC->CR |= (1u << 16);
        while ((RCC->CR & (1u << 17)) == 0u) {}
        RCC->CFGR |= (1u << 16); /* PLL source HSE */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (6u << 18); /* multiplier 6 */
        flash_latency_cfg(48000000u);
        RCC->CR |= (1u << 24);
        while ((RCC->CR & (1u << 25)) == 0u) {}
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u;
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

uint32_t rcc_flash_latency_ws(void) {
    return FLASH->ACR & 0x7u;
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

void rcc_mco_enable_sysclk(void) {
    /* route system clock to MCO pin without prescaler */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_MCO_MASK) | RCC_CFGR_MCO_SYSCLK;
}
