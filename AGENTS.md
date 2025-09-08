# AGENTS.md — Guia do agente/Codex para este repositório

## Objetivo
Este repositório contém um projeto STM32 (STM32CubeIDE) **sem HAL**. O agente deve:
- **Gerar/editar código C “puro”** em `./stm32_repo/Drivers/` (ex.: `gpio.c/.h`, `uart.c/.h`, etc.).
- **Criar/rodar testes de unidade no host (PC)** em `./CMakeHost/` (sem acessar registradores reais).
- **Não modificar** o sistema de build da IDE (arquivos/pastas de `./stm32_repo/` como `.project`, `.cproject`, `.settings/`, `Startup/`, script de linker, etc.).

> **Importante:** não criar módulos não solicitados (ex.: “ring buffer”) sem pedido explícito.

---

## Estrutura do repositório

./
AGENTS.md
README.md
.gitignore
CMakeHost/ ← build de testes no host (PC)
CMakeLists.txt
tests/
CMakeLists.txt
test_sanity.c
# (novos testes entram aqui: test_gpio.c, test_uart.c, ...)
stm32_repo/ ← projeto STM32CubeIDE (compilação/flash/debug embarcado)
Inc/
Src/
Startup/
Drivers/ ← (drivers gerados pelo agente: .h/.c)
.project
.cproject
STM32F070RBTX_FLASH.ld


---

## Regras de atuação do agente
1. **Código de produção**: criar/editar **apenas** em `./stm32_repo/Drivers/` (arquivos `.h` e `.c`).
2. **Testes (host/PC)**: criar/editar em `./CMakeHost/` (`tests/`, `CMakeLists.txt`).  
   - Os testes **não** devem acessar registradores reais, nem depender de headers específicos do MCU/IDE.  
   - Quando necessário, use fakes/mocks simples.
3. **Não alterar**: `.project`, `.cproject`, `.settings/`, `Startup/`, script de linker e quaisquer configurações da IDE.
4. **Entrega**: ao final de cada tarefa, fornecer **diff/patch** e **logs** da execução dos testes (`ctest`) no host.
5. **Compatibilidade**: manter o código compilável em MSVC e GCC/Clang (C99), sem warnings (ver flags abaixo).

---

## Compilação & testes no host (PC)

### Windows (Visual Studio / MSVC)
```powershell
cd C:\develop\stm32-hello\CMakeHost
mkdir build; cd build
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release -j
ctest -C Release --output-on-failure
