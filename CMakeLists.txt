cmake_minimum_required(VERSION 3.18)
project(mandelbrot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-mavx2")
find_package(fmt)
find_package(Threads)

add_executable(mandelbrot
        mandelbrot.cpp)

add_executable(mandelbrot_parallel
        mandelbrot_parallel.cpp
        thread_pool.hpp
        type_defs.hpp
        type_defs.cpp
        compute_scalar.hpp
        colour_chunk.hpp
        avx_support.hpp
        compute_avx.hpp
        write_csv.hpp)

target_link_libraries(mandelbrot_parallel
        fmt::fmt
        Threads::Threads)