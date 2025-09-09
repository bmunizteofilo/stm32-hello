# CMakeHost/tests/CMakeLists.txt
add_executable(test_sanity test_sanity.c)
target_link_libraries(test_sanity PRIVATE core)
add_test(NAME sanity COMMAND test_sanity)

add_executable(test_spi test_spi.c)
target_link_libraries(test_spi PRIVATE core)
add_test(NAME spi COMMAND test_spi)

add_executable(test_rcc test_rcc.c)
target_link_libraries(test_rcc PRIVATE core)
add_test(NAME rcc COMMAND test_rcc)

add_executable(test_dma test_dma.c)
target_link_libraries(test_dma PRIVATE core)
add_test(NAME dma COMMAND test_dma)
