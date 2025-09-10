#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "spi.h"
#include "gpio.h"

/** @file sdcard.h
 *  @brief Driver simples para cartão SD em modo SPI.
 */

/** Estrutura de contexto para driver de cartão SD. */
typedef struct {
    SPI_TypeDef *spi;           /**< Periférico SPI utilizado */
    GPIO_TypeDef *cs_port;      /**< Porta do pino CS (opcional) */
    uint8_t cs_pin;             /**< Pino CS */
} sdcard_t;

/**
 * @brief Inicializa o cartão SD no modo SPI.
 *
 * Envia a sequência padrão de reset e comandos básicos (CMD0, CMD8,
 * CMD55/ACMD41).
 *
 * @param sd       Ponteiro para estrutura do driver.
 * @param spi      Periférico SPI a ser utilizado.
 * @param cs_port  Porta do pino CS (pode ser NULL para ignorar).
 * @param cs_pin   Número do pino CS.
 * @return true em caso de sucesso, false caso contrário.
 */
bool sdcard_init(sdcard_t *sd, SPI_TypeDef *spi, GPIO_TypeDef *cs_port, uint8_t cs_pin);

/**
 * @brief Lê um bloco de 512 bytes.
 *
 * @param sd   Ponteiro para driver inicializado.
 * @param addr Endereço do bloco (endereçamento por bloco).
 * @param buf  Buffer de 512 bytes para receber os dados.
 * @return true se a operação foi iniciada, false caso contrário.
 */
bool sdcard_read_block(sdcard_t *sd, uint32_t addr, uint8_t *buf);

/**
 * @brief Escreve um bloco de 512 bytes.
 *
 * @param sd   Ponteiro para driver inicializado.
 * @param addr Endereço do bloco (endereçamento por bloco).
 * @param buf  Buffer de 512 bytes com os dados.
 * @return true se a operação foi iniciada, false caso contrário.
 */
bool sdcard_write_block(sdcard_t *sd, uint32_t addr, const uint8_t *buf);

/**
 * @brief Exemplo de uso: inicializa, lê e grava o bloco 0.
 */
void sdcard_example(void);

#endif /* SDCARD_H */
