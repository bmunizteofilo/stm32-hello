/*
 * Minimal Cortex-M0 utilities.
 * Define -DSTM32F0_FIRMWARE for real implementation; otherwise stubs are built.
 */
#include "cm0.h"

#if defined(STM32F0_FIRMWARE)
#include "stm32f0xx.h"

void cm0_enable_irq_global(void) { __enable_irq(); }
void cm0_disable_irq_global(void) { __disable_irq(); }
void cm0_wfi(void) { __WFI(); }

void cm0_nvic_enable(IRQn_Type irq) { NVIC_EnableIRQ(irq); }
void cm0_nvic_disable(IRQn_Type irq) { NVIC_DisableIRQ(irq); }
void cm0_nvic_set_pending(IRQn_Type irq) { NVIC_SetPendingIRQ(irq); }
void cm0_nvic_clear_pending(IRQn_Type irq) { NVIC_ClearPendingIRQ(irq); }
void cm0_nvic_set_priority(IRQn_Type irq, uint8_t prio) {
    NVIC_SetPriority(irq, prio);
}
bool cm0_systick_config(uint32_t ticks) {
    return SysTick_Config(ticks) == 0U;
}

#else /* STM32F0_FIRMWARE */

void cm0_enable_irq_global(void) {}
void cm0_disable_irq_global(void) {}
void cm0_wfi(void) {}

void cm0_nvic_enable(IRQn_Type irq) { (void)irq; }
void cm0_nvic_disable(IRQn_Type irq) { (void)irq; }
void cm0_nvic_set_pending(IRQn_Type irq) { (void)irq; }
void cm0_nvic_clear_pending(IRQn_Type irq) { (void)irq; }
void cm0_nvic_set_priority(IRQn_Type irq, uint8_t prio) {
    (void)irq;
    (void)prio;
}
bool cm0_systick_config(uint32_t ticks) {
    (void)ticks;
    return false;
}

#endif /* STM32F0_FIRMWARE */
