#include <stdbool.h>
#include <stdio.h>
#include "verifications.h"


typedef struct Animal Animal;

struct Animal{

    char type;
    int x;
    int y;

};

void main() {

    Animal animal;

    animal.type = 'E';
    animal.x = 7;
    animal.y = 9;


    while (true) {

        printf("Bonjour à toi joueur intrépide, dit moi ton nom");



        printf("Type: %c, %i, %i\n", animal.type, animal.x, animal.y);
        return;

    }
}