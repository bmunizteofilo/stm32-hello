#ifndef CM0_H
#define CM0_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "irqn.h"

/** Enable global interrupts. */
void cm0_enable_irq_global(void);
/** Disable global interrupts. */
void cm0_disable_irq_global(void);
/** Enter low-power wait-for-interrupt state. */
void cm0_wfi(void);

/** Enable a specific IRQ in the NVIC. */
void cm0_nvic_enable(IRQn_Type irq);
/** Disable a specific IRQ in the NVIC. */
void cm0_nvic_disable(IRQn_Type irq);
/** Set a pending flag for an IRQ. */
void cm0_nvic_set_pending(IRQn_Type irq);
/** Clear the pending flag of an IRQ. */
void cm0_nvic_clear_pending(IRQn_Type irq);
/** Set priority for an IRQ (0=highest, 3=lowest). */
void cm0_nvic_set_priority(IRQn_Type irq, uint8_t prio);
/** Configure the SysTick timer with the provided tick count. */
bool cm0_systick_config(uint32_t ticks);

/* Example usage:
 *   cm0_disable_irq_global();
 *   cm0_nvic_enable(EXTI0_1_IRQn); // when building for firmware
 */

#endif /* CM0_H */
