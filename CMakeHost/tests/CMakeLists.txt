# CMakeHost/tests/CMakeLists.txt
add_executable(test_sanity test_sanity.c)
target_link_libraries(test_sanity PRIVATE core)  # 'core' vem do CMakeLists da raiz do CMakeHost
add_test(NAME sanity COMMAND test_sanity)

add_executable(test_gpio test_gpio.c)
target_link_libraries(test_gpio PRIVATE core)
add_test(NAME gpio COMMAND test_gpio)
