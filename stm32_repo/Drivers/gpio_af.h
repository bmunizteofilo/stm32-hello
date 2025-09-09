#ifndef GPIO_AF_H
#define GPIO_AF_H

/*
 * Mapa completo de funções alternativas para os pinos do STM32F0.
 * Cada macro segue o padrão PXn_AFy_FUNC e contém o valor numérico
 * que deve ser escrito no registrador AFR correspondente.
 */

/* -------- Port A -------- */
#define PA0_AF0_EVENTOUT      0u
#define PA0_AF1_TIM2_CH1      1u
#define PA0_AF2_TIM2_ETR      2u
#define PA0_AF4_USART2_CTS    4u

#define PA1_AF0_EVENTOUT      0u
#define PA1_AF1_TIM2_CH2      1u
#define PA1_AF4_USART2_RTS    4u
#define PA1_AF5_TIM15_CH1N    5u

#define PA2_AF0_EVENTOUT      0u
#define PA2_AF1_TIM2_CH3      1u
#define PA2_AF4_USART2_TX     4u
#define PA2_AF5_TIM15_CH1     5u

#define PA3_AF0_EVENTOUT      0u
#define PA3_AF1_TIM2_CH4      1u
#define PA3_AF4_USART2_RX     4u
#define PA3_AF5_TIM15_CH2     5u

#define PA4_AF0_SPI1_NSS      0u
#define PA4_AF1_TIM14_CH1     1u
#define PA4_AF4_USART2_CK     4u

#define PA5_AF0_SPI1_SCK      0u
#define PA5_AF1_TIM2_CH1      1u
#define PA5_AF2_TIM2_ETR      2u

#define PA6_AF0_SPI1_MISO     0u
#define PA6_AF1_TIM3_CH1      1u
#define PA6_AF2_TIM1_BKIN     2u
#define PA6_AF4_TIM16_CH1     4u
#define PA6_AF5_TIM1_BKIN2    5u
#define PA6_AF6_TIM1_CH1N     6u
#define PA6_AF7_TIM3_ETR      7u

#define PA7_AF0_SPI1_MOSI     0u
#define PA7_AF1_TIM3_CH2      1u
#define PA7_AF2_TIM1_CH1N     2u
#define PA7_AF4_TIM14_CH1     4u
#define PA7_AF5_TIM1_ETR      5u
#define PA7_AF6_TIM3_CH2N     6u
#define PA7_AF7_COMP1_OUT     7u

#define PA8_AF0_MCO           0u
#define PA8_AF1_USART1_CK     1u
#define PA8_AF2_TIM1_CH1      2u
#define PA8_AF4_I2C3_SCL      4u

#define PA9_AF0_MCO           0u
#define PA9_AF1_USART1_TX     1u
#define PA9_AF2_TIM1_CH2      2u
#define PA9_AF4_I2C1_SCL      4u

#define PA10_AF0_MCO          0u
#define PA10_AF1_USART1_RX    1u
#define PA10_AF2_TIM1_CH3     2u
#define PA10_AF4_I2C1_SDA     4u

#define PA11_AF0_EVENTOUT     0u
#define PA11_AF1_USART1_CTS   1u
#define PA11_AF2_TIM1_CH4     2u
#define PA11_AF4_I2C2_SDA     4u

#define PA12_AF0_EVENTOUT     0u
#define PA12_AF1_USART1_RTS   1u
#define PA12_AF2_TIM1_ETR     2u
#define PA12_AF4_I2C2_SCL     4u

#define PA13_AF0_SWDIO        0u

#define PA14_AF0_SWCLK        0u
#define PA14_AF1_USART2_TX    1u
#define PA14_AF4_TIM15_CH1    4u

#define PA15_AF0_SPI1_NSS     0u
#define PA15_AF1_USART2_RX    1u
#define PA15_AF2_TIM2_CH1     2u
#define PA15_AF4_TIM1_BKIN    4u

/* -------- Port B -------- */
#define PB0_AF0_EVENTOUT      0u
#define PB0_AF1_TIM3_CH3      1u
#define PB0_AF2_TIM1_CH2N     2u
#define PB0_AF4_TIM14_CH1     4u
#define PB0_AF7_COMP1_OUT     7u

#define PB1_AF0_EVENTOUT      0u
#define PB1_AF1_TIM3_CH4      1u
#define PB1_AF2_TIM1_CH3N     2u
#define PB1_AF4_TIM14_CH1     4u

#define PB2_AF0_EVENTOUT      0u
#define PB2_AF1_TIM15_CH1     1u
#define PB2_AF2_TIM15_CH1N    2u

#define PB3_AF0_SPI1_SCK      0u
#define PB3_AF1_TIM2_CH2      1u
#define PB3_AF4_USART1_RX     4u

#define PB4_AF0_SPI1_MISO     0u
#define PB4_AF1_TIM3_CH1      1u
#define PB4_AF4_USART1_TX     4u

#define PB5_AF0_SPI1_MOSI     0u
#define PB5_AF1_TIM3_CH2      1u
#define PB5_AF4_I2C1_SMBA     4u

#define PB6_AF0_EVENTOUT      0u
#define PB6_AF2_TIM16_CH1N    2u
#define PB6_AF4_I2C1_SCL      4u
#define PB6_AF5_USART1_TX     5u

#define PB7_AF0_EVENTOUT      0u
#define PB7_AF2_TIM17_CH1N    2u
#define PB7_AF4_I2C1_SDA      4u
#define PB7_AF5_USART1_RX     5u

#define PB8_AF0_EVENTOUT      0u
#define PB8_AF1_TIM16_CH1     1u
#define PB8_AF2_USART3_RX     2u
#define PB8_AF4_I2C1_SCL      4u

#define PB9_AF0_EVENTOUT      0u
#define PB9_AF1_TIM17_CH1     1u
#define PB9_AF2_USART3_TX     2u
#define PB9_AF4_I2C1_SDA      4u

#define PB10_AF0_EVENTOUT     0u
#define PB10_AF1_TIM2_CH3     1u
#define PB10_AF2_USART3_TX    2u
#define PB10_AF4_I2C2_SCL     4u

#define PB11_AF0_EVENTOUT     0u
#define PB11_AF1_TIM2_CH4     1u
#define PB11_AF2_USART3_RX    2u
#define PB11_AF4_I2C2_SDA     4u

#define PB12_AF0_SPI2_NSS     0u
#define PB12_AF1_TIM1_BKIN    1u
#define PB12_AF2_USART3_CK    2u
#define PB12_AF4_I2C2_SMBA    4u

#define PB13_AF0_SPI2_SCK     0u
#define PB13_AF1_TIM1_CH1N    1u
#define PB13_AF2_USART3_CTS   2u

#define PB14_AF0_SPI2_MISO    0u
#define PB14_AF1_TIM1_CH2N    1u
#define PB14_AF2_USART3_RTS   2u

#define PB15_AF0_SPI2_MOSI    0u
#define PB15_AF1_TIM1_CH3N    1u
#define PB15_AF2_TIM15_CH1    2u

/* -------- Port C -------- */
#define PC0_AF0_EVENTOUT      0u
#define PC0_AF1_I2C1_SDA      1u
#define PC0_AF2_USART4_TX     2u

#define PC1_AF0_EVENTOUT      0u
#define PC1_AF1_I2C1_SCL      1u
#define PC1_AF2_USART4_RX     2u

#define PC2_AF0_EVENTOUT      0u
#define PC2_AF1_I2C2_SDA      1u
#define PC2_AF2_USART5_TX     2u

#define PC3_AF0_EVENTOUT      0u
#define PC3_AF1_I2C2_SCL      1u
#define PC3_AF2_USART5_RX     2u

#define PC4_AF0_EVENTOUT      0u
#define PC4_AF1_I2C1_SDA      1u

#define PC5_AF0_EVENTOUT      0u
#define PC5_AF1_I2C1_SCL      1u

#define PC6_AF0_EVENTOUT      0u
#define PC6_AF1_TIM3_CH1      1u
#define PC6_AF2_USART6_TX     2u

#define PC7_AF0_EVENTOUT      0u
#define PC7_AF1_TIM3_CH2      1u
#define PC7_AF2_USART6_RX     2u

#define PC8_AF0_EVENTOUT      0u
#define PC8_AF1_TIM3_CH3      1u
#define PC8_AF2_USART6_CK     2u

#define PC9_AF0_EVENTOUT      0u
#define PC9_AF1_TIM3_CH4      1u
#define PC9_AF4_I2C1_SDA      4u

#define PC10_AF0_EVENTOUT     0u
#define PC10_AF1_USART4_TX    1u

#define PC11_AF0_EVENTOUT     0u
#define PC11_AF1_USART4_RX    1u

#define PC12_AF0_EVENTOUT     0u
#define PC12_AF1_USART5_TX    1u

#define PC13_AF0_EVENTOUT     0u
#define PC14_AF0_EVENTOUT     0u
#define PC15_AF0_EVENTOUT     0u

#endif /* GPIO_AF_H */

