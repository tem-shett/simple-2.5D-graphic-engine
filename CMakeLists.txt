cmake_minimum_required(VERSION 3.21)
project(graphic-engine)

set(CMAKE_CXX_STANDARD 23)

set(SDL2_INCLUDE_DIR C:\\Games\\libs\\SDL\\include)
set(SDL2_LIB_DIR C:\\Games\\libs\\SDL\\lib\\x64)

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(graphic-engine main.cpp framebuffer.h framebuffer.cpp color_funcs.h color_funcs.cpp gameobjects.cpp gameobjects.h point.h bmp_files texture.cpp texture.h gamehandler.cpp gamehandler.h draw_objects.cpp draw_objects.h line.cpp line.h point.cpp bsp.cpp bsp.h geom_funcs.cpp geom_funcs.h screen_bsp.cpp screen_bsp.h)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
