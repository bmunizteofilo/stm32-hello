# STM32 Hello (sem HAL)

Projeto de exemplo para microcontroladores STM32 escrito apenas em C, **sem utilizar a HAL**. O objetivo é disponibilizar drivers simples, portáveis e acompanhados de testes que podem ser executados no host (PC).

## Estrutura do repositório

- `stm32_repo/` – projeto da STM32CubeIDE para build/flash/debug no microcontrolador.
  - `Drivers/` – drivers e bibliotecas em C "puro" (sem HAL) organizados por categoria.
  - `Src/` – arquivos de aplicação (ex.: `main.c`).
- `CMakeHost/` – ambiente de testes no host usando CMake/ctest.
  - `tests/` – suíte de testes unitários cobrindo os drivers.

## Drivers disponíveis

### Núcleo da MCU
- **adc.c/h** – conversor analógico-digital.
- **debounce.c/h** – tratamento de bouncing em chaves mecânicas.
- **dma.c/h** – controlador de DMA.
- **gpio.c/h**, **gpio_af.h** – manipulação de portas e funções alternativas.
- **i2c.c/h** – barramento I²C.
- **keypad.c/h** – leitura de teclados matriciais.
- **rcc.c/h** – controle do clock do sistema.
- **rtc.c/h** – utilitários de relógio em tempo real.
- **sdcard.c/h** – acesso básico a cartões SD via SPI.
- **spi.c/h** – barramento SPI.
- **tim.c/h** – temporizadores básicos.
- **usart.c/h** – comunicação serial USART.
- **cm0.c/cm0_stub.c** – stubs para rotinas específicas do núcleo ARM Cortex‑M0.

### Periféricos externos
- **Acelerometros/** – ADXL345, ADXL362, BMA280, BMI160, ICM20948, LIS3DH, LSM6DS3, MPU6050.
- **Display/** – controladores de display como HX8357, ILI9325/9341/9486, LGDP4535, R61509, RA8875, S6D1121, SSD1963 e ST7789.
- **Flash/** – memórias Flash SPI (A25LQ64, AT25SF041, EN25Q64, GD25Q32, IS25LP128, MX25L128, N25Q032, S25FL064L, SST26VF064B, W25Q64).
- **Leds/** – LEDs endereçáveis APA102, SK6812 e WS2812.
- **Motor/** – drivers de motor como A4988, BTN8982, BTS7960, DRV8825, DRV8833, DRV8871/DRV8876, L293D, L298N, MC33932, TB6612FNG, TMC2130, TMC2208, TMC2209 e VNH2SP30.
- **RealTime/** – RTCs DS1307, DS3231 e PCF8563.
- **Segments/** – controladores de displays de 7 segmentos MAX7219 e TM1637 (com exemplos).
- **Sensores/** – sensores ambientais AM2320, BME280, BME680, DHT11, DHT22, HTU21D, SHT11, SHT21, SHT31 e SI7021.
- **touch/** – controladores de touchscreen ADS7843, AR1021, CY8CTMA1036, FT5406, FT6206, FT6236, GT911, STMPE811, TSC2007 e XPT2046.

## Compilação e testes no host (PC)

```bash
cd CMakeHost
mkdir -p build && cd build
cmake ..
cmake --build . --config Release -j
ctest --output-on-failure
```

## Como abrir/compilar na STM32CubeIDE

1. Abrir STM32CubeIDE → **File > Open Projects from File System...**
2. Selecionar `stm32_repo/` e importar o projeto.
3. Compilar/flashar normalmente a placa desejada.

## Notas

- O build embarcado acontece pela IDE.
- Os testes no host garantem que os drivers permaneçam portáveis e livres da HAL.

