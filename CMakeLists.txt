cmake_minimum_required(VERSION 3.16.3)
project(ProjetJungle C)

set(CMAKE_C_STANDARD 11)

add_executable(ProjetJungle main.c echiquier.c echiquier.h ia/model.c ia/model.h graphic/sdl_Implement.c graphic/sdl_Implement.h)

target_link_libraries(${CMAKE_PROJECT_NAME} -lws2_32)