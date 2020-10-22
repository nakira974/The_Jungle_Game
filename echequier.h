//
// Created by maxime on 07/10/2020.
//

#ifndef PROJETJUNGLE_ECHEQUIER_H
#define PROJETJUNGLE_ECHEQUIER_H

#include <stdbool.h>

struct Animal;
struct Player;
struct Animal* animalTab;
struct Player* playerTab;
char* animalType;
int deplacer_Pion(int x, int y, int x_dest, int y_dest, bool isEnemy);
void GenererEchequier();
void loadGame();

#endif //PROJETJUNGLE_ECHEQUIER_H
