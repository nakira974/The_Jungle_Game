//
// Created by maxime on 07/10/2020.
//
#ifndef PROJETJUNGLE_ECHIQUIER_H
#define PROJETJUNGLE_ECHIQUIER_H
#define MULTIPLAYER_EXIT "exit"
#define MULTIPLAYER_LOADING_BAR 6,666666666666667
#define DEFAULT_BUFLEN 1024
#define DEFAULT_PORT "32555"
#ifdef _WIN32

#include <io.h>
#include <conio.h>

#else
#include <memory.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <locale.h>


#define Color_Blue "\33[0:34m" // Color Start
#define Color_Red "\33[0:31m" // Color Start
#define Color_Yellow "\33[0:32m"
#define Color_End "\33[0m" // To flush out prev settings
#define Color_Purple "\33[0:95m" // To flush out prev settings

struct Animal;
struct Player;


struct Player {

    int id;
    char* name[150];
    bool isEnemy;
    int score;

};

enum Type {

    ELEPHANT = 'E',
    LION = 'L',
    TIGER = 'T',
    PANTHER = 'P',
    DOG = 'C',
    WOLF = 'O',
    CAT = 'H',
    RAT = 'R'

};

enum Zone {

    BLUE_TRAP,
    RED_TRAP,
    BLUE_SANCTUARY,
    RED_SANCTUARY,
    LAKE,
    NONE

};

struct Animal {

    char type;
    int x;
    int y;
    bool isEnemy;
    bool isAlive;
    bool canEat;
    int index;
    enum Zone zone;

};

void printChessTable();

void color(int t, int f);

bool searchIfCanEat(struct Animal animal, bool isEnemy, struct Player *playerTab);

bool hasRat(struct Animal, char direction);

bool canEat(struct Animal *enemy, struct Animal ally, struct Player *playerTab);

void generateChessSet();

enum Zone movingCheck(struct Animal animal);

void loadGame();

int readString(char *chaine, int longueur);

#endif //PROJETJUNGLE_ECHIQUIER_H
