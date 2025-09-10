#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file keypad.h
 * @brief Driver genérico para teclados matriciais.
 *
 * O driver realiza a varredura de linhas/colunas através de callbacks
 * fornecidos pela aplicação. Cada varredura retorna o índice da tecla
 * pressionada ou -1 se nenhuma tecla for detectada.
 */

/** Callback usado para acionar uma linha do teclado.
 *  Deve definir o nível lógico da linha especificada.
 */
typedef void (*keypad_drive_row_fn)(void *ctx, uint8_t row, bool level);

/** Callback usado para ler o estado de uma coluna.
 *  Deve retornar false quando a tecla da coluna estiver pressionada.
 */
typedef bool (*keypad_read_col_fn)(void *ctx, uint8_t col);

/** Estrutura de controle do teclado matricial. */
typedef struct {
    uint8_t rows;                 /**< Número de linhas. */
    uint8_t cols;                 /**< Número de colunas. */
    keypad_drive_row_fn drive_row;/**< Função para acionar linhas. */
    keypad_read_col_fn read_col;  /**< Função para ler colunas. */
    void *ctx;                    /**< Contexto repassado aos callbacks. */
} keypad_t;

/**
 * @brief Inicializa a estrutura de teclado.
 *
 * @param kp        Estrutura a ser inicializada.
 * @param rows      Número de linhas.
 * @param cols      Número de colunas.
 * @param drive_row Função para acionar linhas.
 * @param read_col  Função para ler colunas.
 * @param ctx       Contexto repassado aos callbacks.
 */
void keypad_init(keypad_t *kp, uint8_t rows, uint8_t cols,
                 keypad_drive_row_fn drive_row,
                 keypad_read_col_fn read_col, void *ctx);

/**
 * @brief Realiza uma varredura e retorna a tecla pressionada.
 *
 * O índice retornado segue a convenção `row * cols + col`.
 * Retorna -1 se nenhuma tecla for detectada.
 */
int keypad_scan(keypad_t *kp);

/** Função de exemplo mostrando uso com GPIO. */
void keypad_example(void);

/* Exemplo de uso:
 *
 * static const uint8_t row_pins[2] = {0, 1};
 * static const uint8_t col_pins[3] = {2, 3, 4};
 *
 * static void drive_row(void *ctx, uint8_t row, bool level) {
 *     (void)ctx;
 *     gpio_write(GPIOA, row_pins[row], level);
 * }
 *
 * static bool read_col(void *ctx, uint8_t col) {
 *     (void)ctx;
 *     // Retorna true quando coluna está em nível alto
 *     return gpio_read(GPIOA, col_pins[col]);
 * }
 *
 * keypad_t kp;
 * keypad_init(&kp, 2, 3, drive_row, read_col, NULL);
 * int tecla = keypad_scan(&kp);
 */

#ifdef __cplusplus
}
#endif

#endif /* KEYPAD_H */
