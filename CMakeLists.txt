

cmake_minimum_required(VERSION 3.7)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCE_FILES main.cpp Partido.cpp Partido.h Equipo.cpp Equipo.h Jugador.cpp Jugador.h Resultado.cpp Resultado.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})

