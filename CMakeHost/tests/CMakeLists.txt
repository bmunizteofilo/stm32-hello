# CMakeHost/tests/CMakeLists.txt
add_executable(test_sanity test_sanity.c)
target_link_libraries(test_sanity PRIVATE core)
add_test(NAME sanity COMMAND test_sanity)

add_executable(test_rcc test_rcc.c)
target_link_libraries(test_rcc PRIVATE core)
add_test(NAME rcc COMMAND test_rcc)
