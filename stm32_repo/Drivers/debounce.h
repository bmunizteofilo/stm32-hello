#ifndef DEBOUNCE_H
#define DEBOUNCE_H

/**
 * @file debounce.h
 * @brief Runtime-configurable software debouncer for digital inputs.
 *
 * The module does not manage any hardware timer. The application is
 * responsible for calling ::debounce_process() at a constant rate. The
 * number of successive samples required to accept a transition is
 * configured through the `ticks` parameter of ::debounce_add(). The
 * resulting debounce interval is `ticks` multiplied by the period
 * between calls to ::debounce_process().
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Function to read current level of input. Should return true when pressed. */
typedef bool (*debounce_read_fn)(void *ctx);

/** Callback invoked on press or release events. */
typedef void (*debounce_cb_t)(void *ctx);

/**
 * @brief Add a new input to debounce manager.
 *
 * @param read       Function used to sample the input.
 * @param read_ctx   Context pointer passed to read function.
 * @param on_press   Callback invoked when the input becomes pressed.
 * @param on_release Callback invoked when the input becomes released.
 * @param cb_ctx     Context pointer passed to callbacks.
 * @param ticks      Number of consecutive samples required to validate
 *                   a change. The effective debounce time corresponds to
 *                   `ticks` multiplied by the period between consecutive
 *                   calls to ::debounce_process(). If zero, a default
 *                   value is used.
 * @return Identifier of the new input, or -1 on failure.
 */
int debounce_add(debounce_read_fn read, void *read_ctx,
                 debounce_cb_t on_press, debounce_cb_t on_release,
                 void *cb_ctx, uint8_t ticks);

/**
 * @brief Remove an input previously added.
 * @param id Identifier returned by debounce_add.
 * @return true on success, false if id not found.
 */
bool debounce_remove(int id);

/**
 * @brief Process all registered inputs.
 *
 * Call this function periodically at a fixed rate to perform debouncing.
 * Typical usage is from a SysTick handler, timer interrupt or the main
 * execution loop. The sampling period established by this call frequency
 * is used together with the `ticks` threshold to determine the total
 * debounce time for each input.
 */
void debounce_process(void);

/**
 * @brief Remove all inputs and release resources.
 */
void debounce_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBOUNCE_H */
