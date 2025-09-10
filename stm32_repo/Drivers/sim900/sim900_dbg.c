#include "sim900_dbg.h"
#include "sim900_port.h"
#include <stdarg.h>
#include <stdio.h>

void sim900_dbg_log(const char *tag, const char *fmt, ...)
{
    char buf[128];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    sim900_port_log("[%s] %s\n", tag, buf);
}
