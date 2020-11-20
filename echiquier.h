//
// Created by maxime on 07/10/2020.
//
#ifndef PROJETJUNGLE_ECHIQUIER_H
#define PROJETJUNGLE_ECHIQUIER_H

#ifdef _WIN32

#include <windows.h>
#include <io.h>
#include <conio.h>

#else
#include <memory.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define Color_Blue "\33[0:34m" // Color Start
#define Color_Red "\33[0:31m" // Color Start
#define Color_Yellow "\33[0:32m"
#define Color_End "\33[0m" // To flush out prev settings
#define Color_Purple "\33[0:95m" // To flush out prev settings
#define MULTIPLAYER_EXIT "exit"
#define MULTIPLAYER_LOADING_BAR 6,666666666666667

struct Animal;
struct Player;
struct Animal *animalTab;
struct Player *playerTab;
char *animalType;

struct Player {

    char nom[150];
    bool isEnemy;
    int score;

};

enum Type {

    ELEPHANT = 'E',
    LION = 'L',
    TIGRE = 'T',
    PANTHERE = 'P',
    CHIEN = 'C',
    LOUP = 'O',
    CHAT = 'H',
    RAT = 'R'

};

enum Zone {

    PIEGE_BLEU,
    PIEGE_ROUGE,
    SANCTUAIRE_BLEU,
    SANCTUAIRE_ROUGE,
    LAC,
    AUCUNE

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

void afficherEchiquier();

void color(int t, int f);

bool searchCanEat(struct Animal animal, bool isEnemy, struct Player *playerTab);

bool hasRat(struct Animal, char direction);

bool checkEat(struct Animal *enemy, struct Animal ally, struct Player *playerTab);

void GenererEchequier();

enum Zone checkZone(struct Animal animal);

void loadGame();

#endif //PROJETJUNGLE_ECHIQUIER_H
