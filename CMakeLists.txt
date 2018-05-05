

cmake_minimum_required(VERSION 3.7)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCE_FILES src/Entity.h src/PlayerControllerHuman.cpp src/PlayerControllerHuman.h src/PlayerControllerAI.cpp src/PlayerControllerAI.h src/TeamFactory.cpp src/TeamFactory.h src/Texto.cpp src/Texto.h src/Texture.cpp src/Texture.h src/Surface.h src/Surface.cpp src/Animation.cpp src/Animation.h src/Camera.cpp src/Camera.h src/GameConstants.h src/Log.cpp src/Log.h src/YAMLReader.cpp src/YAMLReader.h src/Player.cpp src/Player.h src/World.cpp src/World.h src/Surface.cpp src/Surface.h src/main.cpp src/Partido.cpp src/Partido.h src/Equipo.cpp src/Equipo.h src/Jugador.cpp src/Jugador.h src/Resultado.cpp src/Resultado.h src/PlayerModel.cpp src/PlayerModel.h src/PlayerView.cpp src/PlayerView.h src/PlayerController.cpp src/PlayerController.h src/TextureSetter.cpp src/TextureSetter.h src/BallModel.cpp src/BallModel.h src/BallView.cpp src/BallView.h src/BallController.cpp src/BallController.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})

