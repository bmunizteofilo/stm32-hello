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

add_executable(test_i2c test_i2c.c)
target_link_libraries(test_i2c PRIVATE core)
add_test(NAME i2c COMMAND test_i2c)

add_executable(test_rtc test_rtc.c)
target_link_libraries(test_rtc PRIVATE core)
add_test(NAME rtc COMMAND test_rtc)

add_executable(test_usart test_usart.c)
target_link_libraries(test_usart PRIVATE core)
add_test(NAME usart COMMAND test_usart)

add_executable(test_tim test_tim.c)
target_link_libraries(test_tim PRIVATE core)
add_test(NAME tim COMMAND test_tim)

add_executable(test_adc test_adc.c)
target_link_libraries(test_adc PRIVATE core)
add_test(NAME adc COMMAND test_adc)
add_executable(test_sensors test_sensors.c)
target_link_libraries(test_sensors PRIVATE core)
add_test(NAME sensors COMMAND test_sensors)

add_executable(test_display test_display.c)
target_link_libraries(test_display PRIVATE core)
add_test(NAME display COMMAND test_display)
