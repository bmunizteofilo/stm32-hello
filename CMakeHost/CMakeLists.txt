cmake_minimum_required(VERSION 3.16)
project(host_build C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)

# Flags por compilador: MSVC (Visual Studio) vs GCC/Clang
if(MSVC)
  add_compile_options(/W4 /WX /O2 /Zi)
else()
  add_compile_options(-Wall -Wextra -Werror -O2 -g)
endif()

# Fontes dos drivers "puros C" dentro do projeto da IDE
file(GLOB DRIVERS_SRC
  ../stm32_repo/Drivers/*.c
)

# Lib 'core' com os drivers (ou interface vazia se ainda não existirem .c)
if(DRIVERS_SRC)
  add_library(core STATIC ${DRIVERS_SRC})
  target_include_directories(core PUBLIC ../stm32_repo/Drivers)
else()
  add_library(core INTERFACE)
  target_include_directories(core INTERFACE ../stm32_repo/Drivers)
endif()

# Habilita testes SEMPRE; adiciona subpasta se existir
enable_testing()
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/tests/CMakeLists.txt")
  add_subdirectory(tests)
endif()
