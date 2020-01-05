cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(CERT VERSION 0.0.1)

add_executable(cert src/main.cpp 
    src/threads/thread_0.cpp src/threads/thread_1.cpp src/threads/thread_2.cpp src/threads/thread_3.cpp 
    src/threads/thread_4.cpp src/threads/thread_5.cpp src/threads/thread_6.cpp src/threads/thread_7.cpp 
    src/threads/thread_8.cpp src/threads/thread_9.cpp src/threads/thread_10.cpp src/threads/thread_11.cpp 
    src/threads/threads.h
    src/color.h src/ray.h src/shape.h src/shape.h src/util.h src/vec3.h)

set_property(TARGET cert PROPERTY CXX_STANDARD 20)

target_compile_options(cert PUBLIC -std=c++2a -fconstexpr-steps=2147483647 -O3)