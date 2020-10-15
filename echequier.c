//
// Created by maxime on 07/10/2020.
//

#include "echequier.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Animal Animal;
//typedef enum Type Type;
char coord[7][9];
Animal* animalTab=NULL;
char* animalType;



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
    bool isEnnemy;

};

void affichage() {

    int i;


    for (i = 0; i < sizeof(&animalTab); i++) {

        printf("%s", &animalTab[i].type);

    }
}


char GenererEchequier(){

    char nomJoueur[150];

    int i,j,k,l;

    animalTab= malloc(16 * sizeof(Animal));//Création du tableau d'objets<Animal>

    animalType=malloc(16 * sizeof(char));//nos types d'animaux
    animalType[0]=ELEPHANT;
    animalType[1]=LION;
    animalType[2]=TIGRE;
    animalType[3]=PANTHERE;
    animalType[4]=CHIEN;
    animalType[5]=LOUP;
    animalType[6]=CHAT;
    animalType[7]=RAT;

    for (l = 0; l < 7 ; l++) {//1er joueur
        Animal animal;
        animal.isEnnemy=false;
        animal.type=animalType[l];
        animalTab[l]=animal;
    }
    /*for (k = 0; k < 7; ++k) {
        Animal animal;
        animal.isEnnemy=true;
        animal.type=animalType[k];
        animalTab[k]=animal;
    }*/

    affichage();
/*

    while (true) {

        printf("Bonjour à toi joueur intrépide, dit moi ton nom\n");
        printf("Pseudo : ");
        scanf("%s", &nomJoueur);

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

       // printf("Type: %c, %i, %i\n", animal.type, animal.x, animal.y);
        return true;

    }*/


}




  /*  for(i = 0; i < 9; i++)
    {
        for(j = 0; j < 7; j++)
        {


            //scanf("%d", coord[i][j]);
            //coord[i][j] = animal.type;
        }
    }*/



int Deplacement(Animal *p_animal){

    return 0;
}