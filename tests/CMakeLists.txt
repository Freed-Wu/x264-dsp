find_package(check CONFIG)

add_executable(check_downsample check_downsample.c ../downsample.c)
target_compile_options(check_downsample PUBLIC -fprofile-arcs -ftest-coverage)
target_link_libraries(check_downsample check gcov)
add_test(NAME downsample COMMAND check_downsample)

add_custom_command(
  OUTPUT check_downsample.c
  COMMAND checkmk ${CMAKE_CURRENT_SOURCE_DIR}/check_downsample.c.in >
          check_downsample.c)
