#include "cm0.h"

/* Empty implementations for host testing environment. */

/** Stub: enable global interrupts. */
void cm0_enable_irq_global(void) {}
/** Stub: disable global interrupts. */
void cm0_disable_irq_global(void) {}
/** Stub: wait for interrupt. */
void cm0_wfi(void) {}

/** Stub: enable NVIC interrupt. */
void cm0_nvic_enable(IRQn_Type irq) { (void)irq; }
/** Stub: disable NVIC interrupt. */
void cm0_nvic_disable(IRQn_Type irq) { (void)irq; }
/** Stub: set pending IRQ. */
void cm0_nvic_set_pending(IRQn_Type irq) { (void)irq; }
/** Stub: clear pending IRQ. */
void cm0_nvic_clear_pending(IRQn_Type irq) { (void)irq; }
/** Stub: set interrupt priority. */
void cm0_nvic_set_priority(IRQn_Type irq, uint8_t prio) { (void)irq; (void)prio; }

/** Stub: configure SysTick timer. */
bool cm0_systick_config(uint32_t ticks) { (void)ticks; return true; }
