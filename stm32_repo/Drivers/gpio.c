#include "gpio.h"

#if defined(_MSC_VER)
#define WEAK_SYMBOL __declspec(selectany)
#else
#define WEAK_SYMBOL __attribute__((weak))
#endif

/* Endereços base reais; nos testes os símbolos fortes substituem estes. */
WEAK_SYMBOL RCC_TypeDef *RCC   = (RCC_TypeDef *)0x40021000u;
WEAK_SYMBOL GPIO_TypeDef *GPIOA = (GPIO_TypeDef *)0x48000000u;
WEAK_SYMBOL GPIO_TypeDef *GPIOB = (GPIO_TypeDef *)0x48000400u;
WEAK_SYMBOL GPIO_TypeDef *GPIOC = (GPIO_TypeDef *)0x48000800u;
WEAK_SYMBOL GPIO_TypeDef *GPIOD = (GPIO_TypeDef *)0x48000C00u;
WEAK_SYMBOL GPIO_TypeDef *GPIOE = (GPIO_TypeDef *)0x48001000u;
WEAK_SYMBOL GPIO_TypeDef *GPIOF = (GPIO_TypeDef *)0x48001400u;

/* Helper interno para habilitar o clock do port */
static void gpio_enable_clock(GPIO_TypeDef *port) {
    if (port == GPIOA) {
        RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    } else if (port == GPIOB) {
        RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    } else if (port == GPIOC) {
        RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    } else if (port == GPIOD) {
        RCC->AHBENR |= RCC_AHBENR_GPIODEN;
    } else if (port == GPIOE) {
        RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
    } else if (port == GPIOF) {
        RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
    }
}

void gpio_init_output(GPIO_TypeDef* port, uint8_t pin) {
    gpio_enable_clock(port);

    /* MODER: 01 = saída */
    port->MODER &= ~(0x3u << (pin * 2u));
    port->MODER |=  (0x1u << (pin * 2u));

    /* OTYPER: 0 = push-pull */
    port->OTYPER &= ~(1u << pin);

    /* OSPEEDR: 11 = alta velocidade */
    port->OSPEEDR &= ~(0x3u << (pin * 2u));
    port->OSPEEDR |=  (0x3u << (pin * 2u));

    /* PUPDR: 00 = sem pull */
    port->PUPDR &= ~(0x3u << (pin * 2u));
}

void gpio_init_input(GPIO_TypeDef* port, uint8_t pin, bool pull_up, bool pull_down) {
    gpio_enable_clock(port);

    /* MODER: 00 = entrada */
    port->MODER &= ~(0x3u << (pin * 2u));

    /* OTYPER: 0 = push-pull */
    port->OTYPER &= ~(1u << pin);

    /* OSPEEDR: 00 = baixa velocidade */
    port->OSPEEDR &= ~(0x3u << (pin * 2u));

    /* PUPDR: configura conforme flags (se ambos true, mantém 00) */
    port->PUPDR &= ~(0x3u << (pin * 2u));
    if (pull_up && !pull_down) {
        port->PUPDR |= (0x1u << (pin * 2u));
    } else if (pull_down && !pull_up) {
        port->PUPDR |= (0x2u << (pin * 2u));
    }
}

void gpio_write(GPIO_TypeDef* port, uint8_t pin, uint8_t level) {
    if (level) {
        port->BSRR = (1u << pin);      /* Set */
    } else {
        port->BSRR = (1u << (pin + 16u)); /* Reset */
    }
}

void gpio_toggle(GPIO_TypeDef* port, uint8_t pin) {
    port->ODR ^= (1u << pin);
}

uint8_t gpio_read_input(GPIO_TypeDef* port, uint8_t pin) {
    return (uint8_t)((port->IDR >> pin) & 0x1u);
}

