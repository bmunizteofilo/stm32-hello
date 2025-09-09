/*
 * Minimal Cortex-M0 utilities.
 * Define -DSTM32F0_FIRMWARE for real implementation; otherwise stubs are built.
 */
#include "cm0.h"

/* Direct register addresses for Cortex-M0 core peripherals */
#define NVIC_ISER    (*(volatile uint32_t *)0xE000E100u)
#define NVIC_ICER    (*(volatile uint32_t *)0xE000E180u)
#define NVIC_ISPR    (*(volatile uint32_t *)0xE000E200u)
#define NVIC_ICPR    (*(volatile uint32_t *)0xE000E280u)
#define NVIC_IPR     ((volatile uint8_t  *)0xE000E400u)

#define SYST_CSR     (*(volatile uint32_t *)0xE000E010u)
#define SYST_RVR     (*(volatile uint32_t *)0xE000E014u)
#define SYST_CVR     (*(volatile uint32_t *)0xE000E018u)

#define SYST_CSR_ENABLE    (1u << 0)
#define SYST_CSR_TICKINT   (1u << 1)
#define SYST_CSR_CLKSOURCE (1u << 2)

/** Enable global interrupts. */
void cm0_enable_irq_global(void) { __asm volatile ("cpsie i"); }
/** Disable global interrupts. */
void cm0_disable_irq_global(void) { __asm volatile ("cpsid i"); }
/** Enter wait-for-interrupt state. */
void cm0_wfi(void) { __asm volatile ("wfi"); }

/** Enable an IRQ in the NVIC. */
void cm0_nvic_enable(IRQn_Type irq) {
    NVIC_ISER = (uint32_t)1u << ((uint32_t)irq & 0x1Fu);
}

/** Disable an IRQ in the NVIC. */
void cm0_nvic_disable(IRQn_Type irq) {
    NVIC_ICER = (uint32_t)1u << ((uint32_t)irq & 0x1Fu);
}

/** Set an IRQ as pending. */
void cm0_nvic_set_pending(IRQn_Type irq) {
    NVIC_ISPR = (uint32_t)1u << ((uint32_t)irq & 0x1Fu);
}

/** Clear the pending flag of an IRQ. */
void cm0_nvic_clear_pending(IRQn_Type irq) {
    NVIC_ICPR = (uint32_t)1u << ((uint32_t)irq & 0x1Fu);
}

/** Set priority for an IRQ (0=highest, 3=lowest). */
void cm0_nvic_set_priority(IRQn_Type irq, uint8_t prio) {
    NVIC_IPR[(uint32_t)irq & 0x1Fu] = (uint8_t)((prio & 0x3u) << 6);
}

/**
 * @brief Configure the SysTick timer.
 * @param ticks Reload value for the timer.
 * @return true on success, false if ticks is out of range.
 */
bool cm0_systick_config(uint32_t ticks) {
    if (ticks == 0u || ticks > 0xFFFFFFu) {
        return false;
    }
    SYST_RVR = ticks - 1u;
    SYST_CVR = 0u;
    SYST_CSR = SYST_CSR_ENABLE | SYST_CSR_TICKINT | SYST_CSR_CLKSOURCE;
    return true;
}

