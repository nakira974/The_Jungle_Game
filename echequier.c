//
// Created by maxime on 07/10/2020.
//

#include "echequier.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#define Color_Blue "\33[0:34m" // Color Start
#define Color_Red "\33[0:31m" // Color Start
#define Color_Yellow "\33[0:32m"
#define Color_End "\33[0m" // To flush out prev settings



typedef struct Animal Animal;
//typedef struct Echiquier Echiquier;
Animal* animalTab=NULL;
char* animalType=NULL;
//typedef enum Type Type;
char coord[9][7];




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
    bool isEnemy;

};

void affichage() {

    int i;


    for (i = 0; i < sizeof(*animalTab); i++) {

        printf("%c", animalTab[i].type);

    }
}


void setCoord(){
    int m;

    for(m=0;m<sizeof(*animalTab);m++){
        if(!animalTab[m].isEnemy) {
            if (animalTab[m].type == ELEPHANT) {
                animalTab[m].x = 2;
                animalTab[m].y = 6;
            }
            if (animalTab[m].type == LION) {
                animalTab[m].x = 0;
                animalTab[m].y = 0;
            }
            if (animalTab[m].type == TIGRE) {
                animalTab[m].x = 0;
                animalTab[m].y = 6;
            }
            if (animalTab[m].type == PANTHERE) {
                animalTab[m].x = 2;
                animalTab[m].y = 2;
            }
            if (animalTab[m].type == CHIEN) {
                animalTab[m].x = 1;
                animalTab[m].y = 1;
            }
            if (animalTab[m].type == LOUP) {
                animalTab[m].x = 2;
                animalTab[m].y = 4;
            }
            if (animalTab[m].type == CHAT) {
                animalTab[m].x = 1;
                animalTab[m].y = 5;
            }
            if (animalTab[m].type == RAT) {
                animalTab[m].x = 2;
                animalTab[m].y = 0;
            }

        }else{
            if (animalTab[m].type == ELEPHANT) {
                animalTab[m].x = 6;
                animalTab[m].y = 0;
            }
            if (animalTab[m].type == LION) {
                animalTab[m].x = 8;
                animalTab[m].y = 6;
            }
            if (animalTab[m].type == TIGRE) {
                animalTab[m].x = 8;
                animalTab[m].y = 0;
            }
            if (animalTab[m].type == PANTHERE) {
                animalTab[m].x = 6;
                animalTab[m].y = 4;
            }
            if (animalTab[m].type == CHIEN) {
                animalTab[m].x = 7;
                animalTab[m].y = 5;
            }
            if (animalTab[m].type == LOUP) {
                animalTab[m].x = 6;
                animalTab[m].y = 2;
            }
            if (animalTab[m].type == CHAT) {
                animalTab[m].x = 7;
                animalTab[m].y = 1;
            }
            if (animalTab[m].type == RAT) {
                animalTab[m].x = 6;
                animalTab[m].y = 6;
            }


        }

    }

    //printf("%s%s%s",Color_Yellow,test,Color_End);
    //coord[3][1]=test;

}

void GenererEchequier(){

    Animal animal;
    /*animal.type = NULL;
    animal.x = 0;
    animal.y = NULL;
    animal.isEnnemy = NULL;*/

    char nomJoueur[150];

    int i,j,l,m,k;

    animalTab= malloc(16 * sizeof(Animal));//Création du tableau d'objets<Animal>

    animalType=malloc(8 * sizeof(char));//nos types d'animaux
    animalType[0]=ELEPHANT;
    animalType[1]=LION;
    animalType[2]=TIGRE;
    animalType[3]=PANTHERE;
    animalType[4]=CHIEN;
    animalType[5]=LOUP;
    animalType[6]=CHAT;
    animalType[7]=RAT;

    for (l = 0; l < 16 ; l++) {//1er joueur

        if(l<8) {
            animal.isEnemy = false;
            animal.type = animalType[l];
            animalTab[l] = animal;
        }else{

            //second joueur
        animal.isEnemy = true;
        animal.type = animalType[l-8];
        animalTab[l] = animal;
    }
    }

    setCoord();

    for(m=0; m < sizeof(*animalTab); m++){
        for(k=0; k<sizeof(*coord); k++){
            int x,y;
            x=animalTab[m].x;
            y=animalTab[m].y;
            coord[x][y]=animalTab[m].type;
        }
    }


    /*affichage();

      for(i = 0; i < 9; i++)
   {
       for(j = 0; j < 7; j++)
       {

           coord[i][j] = animal.type;
       }
   }*/




       /* printf("Bonjour à toi joueur intrépide, dit moi ton nom\n");
        printf("Pseudo : ");
        scanf("%s", &nomJoueur);*/

        printf(" Le tableau 2D = \n");
    printf("___________________________________________\n");
    printf("|  ");
        for(i=0; i < 9; i++)
        {
            for(j = 0; j < 7; j++)
            {
                char* filled_square = u8"\u2588";

                if(coord[i][j]==0){
                    if(i==3 && j==1 || i==3 && j==2 || i==4 && j==1 || i==4 && j==2 || i==5 && j==1 || i==5 && j==2 || i==3 && j==4 || i==3 && j==5 || i==4 && j==4 || i==4 && j==5 || i==5 && j==4 || i==5 && j==5){
                        printf("%s%s%s",Color_Yellow,filled_square,Color_End);
                    }else if(i==0 && j==2 || i==0 && j==3 || i==0 && j==4 || i==1 && j==3) {
                        printf("%s%s%s", Color_Red, filled_square, Color_End);
                    }else if(i==8 && j==2 || i==8 && j==3 || i==8 && j==4 || i==7 && j==3){
                            printf("%s%s%s",Color_Blue,filled_square,Color_End);
                        }else
                    {
                        printf("   |  ");
                        continue;
                    }

                }else{
                    printf("%c", coord[i][j]);
                }
                printf("  |  ");
            }
            printf("\n");
            printf("___________________________________________\n");
            printf("|  ");

        }
    printf("\r \n");
    printf("Affiché!");







    bool color = true;

        animal.type = animalType[0];
        animal.x = 2;
        animal.y = 5;
        animal.isEnemy = false;
        coord[animal.x][animal.y]=animal.type;



    /*if(color==true) {
        color = false;

        coord[animal.x][animal.y]=animal.type;


}else{
color=true;

}

        int x,y;
    printf("Entrez la ligne:\n");
    scanf("%d", &x);
    printf("Entrez la colonne:\n");
    scanf("%d", &y);
    //Deplacement(x, y, );*/

       // printf("Type: %c, %i, %i\n", animal.type, animal.x, animal.y);



}



int Deplacement(int x, int y, int x_dest, int y_dest, bool isEnemy){
    int i;

            for(i=0;i<sizeof(*animalTab);i++){

                if(!animalTab[i].isEnemy){

                    animalTab[i].x = x;
                    animalTab[i].y = y;
                    //coord[x][y]=animalTab[]

                }

            }






    return 0;
}