cmake_minimum_required(VERSION 3.14.0)

set(PROJ_NAME "modules")
project(${PROJ_NAME} VERSION 0.0.1 LANGUAGES CXX)

add_library(module INTERFACE)

target_include_directories(module INTERFACE modules)

target_compile_features(module INTERFACE cxx_std_20)

set(CMAKE_CXX_STANDARD 20)

if(MODULE_COMPILE_TESTS)
    add_subdirectory(tests)
endif()
