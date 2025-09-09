#include "cm0.h"

/* Implementações vazias para compilação em ambiente de testes (host). */

void cm0_enable_irq_global(void) {}
void cm0_disable_irq_global(void) {}
void cm0_wfi(void) {}

void cm0_nvic_enable(IRQn_Type irq) { (void)irq; }
void cm0_nvic_disable(IRQn_Type irq) { (void)irq; }
void cm0_nvic_set_pending(IRQn_Type irq) { (void)irq; }
void cm0_nvic_clear_pending(IRQn_Type irq) { (void)irq; }
void cm0_nvic_set_priority(IRQn_Type irq, uint8_t prio) { (void)irq; (void)prio; }

bool cm0_systick_config(uint32_t ticks) { (void)ticks; return true; }

