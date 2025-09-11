#ifndef SIM800_PORT_MAP_H
#define SIM800_PORT_MAP_H

#include "usart.h"

/* Ajuste estes mapeamentos para as assinaturas reais do seu projeto. */

bool sim800_uart_lowlevel_init(uint32_t baud);
bool sim800_uart_lowlevel_write(const uint8_t *buf, size_t len);
size_t sim800_uart_lowlevel_read(uint8_t *buf, size_t maxlen, uint32_t timeout_ms);

#ifndef SIM800_UART_INIT
  #define SIM800_UART_INIT(baud)                 sim800_uart_lowlevel_init((uint32_t)(baud))
#endif

#ifndef SIM800_UART_WRITE
  #define SIM800_UART_WRITE(buf,len)             sim800_uart_lowlevel_write((const uint8_t*)(buf),(size_t)(len))
#endif

#ifndef SIM800_UART_READ
  #define SIM800_UART_READ(buf,maxlen,timeout)   sim800_uart_lowlevel_read((uint8_t*)(buf),(size_t)(maxlen),(uint32_t)(timeout))
#endif

/* Logger: se houver logger próprio, mapeie aqui. Caso contrário, use printf. */
#ifndef SIM800_LOGF
  #include <stdio.h>
  #define SIM800_LOGF(fmt, ...)                  printf("[SIM800] " fmt "\n", ##__VA_ARGS__)
#endif

/* Pinos de controle (opcional): se não existirem, deixe como stubs que retornam false */
#ifndef SIM800_GPIO_PWRKEY_SET
  #define SIM800_GPIO_PWRKEY_SET(level)          (false)
#endif

#ifndef SIM800_GPIO_PWRKEY_PULSE
  #define SIM800_GPIO_PWRKEY_PULSE(ms)           (false)
#endif

#ifndef SIM800_GPIO_RESET_PULSE
  #define SIM800_GPIO_RESET_PULSE(ms)            (false)
#endif

/* Tempo monotônico em ms: se não houver, implemente via tick do sistema */
#ifndef SIM800_MILLIS
  #include <stdint.h>
  extern uint32_t platform_millis(void);
  #define SIM800_MILLIS()                        platform_millis()
#endif

#endif /* SIM800_PORT_MAP_H */
