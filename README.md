# STM32 Hello (sem HAL)

Este repositório contém um projeto STM32 (STM32CubeIDE) sem HAL.

## Estrutura
- `stm32_repo/` – Projeto da IDE (ex.: `stm32-drivers`) usado para compilar/flash/debug.
- (futuro) `src/` – Código C “puro” compartilhado (drivers/libs) que integraremos com agentes.
- (futuro) `CMakeHost/` – Ambiente de testes no host (PC) para agentes/CI.

## Como abrir/compilar (STM32CubeIDE)
1. Abrir STM32CubeIDE → **File > Open Projects from File System...**
2. Selecione `stm32_repo/SEU_PROJETO` e importe.
3. Compile/flash normalmente.

## Notas
- O build embarcado acontece pela IDE.
- Testes no host/integração com agentes serão adicionados depois.
