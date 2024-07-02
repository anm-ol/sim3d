cmake_minimum_required(VERSION 3.15)
project(sim3d)

set(CMAKE_CXX_STANDARD 17)

find_package(unofficial-imguizmo CONFIG REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE unofficial::imguizmo::imguizmo)