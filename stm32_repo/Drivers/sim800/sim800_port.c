#include "sim800_port.h"
#include "sim800_port_map.h"
#include "sim800_cfg.h"

struct sim800_port {
    int dummy;
};

static struct sim800_port g_port;

/* Wrappers around existing USART driver. */
bool sim800_uart_lowlevel_init(uint32_t baud) {
    usart_cfg_t cfg;
    cfg.baudrate = baud;
    cfg.word_length = USART_WORD_8;
    cfg.stop_bits = USART_STOP_1;
    cfg.parity = USART_PARITY_NONE;
    cfg.mode = USART_MODE_TX_RX;
    cfg.hw_flow = USART_HW_NONE;
    cfg.oversampling = USART_OVER16;
    if (!usart_init(USART1, &cfg)) {
        return false;
    }
    usart_enable(USART1, true);
    return true;
}

bool sim800_uart_lowlevel_write(const uint8_t *buf, size_t len) {
    return usart_write_poll(USART1, buf, len);
}

size_t sim800_uart_lowlevel_read(uint8_t *buf, size_t maxlen, uint32_t timeout_ms) {
    uint32_t start = SIM800_MILLIS();
    size_t got = 0u;
    while (got < maxlen) {
        if (SIM800_MILLIS() - start >= timeout_ms) {
            break;
        }
        if (usart_read_poll(USART1, &buf[got], 1u)) {
            got++;
        }
    }
    return got;
}

bool sim800_port_init(sim800_port_t **out, uint32_t baud) {
    if (!out) return false;
    if (!SIM800_UART_INIT(baud)) return false;
    *out = &g_port;
    return true;
}

bool sim800_port_write(sim800_port_t *p, const uint8_t *data, size_t len) {
    (void)p;
    return SIM800_UART_WRITE(data, len);
}

size_t sim800_port_read(sim800_port_t *p, uint8_t *out, size_t maxlen, uint32_t timeout_ms) {
    (void)p;
    return SIM800_UART_READ(out, maxlen, timeout_ms);
}

uint32_t sim800_port_millis(void) {
    return SIM800_MILLIS();
}

/* No-op mutex implementation */
void *sim800_port_mutex_create(void) { static int dummy; return &dummy; }
void sim800_port_mutex_lock(void *m) { (void)m; }
void sim800_port_mutex_unlock(void *m) { (void)m; }

bool sim800_port_set_pwrkey(bool level) { (void)level; return SIM800_GPIO_PWRKEY_SET(level); }
bool sim800_port_pulse_pwrkey(uint32_t ms) { (void)ms; return SIM800_GPIO_PWRKEY_PULSE(ms); }
bool sim800_port_pulse_reset(uint32_t ms) { (void)ms; return SIM800_GPIO_RESET_PULSE(ms); }

void sim800_port_log(const char *fmt, ...) {
#if SIM800_LOG_ENABLE
    va_list ap;
    va_start(ap, fmt);
    printf("[SIM800] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
#else
    (void)fmt;
#endif
}

uint32_t platform_millis(void) {
    static uint32_t tick = 0u;
    return tick++;
}
