/*
 * Dynamic software debouncer for digital inputs.
 *
 * The driver itself does not perform timing. The application must call
 * debounce_process() at a fixed rate—typically from a SysTick handler,
 * periodic timer interrupt, or the main loop. The `ticks` parameter of
 * debounce_add() specifies how many consecutive samples are required to
 * validate a state change. The effective debounce time corresponds to
 * `ticks` multiplied by the interval between calls to debounce_process().
 */
#include "debounce.h"
#include <stdlib.h>

#ifndef DEBOUNCE_DEFAULT_TICKS
#define DEBOUNCE_DEFAULT_TICKS 5u
#endif

typedef struct {
    int id;
    debounce_read_fn read;
    void *read_ctx;
    debounce_cb_t on_press;
    debounce_cb_t on_release;
    void *cb_ctx;
    bool stable_state;
    bool last_sample;
    uint8_t counter;
    uint8_t threshold;
} debounce_entry_t;

static debounce_entry_t *entries;
static size_t entry_count;
static int next_id;

void debounce_reset(void) {
    free(entries);
    entries = NULL;
    entry_count = 0;
    next_id = 0;
}

int debounce_add(debounce_read_fn read, void *read_ctx,
                 debounce_cb_t on_press, debounce_cb_t on_release,
                 void *cb_ctx, uint8_t ticks) {
    if (!read) {
        return -1;
    }
    debounce_entry_t *tmp = realloc(entries, (entry_count + 1u) * sizeof(*entries));
    if (!tmp) {
        return -1;
    }
    entries = tmp;
    debounce_entry_t *e = &entries[entry_count];
    e->id = next_id++;
    e->read = read;
    e->read_ctx = read_ctx;
    e->on_press = on_press;
    e->on_release = on_release;
    e->cb_ctx = cb_ctx;
    e->stable_state = e->last_sample = read(read_ctx);
    e->counter = 0u;
    e->threshold = ticks ? ticks : DEBOUNCE_DEFAULT_TICKS;
    entry_count++;
    return e->id;
}

bool debounce_remove(int id) {
    for (size_t i = 0; i < entry_count; ++i) {
        if (entries[i].id == id) {
            for (size_t j = i + 1; j < entry_count; ++j) {
                entries[j - 1] = entries[j];
            }
            entry_count--;
            if (entry_count == 0) {
                free(entries);
                entries = NULL;
            } else {
                debounce_entry_t *tmp = realloc(entries, entry_count * sizeof(*entries));
                if (tmp) {
                    entries = tmp;
                }
            }
            return true;
        }
    }
    return false;
}

void debounce_process(void) {
    for (size_t i = 0; i < entry_count; ++i) {
        debounce_entry_t *e = &entries[i];
        bool sample = e->read(e->read_ctx);
        if (sample != e->last_sample) {
            e->last_sample = sample;
            e->counter = 0u;
        } else if (e->counter < e->threshold) {
            e->counter++;
        }
        if (e->counter >= e->threshold && sample != e->stable_state) {
            e->stable_state = sample;
            if (sample) {
                if (e->on_press) {
                    e->on_press(e->cb_ctx);
                }
            } else {
                if (e->on_release) {
                    e->on_release(e->cb_ctx);
                }
            }
        }
    }
}
