cmake_minimum_required(VERSION 3.13.1)
project(thermolib VERSION 1.0.0 DESCRIPTION "Library for conversion between thermocouple voltages and temperatures")

# Add all thermocouple tables to sources list
FILE(GLOB SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/tables/*.c) 
# Add thermolib sources
list(APPEND SOURCES 
        ${CMAKE_CURRENT_LIST_DIR}/src/tables.c
        ${CMAKE_CURRENT_LIST_DIR}/src/thermolib.c
)
add_library(thermolib ${SOURCES})

target_include_directories(thermolib PUBLIC
                           ${CMAKE_CURRENT_LIST_DIR}/include
)
target_include_directories(thermolib PRIVATE
                           ${CMAKE_CURRENT_LIST_DIR}/src/include
)
