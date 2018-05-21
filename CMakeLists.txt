

cmake_minimum_required(VERSION 3.7)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCE_FILES src/client/Animation.cpp src/client/Animation.h src/client/Ball.cpp src/client/Ball.h src/client/Camera.cpp src/client/Camera.h src/client/CommandSender.cpp src/client/CommandSender.h src/client/Entity.h src/client/GameMenu.cpp src/client/GameMenu.h src/client/GameSelectTeam.cpp src/client/GameSelectTeam.h src/client/ListenStart.cpp src/client/ListenStart.h src/client/main.cpp src/client/Player.cpp src/client/Player.h src/client/Surface.cpp src/client/Surface.h src/client/Texto.cpp src/client/Texto.h src/client/Texture.cpp src/client/Texture.h src/client/TextureSetter.cpp src/client/TextureSetter.h src/client/World.cpp src/client/World.h src/server/Animation.cpp src/server/Game.h src/server/PlayerView.cpp src/server/Animation.h src/server/main.cpp src/server/PlayerView.h src/server/BallController.cpp src/server/PlayerControllerAI.cpp src/server/RequestHandler.cpp src/server/BallController.h src/server/PlayerControllerAI.h src/server/RequestHandler.h src/server/BallModel.cpp src/server/PlayerController.cpp src/server/TeamFactory.cpp src/server/BallModel.h src/server/PlayerController.h src/server/TeamFactory.h src/server/BallView.cpp src/server/PlayerControllerHuman.cpp src/server/UserManager.cpp src/server/BallView.h src/server/PlayerControllerHuman.h src/server/UserManager.h src/server/Entity.h src/server/PlayerModel.cpp src/server/World.cpp src/server/Game.cpp src/server/PlayerModel.h src/server/World.h src/common/GameConstants.h src/common/Protocol.h src/common/SocketException.h src/common/YAMLReader.cpp src/common/Log.cpp src/common/Request.h src/common/Socket.h src/common/YAMLReader.h src/common/Log.h src/common/Socket.cpp src/common/SpriteInfoSetter.cpp src/common/Protocol.cpp src/common/SocketException.cpp src/common/SpriteInfoSetter.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} ${SDL2_LIBRARY})

