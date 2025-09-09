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
bool rcc_sysclk_config(enum rcc_sysclk_cfg cfg) {
    (void)cfg; return true;
}
uint32_t rcc_sysclk_hz(void) { return 8000000u; }
void rcc_ahb_enable(uint32_t mask) { (void)mask; }
void rcc_apb1_enable(uint32_t mask) { (void)mask; }
void rcc_apb2_enable(uint32_t mask) { (void)mask; }
#endif
