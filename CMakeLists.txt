cmake_minimum_required(VERSION 3.16.3)

project(ProjetJungle C)

set(CMAKE_C_STANDARD 11)

#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#find_package(SDL2 REQUIRED)

#include_directories(${SDL2_INCLUDE_DIR})

add_executable(${CMAKE_PROJECT_NAME} main.c echiquier.c echiquier.h ia/model.c ia/model.h multiplayer/client.c multiplayer/client.h multiplayer/server.c multiplayer/server.h graphic/sdl_Implement.c graphic/sdl_Implement.h)

target_link_libraries(
        ${CMAKE_PROJECT_NAME}
        ${SDL2_LIBRARY}
        #-lmingw32
        #-lSDL2main
        #-lSDL2
        -lws2_32
)

