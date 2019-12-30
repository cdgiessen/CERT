cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(CERT VERSION 0.0.1)

add_executable(cert src/main.cpp)

set_property(TARGET cert PROPERTY CXX_STANDARD 20)

target_compile_options(cert PUBLIC -fconstexpr-steps=1271242000)