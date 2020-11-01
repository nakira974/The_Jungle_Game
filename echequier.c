//
// Created by maxime on 07/10/2020.
//
#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

#include "echequier.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <memory.h>
#include <conio.h>

#define Color_Blue "\33[0:34m" // Color Start
#define Color_Red "\33[0:31m" // Color Start
#define Color_Yellow "\33[0:32m"
#define Color_End "\33[0m" // To flush out prev settings


typedef struct Animal Animal;
typedef struct Player Player;
Animal* animalTab=NULL;
char* animalType=NULL;
Player* playerTab=NULL;
int player_Count = 2;
int animal_Count = 16;
char coord[9][7];
FILE *fichier;

struct Player{

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

struct Animal{

    char type;
    int x;
    int y;
    bool isEnemy;
    bool isAlive;
    int score;

};

bool readSave(){

    fichier = fopen ("save.txt", "rb");

    if (!fichier) {
        return false;
    }
    int m;
    playerTab = malloc(2 * sizeof(Player));//nos types d'animaux
    fscanf(fichier, "%s", playerTab[0].nom);
    fseek(fichier, 1, SEEK_CUR);
    fscanf(fichier, "%s", playerTab[1].nom);
    fseek(fichier, 1, SEEK_CUR);
    for (m = 0; m < animal_Count; m++) {
        //on récupère les infos des animaux
        fscanf(fichier, "%c: %i, %i, %d, %d", &animalTab[m].type, &animalTab[m].x, &animalTab[m].y, &animalTab[m].isEnemy, &animalTab[m].isAlive);
        //on saute chaque ligne
        fseek(fichier, 1, SEEK_CUR);
    }
    fclose (fichier);
    return true;
}

bool writeSave(Animal* animalT){

    //remove("save.txt");
    fichier = fopen ("save.txt", "wb");
    if (!fichier) {
        return false;
    }

int m;
    fprintf(fichier, "%s\n", playerTab[0].nom);
    fprintf(fichier, "%s\n", playerTab[1].nom);
    for (m = 0; m < animal_Count; m++) {

        //on sauvegarde les infos de nos animaux
            fprintf(fichier, "%c: ", animalT[m].type);
            fprintf(fichier, "%i, ", animalT[m].x);
            fprintf(fichier, "%i, ", animalT[m].y);
            fprintf(fichier, "%d, ", animalT[m].isEnemy);
            //on saute une ligne à la fin pour passer à l'animal suivant
            fprintf(fichier, "%d\n", animalT[m].isAlive);
    }
    fclose (fichier);
    return true;
}

bool whoisBetter(Animal attacker, Animal defender) {

    if (attacker.x && attacker.y == defender.x && defender.y) {
        if (defender.type == ELEPHANT && attacker.type != ELEPHANT) {
            return false;
        } else if (defender.type == LION && attacker.type != ELEPHANT || attacker.type != LION) {
            return false;
        } else if (defender.type == TIGRE && attacker.type != TIGRE || attacker.type != ELEPHANT || attacker.type != LION) {
            return false;
        } else if (defender.type == PANTHERE && attacker.type != PANTHERE || attacker.type != TIGRE || attacker.type != ELEPHANT || attacker.type != LION) {
            return false;
        } else if (defender.type == CHIEN && attacker.type != CHIEN || attacker.type != ELEPHANT || attacker.type != LION || attacker.type != TIGRE || attacker.type != PANTHERE) {
            return false;
        } else if (defender.type == LOUP && attacker.type != LOUP || attacker.type != CHIEN|| attacker.type != LION || attacker.type != TIGRE || attacker.type != PANTHERE|| attacker.type != ELEPHANT) {
            return false;
        } else if (defender.type == CHAT && attacker.type != CHAT || attacker.type != LOUP || attacker.type != CHIEN || attacker.type != LION || attacker.type != TIGRE || attacker.type != PANTHERE || attacker.type != ELEPHANT) {
            return false;
        } else if (defender.type == RAT && attacker.type == RAT || attacker.type != RAT) {
            return true;
        } else { return true; }

    }
}

bool deplacerPion(char animal, char vector, bool player) {

    int i, j;

    for (i = 0; i < 8; i++) {
        if (!player) {// Joueur 1
            if (animal == animalTab[i].type && animalTab[i].isAlive && !animalTab[i].isEnemy) {// Verifications
                for (j = 0; j < 8; j++) {// On parcourt l'ennemi
                    if (vector == 'H') {//SI VECTOR == 0 ALORS X +1
                        animalTab[i].x + 1;
                        if (!whoisBetter(animalTab[j], animalTab[i])) {
                            &animalTab[i].x - 1;
                            return false;
                        }
                    } else if (vector == 'D') {
                        &animalTab[i].y + 1;
                        if (!whoisBetter(animalTab[j], animalTab[i])) {
                            &animalTab[i].y - 1;
                            return false;
                        }
                    } else if (vector == 'G') {
                        animalTab[i].y - 1;
                        if (!whoisBetter(animalTab[j], animalTab[i])) {
                            animalTab[i].y + 1;
                            return false;
                        }
                    }
                    if (&animalTab[j].x == &animalTab[i].x &&
                        &animalTab[j].y == &animalTab[i].y)// On cherche si un pion ennemi est sur la même case
                        //On vérifie que pendant notre déplacement si on peut tuer quelqu'un
                        animalTab[j].isAlive = false;// On le tue
                    return true;
                }
            }
        }
    }
}

void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;

    if (fgets(chaine, longueur, stdin) != NULL)
    {
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL)
        {
            *positionEntree = '\0';
        }
        else
        {
            viderBuffer();
        }
        return 1;
    }
    else
    {
        viderBuffer();
        return 0;
    }
}

void loadGame(){

    Player player;
    Animal animal;
    char type, direction;

    while (playerTab == NULL){

        playerTab = malloc(2 * sizeof(Player));//nos types d'animaux

            printf("Entrez le nom du premier joueur:\n");
            lire(playerTab[0].nom, 150);
            printf("Nom du premier joueur: %s\n", playerTab[0].nom);
            printf("Entrez le nom du second joueur:\n");
            lire(playerTab[1].nom, 150);
            printf("Nom du second joueur: %s\n", playerTab[1].nom);


        }

        for (int i = 0; i < player_Count; ++i) {

                player = playerTab[i];
                printf("C'est au tour de: %s\n", player.nom);

                    do {
                    printf("Choisissez votre pion: ");
                    scanf("%c", &type);
                }while(type == 0);


                    animal.type = type;
                    animal.isEnemy = true;
                    animal.isAlive = true;
            do {
                printf("Choisissez votre direction: ");
                scanf("%c", &direction);
            }while(direction == 0);

                    /*switch(getch()) {
                        case 65:
                            printf("Up");
                            break;
                        case 66:
                            printf("Up");
                            break;
                        case 67:
                            printf("Up");
                            break;
                        case 68:
                            printf("Up");
                            break;
                    }*/

        }

}

void setCoord(){
    int m;

    for(m=0;m<animal_Count;m++){
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

}

void afficherEchiquier() {

    int i, j, m;

    printf(" Jeu de la Jungle \n");
    printf("___________________________________________\n");
    printf("|  ");
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 7; j++) {


#ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            WORD saved_attributes;
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
            saved_attributes = consoleInfo.wAttributes;
            if (coord[i][j] == 0) {

                _setmode(_fileno(stdout), _O_U16TEXT);
                if (i == 3 && j == 1 || i == 3 && j == 2 || i == 4 && j == 1 || i == 4 && j == 2 ||
                    i == 5 && j == 1 || i == 5 && j == 2 || i == 3 && j == 4 || i == 3 && j == 5 ||
                    i == 4 && j == 4 || i == 4 && j == 5 || i == 5 && j == 4 || i == 5 && j == 5) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                    wprintf(L"\x2588");
                } else if (i == 0 && j == 2 || i == 0 && j == 3 || i == 0 && j == 4 || i == 1 && j == 3) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                    if(i == 1 && j == 3){
                        wprintf(L"\x271D");
                    }else {
                        wprintf(L"\x2588");
                    }
                } else if (i == 8 && j == 2 || i == 8 && j == 3 || i == 8 && j == 4 || i == 7 && j == 3) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
                    if(i == 7 && j == 3){
                        wprintf(L"\x271D");
                    }else {
                        wprintf(L"\x2588");
                    }
                }else {
                    SetConsoleTextAttribute(hConsole, saved_attributes);
                    _setmode(_fileno(stdout), _O_TEXT);
                    printf("   |  ");
                    continue;
                }
                SetConsoleTextAttribute(hConsole, saved_attributes);
                _setmode(_fileno(stdout), _O_TEXT);
            } else {
                //utiliser le meme prototype que ci-dessous pour l'affichage des pions
                for (m = 0; m < animal_Count; m++) {
                    if (animalTab[m].x == i && animalTab[m].y == j) {
                        //Enemy = Blue Team
                        if (animalTab[m].isAlive){
                        if (animalTab[m].isEnemy) {
                            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
                            printf("%c", coord[i][j]);
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                        } else {
                            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                            printf("%c", coord[i][j]);
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                        }
                        }

                    }
                }

            }
#else
            if (coord[i][j] == 0) {
            char *filled_square = u8"\u2588";
            char *sanctuary = u8"\u271D";
                if (i == 3 && j == 1 || i == 3 && j == 2 || i == 4 && j == 1 || i == 4 && j == 2 ||
                    i == 5 && j == 1 || i == 5 && j == 2 || i == 3 && j == 4 || i == 3 && j == 5 ||
                    i == 4 && j == 4 || i == 4 && j == 5 || i == 5 && j == 4 || i == 5 && j == 5) {

                    printf("%s%s%s", Color_Yellow, filled_square, Color_End);

                } else if (i == 0 && j == 2 || i == 0 && j == 3 || i == 0 && j == 4 || i == 1 && j == 3) {
                    if(i == 1 && j == 3){
                        printf("%s%s%s", Color_Red, sanctuary, Color_End);
                    }else {
                        printf("%s%s%s", Color_Red, filled_square, Color_End);
                    }
                } else if (i == 8 && j == 2 || i == 8 && j == 3 || i == 8 && j == 4 || i == 7 && j == 3) {
                   if(i == 7 && j == 3){
                        printf("%s%s%s", Color_Blue, sanctuary, Color_End);
                    }else {
                        printf("%s%s%s", Color_Blue, filled_square, Color_End);
                    }
                }else {
                    printf("   |  ");
                    continue;
                }
             } else {

                //utiliser le meme prototype que ci-dessous pour l'affichage des pions
                for (m = 0; m < animal_Count; m++) {
                    if (animalTab[m].x == i && animalTab[m].y == j) {
                        //Enemy = Blue Team
                        if (animalTab[m].isAlive){
                            if (animalTab[m].isEnemy) {
                                printf("%s%c%s", Color_Blue, coord[i][j], Color_End);
                            } else {
                                printf("%s%c%s", Color_Red, coord[i][j], Color_End);
                            }
                    }

                    }
                }

            }
#endif

            printf("  |  ");
        }
        printf("\n");
        printf("___________________________________________\n");
        printf("|  ");

    }
    printf("\r \n");
}

void GenererEchequier() {
    setlocale(LC_ALL, "fr_FR.UTF-8");

    Animal animal;
    int l;

    animalTab = malloc(16 * sizeof(Animal));//Création du tableau d'objets<Animal>
    animalType = malloc(8 * sizeof(char));//nos types d'animaux

    animalType[0] = ELEPHANT;
    animalType[1] = LION;
    animalType[2] = TIGRE;
    animalType[3] = PANTHERE;
    animalType[4] = CHIEN;
    animalType[5] = LOUP;
    animalType[6] = CHAT;
    animalType[7] = RAT;

   if (!readSave()) {

       playerTab = NULL;
       for (l = 0; l < 16; l++) {//1er joueur

           if (l < 8) {
               animal.isEnemy = false;
               animal.isAlive = true;
               animal.type = animalType[l];
               animalTab[l] = animal;
           } else {

               //second joueur
               animal.isEnemy = true;
               animal.isAlive = true;
               animal.type = animalType[l - 8];
               animalTab[l] = animal;
           }
       }
        setCoord();

       int x, y, m;
       //utiliser le meme prototype que ci-dessous pour les déplacement
       for (m = 0; m < animal_Count; m++) {
                   x = animalTab[m].x;
                   y = animalTab[m].y;
                   coord[x][y] = animalTab[m].type;
       }
    }else{

       int x, y, m;
       //utiliser le meme prototype que ci-dessous pour les déplacement
       for (m = 0; m < animal_Count; m++) {
           x = animalTab[m].x;
           y = animalTab[m].y;
           coord[x][y] = animalTab[m].type;
       }

   }


    afficherEchiquier();
    loadGame();
    writeSave(animalTab);

}