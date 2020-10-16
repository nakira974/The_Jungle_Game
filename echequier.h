//
// Created by maxime on 07/10/2020.
//

#ifndef PROJETJUNGLE_ECHEQUIER_H
#define PROJETJUNGLE_ECHEQUIER_H

#include <stdbool.h>

struct Animal;
struct Echequier;
struct Animal* animalTab;
char* animalType;
int Deplacement(int x, int y, int x_dest, int y_dest, bool isEnemy);
void GenererEchequier();
#endif //PROJETJUNGLE_ECHEQUIER_H
