#include <assert.h>
#include <stddef.h>
#include "debounce.h"

static bool input_state;

static bool read_input(void *ctx) {
    (void)ctx;
    return input_state;
}

static int presses;
static int releases;

static void on_press(void *ctx) {
    (void)ctx;
    presses++;
}

static void on_release(void *ctx) {
    (void)ctx;
    releases++;
}

static void step(bool level) {
    input_state = level;
    debounce_process();
}

static void test_press_release(void) {
    debounce_reset();
    presses = 0;
    releases = 0;
    int id = debounce_add(read_input, NULL, on_press, on_release, NULL, 2);
    assert(id >= 0);
    /* Simulate bouncing on press */
    step(true);
    step(false);
    step(true);
    step(true);
    step(true);
    assert(presses == 1);
    /* Simulate bouncing on release */
    step(false);
    step(true);
    step(false);
    step(false);
    step(false);
    assert(releases == 1);
}

/* Second test: add two inputs, remove one, ensure callbacks only for remaining */
static bool input2_state;
static int presses2;

static bool read_input2(void *ctx) {
    (void)ctx;
    return input2_state;
}

static void on_press2(void *ctx) {
    (void)ctx;
    presses2++;
}

static void test_add_remove(void) {
    debounce_reset();
    presses = presses2 = 0;
    int id1 = debounce_add(read_input, NULL, on_press, NULL, NULL, 1);
    int id2 = debounce_add(read_input2, NULL, on_press2, NULL, NULL, 1);
    assert(id1 >= 0 && id2 >= 0 && id1 != id2);

    /* Trigger press on both inputs */
    input_state = true;
    input2_state = true;
    debounce_process();
    debounce_process();
    assert(presses == 1);
    assert(presses2 == 1);

    /* Remove first input and toggle it a few times */
    assert(debounce_remove(id1));
    input_state = false;
    debounce_process();
    input_state = true;
    debounce_process();
    assert(presses == 1);

    /* Second input should still respond */
    input2_state = false;
    debounce_process();
    debounce_process();
    input2_state = true;
    debounce_process();
    debounce_process();
    assert(presses2 == 2);
}

int main(void) {
    test_press_release();
    test_add_remove();
    return 0;
}
