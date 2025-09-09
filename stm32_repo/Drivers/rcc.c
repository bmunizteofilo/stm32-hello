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
        RCC->CR |= (1u << 0); /* HSION */
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 1)) == 0u) {}
#else
        RCC->CR |= (1u << 1);
#endif
        flash_latency_cfg(8000000u);
        RCC->CFGR = (RCC->CFGR & ~3u);
        sysclk_hz = 8000000u;
        return true;
    case RCC_SYSCLK_HSE8:
        RCC->CR |= (1u << 16); /* HSEON */
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 17)) == 0u) {}
#else
        RCC->CR |= (1u << 17);
#endif
        flash_latency_cfg(8000000u);
        RCC->CFGR = (RCC->CFGR & ~3u) | 1u; /* HSE as SYSCLK */
        sysclk_hz = 8000000u;
        return true;
    case RCC_SYSCLK_PLL_HSI_24MHZ:
        RCC->CR |= (1u << 0); /* HSION */
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 1)) == 0u) {}
#else
        RCC->CR |= (1u << 1);
#endif
        RCC->CFGR &= ~(1u << 16); /* PLL source HSI/2 */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (6u << 18); /* multiplier 6 */
        flash_latency_cfg(24000000u);
        RCC->CR |= (1u << 24); /* PLLON */
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 25)) == 0u) {}
#else
        RCC->CR |= (1u << 25);
#endif
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u; /* PLL as SYSCLK */
        sysclk_hz = 24000000u;
        return true;
    case RCC_SYSCLK_PLL_HSI_48MHZ:
        RCC->CR |= (1u << 0); /* HSION */
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 1)) == 0u) {}
#else
        RCC->CR |= (1u << 1);
#endif
        RCC->CFGR &= ~(1u << 16); /* PLL source HSI/2 */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (12u << 18); /* multiplier 12 */
        flash_latency_cfg(48000000u);
        RCC->CR |= (1u << 24); /* PLLON */
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 25)) == 0u) {}
#else
        RCC->CR |= (1u << 25);
#endif
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u;
        sysclk_hz = 48000000u;
        return true;
    case RCC_SYSCLK_PLL_HSE_24MHZ:
        RCC->CR |= (1u << 16); /* HSEON */
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 17)) == 0u) {}
#else
        RCC->CR |= (1u << 17);
#endif
        RCC->CFGR |= (1u << 16); /* PLL source HSE */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (3u << 18); /* multiplier 3 */
        flash_latency_cfg(24000000u);
        RCC->CR |= (1u << 24);
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 25)) == 0u) {}
#else
        RCC->CR |= (1u << 25);
#endif
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u;
        sysclk_hz = 24000000u;
        return true;
    case RCC_SYSCLK_PLL_HSE_48MHZ:
        RCC->CR |= (1u << 16);
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 17)) == 0u) {}
#else
        RCC->CR |= (1u << 17);
#endif
        RCC->CFGR |= (1u << 16); /* PLL source HSE */
        RCC->CFGR &= ~(0xFu << 18);
        RCC->CFGR |= (6u << 18); /* multiplier 6 */
        flash_latency_cfg(48000000u);
        RCC->CR |= (1u << 24);
#ifdef STM32F0_FIRMWARE
        while ((RCC->CR & (1u << 25)) == 0u) {}
#else
        RCC->CR |= (1u << 25);
#endif
        RCC->CFGR = (RCC->CFGR & ~3u) | 2u;
        sysclk_hz = 48000000u;
        return true;
    default:
        return false;
    }
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

