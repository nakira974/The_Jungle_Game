//
// Created by maxime on 07/10/2020.
//
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <conio.h>
#else
#include <memory.h>
#endif

#include "echiquier.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define Color_Blue "\33[0:34m" // Color Start
#define Color_Red "\33[0:31m" // Color Start
#define Color_Yellow "\33[0:32m"
#define Color_End "\33[0m" // To flush out prev settings
#define Color_Purple "\33[0:95m" // To flush out prev settings

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

enum Zone {

    PIEGE_BLEU,
    PIEGE_ROUGE,
    SANCTUAIRE_BLEU,
    SANCTUAIRE_ROUGE,
    LAC,
    AUCUNE

};

struct Animal{

    char type;
    int x;
    int y;
    bool isEnemy;
    bool isAlive;
    bool canEat;
    int index;
    enum Zone zone;

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
        fscanf(fichier, "%c: %i, %i, %d, %d, %i, %i", &animalTab[m].type, &animalTab[m].x, &animalTab[m].y, &animalTab[m].isEnemy, &animalTab[m].isAlive, &animalTab[m].index, &animalTab[m].zone);
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
            fprintf(fichier, "%d, ", animalT[m].isAlive);
        fprintf(fichier, "%i, ", animalT[m].index);
        fprintf(fichier, "%i\n", animalT[m].zone);
        //on saute une ligne à la fin pour passer à l'animal suivant
    }
    fclose (fichier);
    return true;
}

#ifdef _WIN32
void color(int t,int f)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,f*16+t);

}
#endif

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

void loadGame() {

    Player player;
    Animal animal;
    char type;
    char direction;
    bool turn = false;

    while (playerTab == NULL) {

        playerTab = malloc(2 * sizeof(Player));//nos types d'animaux

        printf("Entrez le nom du premier joueur:\n");
        lire(playerTab[0].nom, 150);
        printf("Nom du premier joueur: %s\n", playerTab[0].nom);
        printf("Entrez le nom du second joueur:\n");
        lire(playerTab[1].nom, 150);
        printf("Nom du second joueur: %s\n", playerTab[1].nom);


    }


    do {

        for (int i = 0; i < player_Count; ++i) {

            player = playerTab[i];
            printf("\nC'est au tour de: %s\n", player.nom);

            do {
                printf("Choisissez votre pion: ");
                scanf("%c", &type);
            } while (type == 10);
            viderBuffer();

            if (turn == false) {
                turn = true;
                int nbb;
                animal.type = type;
                animal.isEnemy = true;
                animal.isAlive = true;
                for (nbb = 0; nbb < animal_Count; nbb++) {
                    if (animalTab[nbb].isEnemy == true && animalTab[nbb].type == type &&
                        animalTab[nbb].isAlive == true) {
                        animal.index = animalTab[nbb].index;
                        animal.x = animalTab[nbb].x;
                        animal.y = animalTab[nbb].y;
                        animal.zone = animalTab[nbb].zone;
                    }
                }
            } else {
                turn = false;
                int nbb;
                animal.type = type;
                animal.isEnemy = false;
                animal.isAlive = true;
                for (nbb = 0; nbb < animal_Count; nbb++) {
                    if (animalTab[nbb].isEnemy == false && animalTab[nbb].type == type &&
                        animalTab[nbb].isAlive == true) {
                        animal.index = animalTab[nbb].index;
                        animal.x = animalTab[nbb].x;
                        animal.y = animalTab[nbb].y;
                        animal.zone = animalTab[nbb].zone;
                    }
                }
            }

do{
            printf("Choisissez votre direction: ");
            scanf("%c", &direction);
        }
        while (direction == 10);
        viderBuffer();
        int nb;

        switch (direction) {

            case 'A':
                //printf("Avancer\n");
                for (nb = 0; nb < animal_Count; nb++) {
                    if (animalTab[nb].x == animal.x && animalTab[nb].y == animal.y &&
                        animalTab[nb].isAlive == true) {
                        if (animalTab[nb].isEnemy == true) {
                            if(animalTab[nb].x-1 < 9 && animalTab[nb].x-1 > -1) {
                                if (searchCanEat(animal, 'A', true)) {
                                    coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                    animalTab[nb].x = animal.x - 1;
                                }
                            }
                        } else {
                            if(animalTab[nb].x+1 < 9 && animalTab[nb].x+1 > -1) {
                                if (searchCanEat(animal, 'A', false)) {
                                    coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                    animalTab[nb].x = animal.x + 1;
                                }
                            }

                        }
                    }
                }
                break;
            case 'R':
                //printf("Reculer\n");
                for (nb = 0; nb < animal_Count; nb++) {
                    if (animalTab[nb].x == animal.x && animalTab[nb].y == animal.y &&
                        animalTab[nb].isAlive == true) {
                        if (animalTab[nb].isEnemy == true) {
                            if(animalTab[nb].x+1 < 9 && animalTab[nb].x+1 > -1) {
                                if (searchCanEat(animal, 'R', true)) {
                                    coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                    animalTab[nb].x = animal.x + 1;
                                }
                            }
                        } else {
                            if(animalTab[nb].x-1 < 9 && animalTab[nb].x-1 > -1) {
                                if (searchCanEat(animal, 'R', false)) {
                                    coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                    animalTab[nb].x = animal.x - 1;
                                }
                            }
                        }

                    }
                }
                break;
            case 'D':
                //printf("Droite\n");
                for (nb = 0; nb < animal_Count; nb++) {
                    if (animalTab[nb].x == animal.x && animalTab[nb].y == animal.y &&
                        animalTab[nb].isAlive == true) {
                        if (animalTab[nb].isEnemy == true) {
                            if(animalTab[nb].y+1 < 7 && animalTab[nb].y+1 > -1) {
                                if (searchCanEat(animal, 'D', true)) {
                                    coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                    animalTab[nb].y = animal.y + 1;
                                }
                            }
                        } else {
                            if(animalTab[nb].y-1 < 7 && animalTab[nb].y-1 > -1) {
                                if (searchCanEat(animal, 'D', false)) {
                                    coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                    animalTab[nb].y = animal.y - 1;
                                }
                            }

                        }
                    }
                }
                break;
            case 'G':
                //printf("Gauche\n");
                for (nb = 0; nb < animal_Count; nb++) {
                    if (animalTab[nb].x == animal.x && animalTab[nb].y == animal.y &&
                        animalTab[nb].isAlive == true) {
                            if (animalTab[nb].isEnemy == true) {
                                if(animalTab[nb].y-1 < 7 && animalTab[nb].y-1 > -1) {
                                    if (searchCanEat(animal, 'G', true)) {
                                        coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                        animalTab[nb].y = animal.y - 1;
                                    }
                                }
                            } else {
                                if(animalTab[nb].y+1 < 7 && animalTab[nb].y+1 > -1) {
                                    if (searchCanEat(animal, 'G', false)) {
                                        coord[animalTab[nb].x][animalTab[nb].y] = 0;
                                        animalTab[nb].y = animal.y + 1;
                                    }
                                }

                            }
                        }
                    }

                break;

            default:
                break;




        }

        afficherEchiquier();

    }

    printf("Voulez-vous continuer? (Oui: Entrer / Non: Echap)");
#ifdef _WIN32
    }while(getch()!=27);
#else
}while(getc(stdin)!=27);
#endif



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

enum Zone checkZone(Animal animal){


    int i, j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 7; j++) {

                if (animal.x == 3 && animal.y == 1 || animal.x == 3 && animal.y == 2 || animal.x == 4 && animal.y == 1 || animal.x == 4 && animal.y == 2 ||
                        animal.x == 5 && animal.y == 1 || animal.x == 5 && animal.y == 2 || animal.x == 3 && animal.y == 4 || animal.x == 3 && animal.y == 5 ||
                        animal.x == 4 && animal.y == 4 || animal.x == 4 && animal.y == 5 || animal.x == 5 && animal.y == 4 || animal.x == 5 && animal.y == 5) {
                    //zone lac
                    return LAC;
                } else if (animal.x == 0 && animal.y == 2 || animal.x == 0 && animal.y == 3 || animal.x == 0 && animal.y == 4 || animal.x == 1 && animal.y == 3) {
                    if(animal.x == 0 && animal.y == 3){
                       //zone sanctuaire rouge
                       return SANCTUAIRE_ROUGE;
                    }else {
                        //zone piege rouge
                        return PIEGE_ROUGE;
                    }
                } else if (animal.x == 8 && animal.y == 2 || animal.x == 8 && animal.y == 3 || animal.x == 8 && animal.y == 4 || animal.x == 7 && animal.y == 3) {
                    if(animal.x == 8 && animal.y == 3){
                        //zone sanctuaire bleu
                        return SANCTUAIRE_BLEU;
                    }else {
                        //zone piege bleu
                        return PIEGE_BLEU;
                    }
                }else {
                    return AUCUNE;
                }




        }
    }



}

void afficherEchiquier() {

    int i, j, m, b, x, y;

    //utiliser le meme prototype que ci-dessous pour les déplacement
    for (b = 0; b < animal_Count; b++) {
        if (animalTab[b].isAlive) {
        x = animalTab[b].x;
        y = animalTab[b].y;
        coord[x][y] = animalTab[b].type;
    }
    }
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf(" Jeu de la Jungle \n");
    printf("-----------------------------\n");
    printf("|");
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 7; j++) {


#ifdef _WIN32
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            WORD saved_attributes;
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
            saved_attributes = consoleInfo.wAttributes;
            if (coord[i][j] == 0) {
                if (i == 3 && j == 1 || i == 3 && j == 2 || i == 4 && j == 1 || i == 4 && j == 2 ||
                    i == 5 && j == 1 || i == 5 && j == 2 || i == 3 && j == 4 || i == 3 && j == 5 ||
                    i == 4 && j == 4 || i == 4 && j == 5 || i == 5 && j == 4 || i == 5 && j == 5) {
                    color(15, 2);
                    printf("   ");
                    SetConsoleTextAttribute(hConsole, saved_attributes);
                } else if (i == 0 && j == 2 || i == 0 && j == 3 || i == 0 && j == 4 || i == 1 && j == 3) {
                    if(i == 0 && j == 3){
                        color(15, 4);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    }else {
                        color(15, 6);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    }
                } else if (i == 8 && j == 2 || i == 8 && j == 3 || i == 8 && j == 4 || i == 7 && j == 3) {
                    if(i == 8 && j == 3){
                        color(15, 1);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    }else {
                        color(15, 6);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    }
                }else {
                    printf("   |");
                    continue;
                }
            } else {
                //utiliser le meme prototype que ci-dessous pour l'affichage des pions
                for (m = 0; m < animal_Count; m++) {
                    if (animalTab[m].x == i && animalTab[m].y == j) {
                        //Enemy = Blue Team
                        if (animalTab[m].isAlive){
                        if (animalTab[m].isEnemy) {
                            color(1, 0);
                            printf(" %c ", coord[i][j]);
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                        } else {
                            color(4, 0);
                            printf(" %c ", coord[i][j]);
                            SetConsoleTextAttribute(hConsole, saved_attributes);
                        }
                        }

                    }
                }

            }
#else
            if (coord[i][j] == 0) {
            char *filled_square = u8"\u2588";
            //char *sanctuary = u8"\u271D";
                if (i == 3 && j == 1 || i == 3 && j == 2 || i == 4 && j == 1 || i == 4 && j == 2 ||
                    i == 5 && j == 1 || i == 5 && j == 2 || i == 3 && j == 4 || i == 3 && j == 5 ||
                    i == 4 && j == 4 || i == 4 && j == 5 || i == 5 && j == 4 || i == 5 && j == 5) {

                    printf(" %s%s%s ", Color_Yellow, filled_square, Color_End);

                } else if (i == 0 && j == 2 || i == 0 && j == 3 || i == 0 && j == 4 || i == 1 && j == 3) {
                    if(i == 0 && j == 3){
                        printf(" %s%s%s ", Color_Red, filled_square, Color_End);
                    }else {
                        printf(" %s%s%s ", Color_Purple, filled_square, Color_End);
                    }
                } else if (i == 8 && j == 2 || i == 8 && j == 3 || i == 8 && j == 4 || i == 7 && j == 3) {
                   if(i == 8 && j == 3){
                        printf(" %s%s%s ", Color_Blue, filled_square, Color_End);
                    }else {
                        printf(" %s%s%s ", Color_Purple, filled_square, Color_End);
                    }
                }else {
                    printf("   |");
                    continue;
                }
             } else {

                //utiliser le meme prototype que ci-dessous pour l'affichage des pions
                for (m = 0; m < animal_Count; m++) {

                    if (animalTab[m].x == i && animalTab[m].y == j) {
                        //Enemy = Blue Team
                        if (animalTab[m].isAlive){
                            if (animalTab[m].isEnemy) {
                                printf(" %s%c%s ", Color_Blue, coord[i][j], Color_End);
                            } else {
                                printf(" %s%c%s ", Color_Red, coord[i][j], Color_End);
                            }
                    }

                    }
                }

            }
#endif

            printf("|");
        }
        printf("\n-----------------------------\n");
        printf("|");

    }
    printf("\r \n");
}

bool checkEat(Animal *enemy, Animal ally){

    //faire les comparaisons

    switch(checkZone(*enemy)){

        case SANCTUAIRE_BLEU:
            break;
        case SANCTUAIRE_ROUGE:
            break;
        case PIEGE_BLEU:
            break;
        case PIEGE_ROUGE:
            break;
        case LAC:
            break;
        case AUCUNE:
            break;
        default:
            break;

    }

    switch(checkZone(ally)){

        case SANCTUAIRE_BLEU:
            break;
        case SANCTUAIRE_ROUGE:
            break;
        case PIEGE_BLEU:
            break;
        case PIEGE_ROUGE:
            break;
        case LAC:
            break;
        case AUCUNE:
            break;
        default:
            break;

    }

    if(enemy->zone == checkZone(*enemy) && ally.zone == checkZone(ally)){



    }

    if(enemy->isEnemy != ally.isEnemy) {


            if (enemy->index >= ally.index) {
                enemy->isAlive = false;
                coord[enemy->x][enemy->y] = 0;
                return true;

            } else if (enemy->type == 'E' && ally.type == 'R') {

                enemy->isAlive = false;
                coord[enemy->x][enemy->y] = 0;
                return true;

            } else {
                return false;
            }

    }else{
        return false;
    }

}

bool searchCanEat(Animal animal, char direction, bool isEnemy){

    int b, u;
        //refaire même système que le déplacement
        switch(direction){

            case 'A':
                if(isEnemy){
                    animal.x = animal.x - 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(!animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }else{
                    animal.x = animal.x + 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }
                break;

            case 'R':
                if(isEnemy){
                    animal.x = animal.x + 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(!animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }else{
                    animal.x = animal.x - 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }
                break;

            case 'D':
                if(isEnemy){
                    animal.y = animal.y + 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(!animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }else{
                    animal.y = animal.y - 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }
                break;

            case 'G':
                if(isEnemy){
                    animal.y = animal.y - 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(!animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }else{
                    animal.y = animal.y + 1;
                    for (b = 0; b < animal_Count; b++) {
                        if(animalTab[b].x == animal.x && animalTab[b].y == animal.y && animalTab[b].isAlive){
                            if(animalTab[b].isEnemy) {
                                if (checkEat(&animalTab[b], animal)) {
                                    return true;

                                } else {
                                    return false;
                                }
                            }else{
                                return false;
                            }
                        }
                    }
                }
                break;

            default:
                break;

        }


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
               animal.index = l;
               animal.type = animalType[l];
               animal.zone = AUCUNE;
               animalTab[l] = animal;
           } else {

               //second joueur
               animal.isEnemy = true;
               animal.isAlive = true;
               animal.index = l-8;
               animal.type = animalType[l - 8];
               animal.zone = AUCUNE;
               animalTab[l] = animal;
           }
       }
        setCoord();

   }

    afficherEchiquier();
    loadGame();
    writeSave(animalTab);

}