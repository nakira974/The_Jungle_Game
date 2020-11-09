//
// Created by maxime on 07/10/2020.
//
#ifndef PROJETJUNGLE_ECHIQUIER_H
#define PROJETJUNGLE_ECHIQUIER_H

#include <stdbool.h>

struct Animal;
struct Player;
struct Animal* animalTab;
struct Player* playerTab;
char* animalType;
void afficherEchiquier();
void color(int t, int f);
bool searchCanEat(struct Animal animal, char direction, bool isEnemy);
bool checkEat(struct Animal *enemy, struct Animal ally);
void GenererEchequier();
enum Zone checkZone(struct Animal animal);
void loadGame();

#endif //PROJETJUNGLE_ECHIQUIER_H
