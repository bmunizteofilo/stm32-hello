#include <assert.h>
#include <string.h>
#include "sim900/sim900.h"
#include "sim900/sim900_sms.h"

/* Stubs for port layer */
struct sim900_port { int dummy; };

bool sim900_port_init(sim900_port_t **out, uint32_t baud) { (void)baud; static struct sim900_port p; *out = &p; return true; }
bool sim900_port_write(sim900_port_t *p, const uint8_t *data, size_t len) { (void)p; (void)data; return len>0; }
size_t sim900_port_read(sim900_port_t *p, uint8_t *out, size_t maxlen, uint32_t timeout_ms) { (void)p; (void)out; (void)maxlen; (void)timeout_ms; return 0; }
uint32_t sim900_port_millis(void) { static uint32_t t; return t++; }
void *sim900_port_mutex_create(void) { return NULL; }
void sim900_port_mutex_lock(void *m) { (void)m; }
void sim900_port_mutex_unlock(void *m) { (void)m; }
bool sim900_port_set_pwrkey(bool level) { (void)level; return false; }
bool sim900_port_pulse_pwrkey(uint32_t ms) { (void)ms; return false; }
bool sim900_port_pulse_reset(uint32_t ms) { (void)ms; return false; }
void sim900_port_log(const char *fmt, ...) { (void)fmt; }

int main(void)
{
    sim900_port_t *p;
    assert(sim900_port_init(&p, 115200));
    assert(sim900_init(p));
    sim900_sms_set_text_mode(true);
    return 0;
}
