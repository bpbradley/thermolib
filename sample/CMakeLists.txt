
cmake_minimum_required(VERSION 3.13.1)
project(thermolib_sample)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
add_subdirectory(.. lib/thermolib)

add_executable(sample sample.c data.c)
target_link_libraries(sample PUBLIC thermolib)

install(TARGETS sample DESTINATION bin)
