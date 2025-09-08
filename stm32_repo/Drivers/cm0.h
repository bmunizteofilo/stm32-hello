#ifndef CM0_H
#define CM0_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration for host builds without CMSIS headers */
typedef int32_t IRQn_Type;

void cm0_enable_irq_global(void);
void cm0_disable_irq_global(void);
void cm0_wfi(void);

void cm0_nvic_enable(IRQn_Type irq);
void cm0_nvic_disable(IRQn_Type irq);
void cm0_nvic_set_pending(IRQn_Type irq);
void cm0_nvic_clear_pending(IRQn_Type irq);
void cm0_nvic_set_priority(IRQn_Type irq, uint8_t prio);
bool cm0_systick_config(uint32_t ticks);

/* Example usage:
 *   cm0_disable_irq_global();
 *   cm0_nvic_enable(EXTI0_1_IRQn); // when building for firmware
 */

#endif /* CM0_H */
