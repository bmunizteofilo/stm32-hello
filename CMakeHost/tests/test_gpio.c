#include <assert.h>
#include <stdio.h>
#include "gpio.h"

/* Fakes para os registradores */
static RCC_TypeDef rcc_fake;
static GPIO_TypeDef gpioa_fake;
static GPIO_TypeDef gpiob_fake;

RCC_TypeDef *RCC = &rcc_fake;
GPIO_TypeDef *GPIOA = &gpioa_fake;
GPIO_TypeDef *GPIOB = &gpiob_fake;

int main(void) {
    printf("[gpio] executando testes...\n");

    /* Teste: inicialização como saída */
    rcc_fake.AHBENR = 0;
    gpioa_fake.MODER = 0;
    gpioa_fake.OTYPER = 0xFFFFu;
    gpioa_fake.OSPEEDR = 0;
    gpioa_fake.PUPDR = 0xFFFFFFFFu;
    gpio_init_output(GPIOA, 5);
    assert((rcc_fake.AHBENR & RCC_AHBENR_GPIOAEN) != 0);
    assert(((gpioa_fake.MODER >> (5 * 2)) & 0x3u) == 0x1u);
    assert(((gpioa_fake.OTYPER >> 5) & 0x1u) == 0u);
    assert(((gpioa_fake.OSPEEDR >> (5 * 2)) & 0x3u) == 0x3u);
    assert(((gpioa_fake.PUPDR >> (5 * 2)) & 0x3u) == 0x0u);

    /* Teste: write/toggle */
    gpioa_fake.BSRR = 0;
    gpio_write(GPIOA, 5, 1);
    assert(gpioa_fake.BSRR == (1u << 5));
    gpioa_fake.BSRR = 0;
    gpio_write(GPIOA, 5, 0);
    assert(gpioa_fake.BSRR == (1u << (5 + 16)));

    gpioa_fake.ODR = 0;
    gpio_toggle(GPIOA, 5);
    assert((gpioa_fake.ODR & (1u << 5)) != 0u);
    gpio_toggle(GPIOA, 5);
    assert((gpioa_fake.ODR & (1u << 5)) == 0u);

    /* Teste: inicialização como entrada com pull-up */
    rcc_fake.AHBENR = 0;
    gpiob_fake.MODER = 0xFFFFFFFFu;
    gpiob_fake.PUPDR = 0;
    gpiob_fake.OTYPER = 0xFFFFFFFFu;
    gpiob_fake.OSPEEDR = 0xFFFFFFFFu;
    gpio_init_input(GPIOB, 3, true, false);
    assert((rcc_fake.AHBENR & RCC_AHBENR_GPIOBEN) != 0);
    assert(((gpiob_fake.MODER >> (3 * 2)) & 0x3u) == 0x0u);
    assert(((gpiob_fake.PUPDR >> (3 * 2)) & 0x3u) == 0x1u);
    assert(((gpiob_fake.OTYPER >> 3) & 0x1u) == 0x0u);
    assert(((gpiob_fake.OSPEEDR >> (3 * 2)) & 0x3u) == 0x0u);

    /* Teste: entrada com pull-down */
    gpiob_fake.MODER = 0xFFFFFFFFu;
    gpiob_fake.PUPDR = 0;
    gpio_init_input(GPIOB, 4, false, true);
    assert(((gpiob_fake.MODER >> (4 * 2)) & 0x3u) == 0x0u);
    assert(((gpiob_fake.PUPDR >> (4 * 2)) & 0x3u) == 0x2u);

    /* Teste: leitura de input */
    gpiob_fake.IDR = 0;
    assert(gpio_read_input(GPIOB, 3) == 0u);
    gpiob_fake.IDR = (1u << 3);
    assert(gpio_read_input(GPIOB, 3) == 1u);

    printf("[gpio] OK!\n");
    return 0;
}

