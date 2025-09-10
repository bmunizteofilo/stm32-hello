#include <assert.h>
#include "keypad.h"

/* Estado atual da linha ativada pelo driver */
static int current_row = -1;
/* Coordenadas da tecla considerada pressionada */
static int pressed_row = -1;
static int pressed_col = -1;

static void drv_row(void *ctx, uint8_t row, bool level) {
    (void)ctx;
    if (!level) {
        current_row = (int)row;
    } else if (current_row == (int)row) {
        current_row = -1;
    }
}

static bool rd_col(void *ctx, uint8_t col) {
    (void)ctx;
    /* Colunas retornam false quando a tecla correspondente está pressionada */
    if (current_row == pressed_row && (int)col == pressed_col) {
        return false;
    }
    return true;
}

static void test_scan(void) {
    keypad_t kp;
    keypad_init(&kp, 2, 3, drv_row, rd_col, NULL);

    /* Nenhuma tecla pressionada */
    pressed_row = pressed_col = -1;
    assert(keypad_scan(&kp) == -1);

    /* Pressiona tecla na segunda linha, terceira coluna */
    pressed_row = 1;
    pressed_col = 2;
    assert(keypad_scan(&kp) == 1 * 3 + 2);
}

int main(void) {
    test_scan();
    return 0;
}
