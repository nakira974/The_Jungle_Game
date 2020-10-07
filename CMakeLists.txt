cmake_minimum_required(VERSION 3.17)
project(ProjetJungle C)

set(CMAKE_C_STANDARD 11)

add_executable(ProjetJungle main.c verifications.c verifications.h echequier.c echequier.h)