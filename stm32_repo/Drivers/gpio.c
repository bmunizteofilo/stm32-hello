/*
 * GPIO driver for STM32F0 without HAL.
 * Define -DSTM32F0_FIRMWARE for real implementation; otherwise stubs are built.
 */
#include "gpio.h"
#include "rcc.h"
#include "cm0.h"

#if defined(STM32F0_FIRMWARE)
struct GPIO_TypeDef {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
};

typedef struct {
    volatile uint32_t CFGR1;
    volatile uint32_t RCR;
    volatile uint32_t EXTICR[4];
} SYSCFG_TypeDef;

typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;

#define SYSCFG ((SYSCFG_TypeDef *)0x40010000u)
#define EXTI   ((EXTI_TypeDef *)0x40010400u)

/* Callback table for EXTI lines 0..15 */
typedef struct {
    gpio_cb_t cb;
    void *ctx;
} gpio_irq_entry_t;

static gpio_irq_entry_t gpio_irq_table[16];

static uint8_t gpio_port_index(GPIO_TypeDef *port) {
    if (port == GPIOA) return 0;
    if (port == GPIOB) return 1;
    if (port == GPIOC) return 2;
    if (port == GPIOD) return 3;
    if (port == GPIOE) return 4;
    if (port == GPIOF) return 5;
    return 0xFF;
}

static uint32_t gpio_rcc_mask(GPIO_TypeDef *port) {
    uint8_t idx = gpio_port_index(port);
    if (idx > 5) {
        return 0;
    }
    return 1u << (17U + idx);
}

void gpio_config(GPIO_TypeDef *port, uint8_t pin, const gpio_cfg_t *cfg) {
    uint32_t bit = 1u << pin;
    uint32_t shift = (uint32_t)pin * 2u;

    rcc_ahb_enable(gpio_rcc_mask(port));

    port->MODER = (port->MODER & ~(3u << shift)) | ((uint32_t)cfg->mode << shift);
    port->PUPDR = (port->PUPDR & ~(3u << shift)) | ((uint32_t)cfg->pull << shift);

    if (cfg->mode == GPIO_MODE_OUTPUT || cfg->mode == GPIO_MODE_AF) {
        if (cfg->otype == GPIO_OTYPE_OD) {
            port->OTYPER |= bit;
        } else {
            port->OTYPER &= ~bit;
        }
        port->OSPEEDR = (port->OSPEEDR & ~(3u << shift)) | ((uint32_t)cfg->speed << shift);
    }

    if (cfg->mode == GPIO_MODE_AF) {
        uint32_t afr_idx = pin / 8u;
        uint32_t afr_shift = (pin % 8u) * 4u;
        port->AFR[afr_idx] = (port->AFR[afr_idx] & ~(0xFu << afr_shift)) |
                             (((uint32_t)cfg->alternate & 0xFu) << afr_shift);
    }
}

void gpio_deinit(GPIO_TypeDef *port, uint8_t pin) {
    uint32_t shift = (uint32_t)pin * 2u;
    uint32_t bit = 1u << pin;
    port->MODER &= ~(3u << shift);
    port->PUPDR &= ~(3u << shift);
    port->OTYPER &= ~bit;
    port->OSPEEDR &= ~(3u << shift);
    uint32_t afr_idx = pin / 8u;
    uint32_t afr_shift = (pin % 8u) * 4u;
    port->AFR[afr_idx] &= ~(0xFu << afr_shift);
}

void gpio_write(GPIO_TypeDef *port, uint8_t pin, uint8_t level) {
    uint32_t bit = 1u << pin;
    if (level) {
        port->BSRR = bit;
    } else {
        port->BRR = bit;
    }
}

void gpio_toggle(GPIO_TypeDef *port, uint8_t pin) {
    port->ODR ^= 1u << pin;
}

uint8_t gpio_read(GPIO_TypeDef *port, uint8_t pin) {
    return (uint8_t)((port->IDR >> pin) & 1u);
}

bool gpio_attach_irq(GPIO_TypeDef *port, uint8_t pin, enum gpio_edge_t edge,
                     gpio_cb_t cb, void *ctx) {
    if (pin > 15) {
        return false;
    }
    uint8_t port_idx = gpio_port_index(port);
    if (port_idx == 0xFF) {
        return false;
    }

    /* Enable clocks for GPIO port and SYSCFG */
    rcc_ahb_enable(gpio_rcc_mask(port));
    rcc_apb2_enable(RCC_APB2ENR_SYSCFG);

    /* Configure EXTI source */
    uint32_t idx = pin / 4u;
    uint32_t shift = (pin % 4u) * 4u;
    SYSCFG->EXTICR[idx] &= ~(0xFu << shift);
    SYSCFG->EXTICR[idx] |= ((uint32_t)port_idx << shift);

    uint32_t bit = 1u << pin;
    switch (edge) {
    case GPIO_EDGE_RISING:
        EXTI->RTSR |= bit;
        EXTI->FTSR &= ~bit;
        break;
    case GPIO_EDGE_FALLING:
        EXTI->FTSR |= bit;
        EXTI->RTSR &= ~bit;
        break;
    case GPIO_EDGE_BOTH:
        EXTI->RTSR |= bit;
        EXTI->FTSR |= bit;
        break;
    case GPIO_EDGE_NONE:
    default:
        EXTI->IMR &= ~bit;
        return true;
    }

    EXTI->IMR |= bit;
    EXTI->PR = bit; /* clear pending */

    gpio_irq_table[pin].cb = cb;
    gpio_irq_table[pin].ctx = ctx;

    if (pin <= 1u) {
        cm0_nvic_enable(EXTI0_1_IRQn);
    } else if (pin <= 3u) {
        cm0_nvic_enable(EXTI2_3_IRQn);
    } else {
        cm0_nvic_enable(EXTI4_15_IRQn);
    }
    return true;
}

void gpio_detach_irq(GPIO_TypeDef *port, uint8_t pin) {
    (void)port;
    if (pin > 15) {
        return;
    }
    uint32_t bit = 1u << pin;
    EXTI->IMR &= ~bit;
    EXTI->RTSR &= ~bit;
    EXTI->FTSR &= ~bit;
    EXTI->PR = bit;
    gpio_irq_table[pin].cb = NULL;
    gpio_irq_table[pin].ctx = NULL;

    bool active = false;
    if (pin <= 1u) {
        active = gpio_irq_table[pin ^ 1u].cb != NULL;
        if (!active) cm0_nvic_disable(EXTI0_1_IRQn);
    } else if (pin <= 3u) {
        active = gpio_irq_table[(pin ^ 1u)].cb != NULL;
        if (!active) cm0_nvic_disable(EXTI2_3_IRQn);
    } else {
        for (uint8_t i = 4u; i < 16u; ++i) {
            if (gpio_irq_table[i].cb) {
                active = true;
                break;
            }
        }
        if (!active) cm0_nvic_disable(EXTI4_15_IRQn);
    }
}

static void gpio_dispatch(uint32_t pending, uint8_t base) {
    for (uint8_t i = 0; i < 2 && (base + i) < 16; ++i) {
        if (pending & (1u << (base + i))) {
            gpio_irq_entry_t *e = &gpio_irq_table[base + i];
            if (e->cb) {
                e->cb(e->ctx, (uint8_t)(base + i));
            }
        }
    }
}

void EXTI0_1_IRQHandler(void) {
    uint32_t pending = EXTI->PR & 0x3u;
    EXTI->PR = pending;
    gpio_dispatch(pending, 0);
}

void EXTI2_3_IRQHandler(void) {
    uint32_t pending = EXTI->PR & 0xCu;
    EXTI->PR = pending;
    gpio_dispatch(pending, 2);
}

void EXTI4_15_IRQHandler(void) {
    uint32_t pending = EXTI->PR & 0xFFF0u;
    EXTI->PR = pending;
    for (uint8_t i = 4u; i < 16u; ++i) {
        if (pending & (1u << i)) {
            gpio_irq_entry_t *e = &gpio_irq_table[i];
            if (e->cb) {
                e->cb(e->ctx, i);
            }
        }
    }
}

/* Example: configure PC8 push-pull and PC9 open-drain */
void gpio_example_output(void) {
    gpio_config(GPIOC, 8, &(gpio_cfg_t){
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_PP,
        .speed = GPIO_SPEED_HIGH,
    });
    gpio_write(GPIOC, 8, 1);

    gpio_config(GPIOC, 9, &(gpio_cfg_t){
        .mode = GPIO_MODE_OUTPUT,
        .otype = GPIO_OTYPE_OD,
        .pull = GPIO_PULL_UP,
        .speed = GPIO_SPEED_HIGH,
    });
    gpio_write(GPIOC, 9, 0);
}

/* Example: configure PA0 as input with pull-down */
void gpio_example_input(void) {
    gpio_config(GPIOA, 0, &(gpio_cfg_t){
        .mode = GPIO_MODE_INPUT,
        .pull = GPIO_PULL_DOWN,
    });
}

/* Example callback toggles PC9 on PA1 interrupt */
static void gpio_example_cb(void *ctx, uint8_t pin) {
    (void)ctx; (void)pin;
    gpio_toggle(GPIOC, 9);
}

/* Example: attach rising-edge IRQ on PA1 */
void gpio_example_irq(void) {
    gpio_config(GPIOA, 1, &(gpio_cfg_t){
        .mode = GPIO_MODE_INPUT,
        .pull = GPIO_PULL_UP,
    });
    gpio_attach_irq(GPIOA, 1, GPIO_EDGE_RISING, gpio_example_cb, NULL);
}

/* Example: toggle PC8 */
void gpio_example_toggle(void) {
    gpio_toggle(GPIOC, 8);
}

#else /* STM32F0_FIRMWARE */

void gpio_config(GPIO_TypeDef *port, uint8_t pin, const gpio_cfg_t *cfg) {
    (void)port; (void)pin; (void)cfg;
}
void gpio_deinit(GPIO_TypeDef *port, uint8_t pin) { (void)port; (void)pin; }
void gpio_write(GPIO_TypeDef *port, uint8_t pin, uint8_t level) {
    (void)port; (void)pin; (void)level;
}
void gpio_toggle(GPIO_TypeDef *port, uint8_t pin) { (void)port; (void)pin; }
uint8_t gpio_read(GPIO_TypeDef *port, uint8_t pin) {
    (void)port; (void)pin; return 0u;
}
bool gpio_attach_irq(GPIO_TypeDef *port, uint8_t pin, enum gpio_edge_t edge,
                     gpio_cb_t cb, void *ctx) {
    (void)port; (void)pin; (void)edge; (void)cb; (void)ctx; return false;
}
void gpio_detach_irq(GPIO_TypeDef *port, uint8_t pin) {
    (void)port; (void)pin;
}
void EXTI0_1_IRQHandler(void) {}
void EXTI2_3_IRQHandler(void) {}
void EXTI4_15_IRQHandler(void) {}

void gpio_example_output(void) {}
void gpio_example_input(void) {}
void gpio_example_irq(void) {}
void gpio_example_toggle(void) {}

#endif /* STM32F0_FIRMWARE */
