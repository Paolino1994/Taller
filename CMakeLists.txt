

cmake_minimum_required(VERSION 3.7)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCE_FILES src/client/GameMenu.h src/client/GameMenu.cpp src/client/Entity.h src/client/PlayerControllerHuman.cpp src/client/PlayerControllerHuman.h src/client/PlayerControllerAI.cpp src/client/PlayerControllerAI.h src/client/TeamFactory.cpp src/client/TeamFactory.h src/client/Texto.cpp src/client/Texto.h src/client/Texture.cpp src/client/Texture.h src/client/Surface.h src/client/Surface.cpp src/client/Animation.cpp src/client/Animation.h src/client/Camera.cpp src/client/Camera.h src/client/GameConstants.h src/common/Log.cpp src/common/Log.h src/client/YAMLReader.cpp src/client/YAMLReader.h src/client/Player.cpp src/client/Player.h src/client/World.cpp src/client/World.h src/client/Surface.cpp src/client/Surface.h src/client/main.cpp src/client/Partido.cpp src/client/Partido.h src/client/Equipo.cpp src/client/Equipo.h src/client/Jugador.cpp src/client/Jugador.h src/client/Resultado.cpp src/client/Resultado.h src/client/PlayerModel.cpp src/client/PlayerModel.h src/client/PlayerView.cpp src/client/PlayerView.h src/client/PlayerController.cpp src/client/PlayerController.h src/client/TextureSetter.cpp src/client/TextureSetter.h src/client/BallModel.cpp src/client/BallModel.h src/client/BallView.cpp src/client/BallView.h src/client/BallController.cpp src/client/BallController.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})

