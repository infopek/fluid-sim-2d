cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "FluidSimulation2D")
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS
    system
    window
    graphics
    CONFIG REQUIRED   
)

include_directories(
    include
)

set(SOURCES
    src/core/FluidGrid.cpp
    src/core/Solver.cpp
    src/visualization/Renderer.cpp
    src/user-interaction/InputManager.cpp
    src/Application.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-system 
    sfml-network 
    sfml-graphics 
    sfml-window
)