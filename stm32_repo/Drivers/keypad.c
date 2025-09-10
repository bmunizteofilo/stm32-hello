#include "keypad.h"

void keypad_init(keypad_t *kp, uint8_t rows, uint8_t cols,
                 keypad_drive_row_fn drive_row,
                 keypad_read_col_fn read_col, void *ctx) {
    if (!kp) {
        return;
    }
    kp->rows = rows;
    kp->cols = cols;
    kp->drive_row = drive_row;
    kp->read_col = read_col;
    kp->ctx = ctx;
}

int keypad_scan(keypad_t *kp) {
    if (!kp || !kp->drive_row || !kp->read_col) {
        return -1;
    }
    for (uint8_t r = 0; r < kp->rows; ++r) {
        kp->drive_row(kp->ctx, r, false); /* linha ativa em nível baixo */
        for (uint8_t c = 0; c < kp->cols; ++c) {
            if (!kp->read_col(kp->ctx, c)) {
                kp->drive_row(kp->ctx, r, true);
                return (int)(r * kp->cols + c);
            }
        }
        kp->drive_row(kp->ctx, r, true); /* desativa linha */
    }
    return -1;
}

/* Exemplo simples utilizando GPIO para um teclado 2x3. */
#ifdef STM32F0_FIRMWARE
#include "gpio.h"

static const uint8_t example_row_pins[2] = {0, 1};
static const uint8_t example_col_pins[3] = {2, 3, 4};

static void example_drive_row(void *ctx, uint8_t row, bool level) {
    GPIO_TypeDef *port = (GPIO_TypeDef *)ctx;
    gpio_write(port, example_row_pins[row], level);
}

static bool example_read_col(void *ctx, uint8_t col) {
    GPIO_TypeDef *port = (GPIO_TypeDef *)ctx;
    /* Colunas configuradas com pull-up retornam 0 quando pressionadas */
    return gpio_read(port, example_col_pins[col]);
}

void keypad_example(void) {
    keypad_t kp;
    keypad_init(&kp, 2, 3, example_drive_row, example_read_col, GPIOA);
    (void)keypad_scan(&kp); /* resultado ignorado */
}
#else
void keypad_example(void) { /* apenas demonstração */ }
#endif
