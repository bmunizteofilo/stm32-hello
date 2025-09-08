#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Estruturas mínimas dos registradores RCC e GPIO
 * Baseado no RM0091 (STM32F0). Apenas campos utilizados são definidos.
 */

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR; /* Offset 0x14 */
} RCC_TypeDef;

typedef struct {
    volatile uint32_t MODER;   /* Offset 0x00 */
    volatile uint32_t OTYPER;  /* Offset 0x04 */
    volatile uint32_t OSPEEDR; /* Offset 0x08 */
    volatile uint32_t PUPDR;   /* Offset 0x0C */
    volatile uint32_t IDR;     /* Offset 0x10 */
    volatile uint32_t ODR;     /* Offset 0x14 */
    volatile uint32_t BSRR;    /* Offset 0x18 */
    volatile uint32_t LCKR;    /* Offset 0x1C */
    volatile uint32_t AFR[2];  /* Offset 0x20, 0x24 */
    volatile uint32_t BRR;     /* Offset 0x28 */
} GPIO_TypeDef;

/*
 * Ponteiros para bases dos periféricos. No build de testes estes
 * símbolos podem ser redefinidos para fakes.
 */
extern RCC_TypeDef *RCC;
extern GPIO_TypeDef *GPIOA;
extern GPIO_TypeDef *GPIOB;
extern GPIO_TypeDef *GPIOC;
extern GPIO_TypeDef *GPIOD;
extern GPIO_TypeDef *GPIOE;
extern GPIO_TypeDef *GPIOF;

/* Bits de clock no RCC->AHBENR */
#define RCC_AHBENR_GPIOAEN (1u << 17)
#define RCC_AHBENR_GPIOBEN (1u << 18)
#define RCC_AHBENR_GPIOCEN (1u << 19)
#define RCC_AHBENR_GPIODEN (1u << 20)
#define RCC_AHBENR_GPIOEEN (1u << 21)
#define RCC_AHBENR_GPIOFEN (1u << 22)

/* API pública */
void gpio_init_output(GPIO_TypeDef* port, uint8_t pin);
void gpio_init_input(GPIO_TypeDef* port, uint8_t pin, bool pull_up, bool pull_down);
void gpio_write(GPIO_TypeDef* port, uint8_t pin, uint8_t level);
void gpio_toggle(GPIO_TypeDef* port, uint8_t pin);
uint8_t gpio_read_input(GPIO_TypeDef* port, uint8_t pin);

#endif /* GPIO_H */
