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
bool deplacerPion(char animal, char vector, bool isAlive);
void GenererEchequier();
void loadGame();

#endif //PROJETJUNGLE_ECHEQUIER_H
