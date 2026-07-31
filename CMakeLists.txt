cmake_minimum_required(VERSION 3.16)
project(cpp_lint_template CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
  -std=c++20
  -Wall
  -Wextra
  -Wpedantic
  -Wunused-variable
  -Wunused-function
  -Wshadow
  -Werror
  -O3
)

add_subdirectory(grad)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE grad)
