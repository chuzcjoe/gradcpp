file(GLOB grad_src "src/*.cpp")

add_library(grad ${grad_src})

target_include_directories(grad
    PUBLIC
    include)