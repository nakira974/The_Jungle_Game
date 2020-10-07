#include <stdbool.h>
#include <stdio.h>
#include "verifications.h"


typedef struct Animal Animal;
//typedef enum Type Type;
char coord[7][9];
int i, j;

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

struct Animal{

    char type;
    int x;
    int y;

};

void main() {

    Animal animal;
    animal.type = LION;
    animal.x = 7;
    animal.y = 9;


    while (true) {

        printf("Bonjour à toi joueur intrépide, dit moi ton nom\n");

        for(i = 0; i < 9; i++)
        {
            for(j = 0; j < 7; j++)
            {
                //scanf("%d", coord[i][j]);
                coord[i][j] = animal.type;
            }
        }

        printf(" Le tableau 2D = \n");
        for(i=0; i < 9; i++)
        {
            for(j = 0; j < 7; j++)
            {
                printf("%c  |  ", coord[i][j]);
            }
            printf("\n");
            printf("________________________________________\n");
        }

        printf("Type: %c, %i, %i\n", animal.type, animal.x, animal.y);
        return;

    }
}