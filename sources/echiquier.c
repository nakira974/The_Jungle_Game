//
// Created by maxime on 07/10/2020.
//

#include "../headers/echiquier.h"


typedef struct Animal Animal;
typedef struct Player Player;

struct Animal *animals = NULL;
struct Player *players = NULL ;
char *animalType = NULL;

int player_Count = 2;
int animal_Count = 16;
char coordinates[9][7];
FILE *save_rolling_file;
bool isWinner = false;

bool readSave() {

    // Remplacé le 21/11/2020 pour test
    save_rolling_file = fopen("save.txt", "rb");
    //save_rolling_file = fdopen("save.txt", "rb");

    if (!save_rolling_file) {
        return false;
    }
    int m;
    players = malloc(2 * sizeof(Player));
    fscanf(save_rolling_file, "Joueur 1: name=%s", players[0].name);
    fseek(save_rolling_file, 1, SEEK_CUR);
    fscanf(save_rolling_file, "enemy=%i", &players[0].isEnemy);
    fseek(save_rolling_file, 1, SEEK_CUR);
    fscanf(save_rolling_file, "score=%i", &players[0].score);
    fseek(save_rolling_file, 1, SEEK_CUR);
    fscanf(save_rolling_file, "Joueur 2: name=%s", players[1].name);
    fseek(save_rolling_file, 1, SEEK_CUR);
    fscanf(save_rolling_file, "enemy=%i", &players[1].isEnemy);
    fseek(save_rolling_file, 1, SEEK_CUR);
    fscanf(save_rolling_file, "score=%i", &players[1].score);
    fseek(save_rolling_file, 1, SEEK_CUR);
    for (m = 0; m < animal_Count; m++) {
        //on récupère les infos des animaux
        fscanf(save_rolling_file, "type=%c: x=%i, y=%i, enemy=%d, alive=%d, eat=%d, index=%i, zone=%i", &animals[m].type,
               &animals[m].x, &animals[m].y, &animals[m].isEnemy, &animals[m].isAlive, &animals[m].canEat,
               &animals[m].index, &animals[m].zone);
        //on saute chaque ligne
        fseek(save_rolling_file, 1, SEEK_CUR);
    }
    fclose(save_rolling_file);
    return true;
}

bool writeSave(Animal *animalT) {

    //remove("save.txt");
    save_rolling_file = fopen("save.txt", "wb");
    if (!save_rolling_file) {
        return false;
    }

    int m;
    fprintf(save_rolling_file, "Joueur 1: name=%s\n", players[0].name);
    fprintf(save_rolling_file, "enemy=%i\n", players[0].isEnemy);
    fprintf(save_rolling_file, "score=%i\n", players[0].score);
    fprintf(save_rolling_file, "Joueur 2: name=%s\n", players[1].name);
    fprintf(save_rolling_file, "enemy=%i\n", players[1].isEnemy);
    fprintf(save_rolling_file, "score=%i\n", players[1].score);
    for (m = 0; m < animal_Count; m++) {

        //on sauvegarde les infos de nos animaux
        fprintf(save_rolling_file, "type=%c: ", animalT[m].type);
        fprintf(save_rolling_file, "x=%i, ", animalT[m].x);
        fprintf(save_rolling_file, "y=%i, ", animalT[m].y);
        fprintf(save_rolling_file, "enemy=%d, ", animalT[m].isEnemy);
        fprintf(save_rolling_file, "alive=%d, ", animalT[m].isAlive);
        fprintf(save_rolling_file, "eat=%d, ", animalT[m].canEat);
        fprintf(save_rolling_file, "index=%i, ", animalT[m].index);
        fprintf(save_rolling_file, "zone=%i\n", animalT[m].zone);
        //on saute une ligne à la fin pour passer à l'animal suivant
    }
    fclose(save_rolling_file);
    return true;
}

#ifdef _WIN32

void color(int t, int f) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, f * 16 + t);

}

#endif

void clearBuffer() {
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

int readString(char *chaine, int longueur) {
    char *positionEntree = NULL;

    if (fgets(chaine, longueur, stdin) != NULL) {
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL) {
            *positionEntree = '\0';
        } else {
            clearBuffer();
        }
        return 1;
    } else {
        clearBuffer();
        return 0;
    }
}

void loadGame() {

    Player player;
    Animal animal;
    char type;
    char direction;
    char gameType;


    do {

        //printf("Voulez-vous jouer en solo(1) ou en multijoueur(2) ?:\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("   ////   //   //    ///    //  //////   //      //// ");
        printf("   ///   //    //   // // //  //        //     //     ");
        printf("   ///   //   //  //  // //  //   ///  //      //     ");
        printf("/////     /////   //   ///   //////    //////  /////  ");
        printf("\n");
        printf("\n");
        printf("             //////     //     //   ///    ////       ");
        printf("           //         // //   //// ////  //           ");
        printf("          //   ///    /////   // //  //  //           ");
        printf("          //////     //  //  //      //  /////        ");
        printf("\n");
        printf("\n");
        printf("                      [1] Solo Game                   ");
        printf("\n");
        printf("                      [2] Multiplayer Game            ");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        printf("\n");
        scanf("%c", &gameType);

    } while (strlen(&gameType) == 0);

    clearBuffer();

    if (gameType == '1') {
        printChessTable();
        //solo

        while (players == NULL) {

            players = malloc(2 * sizeof(Player));//nos types d'animaux

            printf("Entrez le nom du premier joueur:\n");
            readString(players[0].name, 150);
            players[0].score = 0;
            players[0].isEnemy = true;
            printf("Nom du premier joueur: %s\n", players[0].name);
            printf("Entrez le nom du second joueur:\n");
            readString(players[1].name, 150);
            players[1].score = 0;
            players[1].isEnemy = false;
            printf("Nom du second joueur: %s\n", players[1].name);


        }


        do {

            for (int turn = 0; turn < player_Count; ++turn) {

                for (int q = 0; q < 150; q++) {
                    player.name[q] = players[turn].name[q];
                }
                player.isEnemy = players[turn].isEnemy;

                printf("\nC'est au tour de %s avec un score de %i:\n", player.name, players[turn].score);

                do {
                    printf("Choisissez votre pion: ");
                    scanf("%c", &type);
                } while (type == 10);
                clearBuffer();

                if (player.isEnemy) {
                    int nbb;
                    animal.type = type;
                    animal.isEnemy = true;
                    animal.isAlive = true;
                    for (nbb = 0; nbb < animal_Count; nbb++) {
                        if (animals[nbb].isEnemy == true && animals[nbb].type == type &&
                            animals[nbb].isAlive == true) {
                            animal.index = animals[nbb].index;
                            animal.x = animals[nbb].x;
                            animal.y = animals[nbb].y;
                            animal.canEat = animals[nbb].canEat;
                            animal.zone = animals[nbb].zone;
                        }
                    }
                } else {
                    int nbb;
                    animal.type = type;
                    animal.isEnemy = false;
                    animal.isAlive = true;
                    for (nbb = 0; nbb < animal_Count; nbb++) {
                        if (animals[nbb].isEnemy == false && animals[nbb].type == type &&
                            animals[nbb].isAlive == true) {
                            animal.index = animals[nbb].index;
                            animal.x = animals[nbb].x;
                            animal.y = animals[nbb].y;
                            animal.canEat = animals[nbb].canEat;
                            animal.zone = animals[nbb].zone;
                        }
                    }
                }

                do {
                    printf("Choisissez votre direction: ");
                    scanf("%c", &direction);
                } while (direction == 10);
                clearBuffer();
                int nb;

                switch (direction) {

                    case 'A':
                        //printf("Avancer\n");
                        for (nb = 0; nb < animal_Count; nb++) {
                            if (animals[nb].x == animal.x && animals[nb].y == animal.y &&
                                animals[nb].isAlive == true && animals[nb].type == type) {
                                if (animals[nb].isEnemy == true) {
                                    if (animals[nb].x - 1 < 9 && animals[nb].x - 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.x = animal.x - 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'A')) {
                                                animal.x = animal.x - 3;
                                            }
                                        } else {
                                            animal.x = animal.x - 1;
                                        }
                                        if (searchIfCanEat(animal, true, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].x = animal.x;
                                            if (movingCheck(animals[nb]) == RED_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].x = animal.x + 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }
                                } else {
                                    if (animals[nb].x + 1 < 9 && animals[nb].x + 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.x = animal.x + 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'A')) {
                                                animal.x = animal.x + 3;
                                            }
                                        } else {
                                            animal.x = animal.x + 1;
                                        }
                                        if (searchIfCanEat(animal, false, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].x = animal.x;
                                            if (movingCheck(animals[nb]) == BLUE_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].x = animal.x - 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }

                                }
                            }
                        }
                        break;
                    case 'R':
                        //printf("Reculer\n");
                        for (nb = 0; nb < animal_Count; nb++) {
                            if (animals[nb].x == animal.x && animals[nb].y == animal.y &&
                                animals[nb].isAlive == true && animals[nb].type == type) {
                                if (animals[nb].isEnemy == true) {
                                    if (animals[nb].x + 1 < 9 && animals[nb].x + 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.x = animal.x + 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'R')) {
                                                animal.x = animal.x + 3;
                                            }
                                        } else {
                                            animal.x = animal.x + 1;
                                        }
                                        if (searchIfCanEat(animal, true, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].x = animal.x;
                                            if (movingCheck(animals[nb]) == RED_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].x = animal.x - 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }
                                } else {
                                    if (animals[nb].x - 1 < 9 && animals[nb].x - 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.x = animal.x - 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'R')) {
                                                animal.x = animal.x - 3;
                                            }
                                        } else {
                                            animal.x = animal.x - 1;
                                        }
                                        if (searchIfCanEat(animal, false, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].x = animal.x;
                                            if (movingCheck(animals[nb]) == BLUE_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].x = animal.x + 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }
                                }

                            }
                        }
                        break;
                    case 'D':
                        //printf("Droite\n");
                        for (nb = 0; nb < animal_Count; nb++) {
                            if (animals[nb].x == animal.x && animals[nb].y == animal.y &&
                                animals[nb].isAlive == true && animals[nb].type == type) {
                                if (animals[nb].isEnemy == true) {
                                    if (animals[nb].y + 1 < 7 && animals[nb].y + 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.y = animal.y + 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'D')) {
                                                animal.y = animal.y + 2;
                                            }
                                        } else {
                                            animal.y = animal.y + 1;
                                        }
                                        if (searchIfCanEat(animal, true, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].y = animal.y;
                                            if (movingCheck(animals[nb]) == RED_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].y = animal.y - 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }
                                } else {
                                    if (animals[nb].y - 1 < 7 && animals[nb].y - 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.y = animal.y - 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'D')) {
                                                animal.y = animal.y - 2;
                                            }
                                        } else {
                                            animal.y = animal.y - 1;
                                        }
                                        if (searchIfCanEat(animal, false, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].y = animal.y;
                                            if (movingCheck(animals[nb]) == BLUE_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].y = animal.y + 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }

                                }
                            }
                        }
                        break;
                    case 'G':
                        //printf("Gauche\n");
                        for (nb = 0; nb < animal_Count; nb++) {
                            if (animals[nb].x == animal.x && animals[nb].y == animal.y &&
                                animals[nb].isAlive == true && animals[nb].type == type) {
                                if (animals[nb].isEnemy == true) {
                                    if (animals[nb].y - 1 < 7 && animals[nb].y - 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.y = animal.y - 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'G')) {
                                                animal.y = animal.y - 2;
                                            }
                                        } else {
                                            animal.y = animal.y - 1;
                                        }
                                        if (searchIfCanEat(animal, true, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].y = animal.y;
                                            if (movingCheck(animals[nb]) == RED_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].y = animal.y + 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }
                                } else {
                                    if (animals[nb].y + 1 < 7 && animals[nb].y + 1 > -1) {
                                        if (animals[nb].type == 'L' || animals[nb].type == 'T') {
                                            animal.y = animal.y + 1;
                                            if (movingCheck(animal) == LAKE && !hasRat(animals[nb], 'G')) {
                                                animal.y = animal.y + 2;
                                            }
                                        } else {
                                            animal.y = animal.y + 1;
                                        }
                                        if (searchIfCanEat(animal, false, &players[turn])) {
                                            coordinates[animals[nb].x][animals[nb].y] = 0;
                                            animals[nb].y = animal.y;
                                            if (movingCheck(animals[nb]) == BLUE_TRAP) {
                                                animals[nb].canEat = false;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == BLUE_SANCTUARY) {
                                                isWinner = true;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else if (movingCheck(animals[nb]) == RED_SANCTUARY ||
                                                       animal.type != 'R' && movingCheck(animals[nb]) == LAKE) {
                                                animals[nb].y = animal.y - 1;
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            } else {
                                                animals[nb].zone = movingCheck(animals[nb]);
                                            }
                                        } else {
                                            animals[nb].canEat = true;
                                        }
                                    }

                                }
                            }
                        }

                        break;

                    default:
                        break;


                }

                if (!isWinner) {
                    printChessTable();
                } else {
                    printChessTable();
                    if (players[turn].isEnemy) {
                        printf("%s de l'equipe BLEUE a gagne(e) la partie avec %i point(s)!", player.name,
                               players[turn].score);
                    } else {
                        printf("%s de l'equipe ROUGE a gagne(e) la partie avec %i point(s)!", player.name,
                               players[turn].score);
                    }
                    remove("save.txt");
                    getch();
                    return;
                }


            }
            printf("Voulez-vous continuer? (Oui: Entrer / Non: Echap)");

#ifdef _WIN32
        } while (getch() != 27);
#else
        }while(getc(stdin)!=27);
#endif
    }
}

bool hasRat(Animal animal, char direction) {

    int m, i, j, k, l;
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 7; j++) {
            if (coordinates[i][j] != 0) {
                for (m = 0; m < animal_Count; m++) {
                    if (animals[m].x == i && animals[m].y == j) {

                        if (animals[m].type == 'R' && animals[m].isAlive && animals[m].zone == LAKE) {
                            switch (direction) {

                                case 'D':
                                    k = animal.x;
                                    if (!animal.isEnemy) {
                                        for (l = animal.y; l > animal.y - 3; l--) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    } else {
                                        for (l = animal.y; l < animal.y + 3; l++) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    }
                                case 'G':
                                    k = animal.x;
                                    if (!animal.isEnemy) {
                                        for (l = animal.y; l < animal.y + 3; l++) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    } else {
                                        for (l = animal.y; l > animal.y - 3; l--) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    }
                                case 'A':
                                    l = animal.y;
                                    if (!animal.isEnemy) {
                                        for (k = animal.x; k < animal.x + 4; k++) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    } else {
                                        for (k = animal.x; k > animal.x - 4; k--) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    }
                                case 'R':
                                    l = animal.y;
                                    if (!animal.isEnemy) {
                                        for (k = animal.x; k > animal.x - 4; k--) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    } else {
                                        for (k = animal.x; k < animal.x + 4; k++) {
                                            if (animals[m].x == k && animals[m].y == l) {
                                                return true;

                                            }
                                        }
                                    }


                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

void setCoord() {
    int m;

    for (m = 0; m < animal_Count; m++) {
        if (!animals[m].isEnemy) {
            if (animals[m].type == ELEPHANT) {
                animals[m].x = 2;
                animals[m].y = 6;
            }
            if (animals[m].type == LION) {
                animals[m].x = 0;
                animals[m].y = 0;
            }
            if (animals[m].type == TIGER) {
                animals[m].x = 0;
                animals[m].y = 6;
            }
            if (animals[m].type == PANTHER) {
                animals[m].x = 2;
                animals[m].y = 2;
            }
            if (animals[m].type == DOG) {
                animals[m].x = 1;
                animals[m].y = 1;
            }
            if (animals[m].type == WOLF) {
                animals[m].x = 2;
                animals[m].y = 4;
            }
            if (animals[m].type == CAT) {
                animals[m].x = 1;
                animals[m].y = 5;
            }
            if (animals[m].type == RAT) {
                animals[m].x = 2;
                animals[m].y = 0;
            }

        } else {
            if (animals[m].type == ELEPHANT) {
                animals[m].x = 6;
                animals[m].y = 0;
            }
            if (animals[m].type == LION) {
                animals[m].x = 8;
                animals[m].y = 6;
            }
            if (animals[m].type == TIGER) {
                animals[m].x = 8;
                animals[m].y = 0;
            }
            if (animals[m].type == PANTHER) {
                animals[m].x = 6;
                animals[m].y = 4;
            }
            if (animals[m].type == DOG) {
                animals[m].x = 7;
                animals[m].y = 5;
            }
            if (animals[m].type == WOLF) {
                animals[m].x = 6;
                animals[m].y = 2;
            }
            if (animals[m].type == CAT) {
                animals[m].x = 7;
                animals[m].y = 1;
            }
            if (animals[m].type == RAT) {
                animals[m].x = 6;
                animals[m].y = 6;
            }

        }

    }

}

enum Zone movingCheck(struct Animal animal) {


    int i, j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 7; j++) {

            if (animal.x == 3 && animal.y == 1 || animal.x == 3 && animal.y == 2 || animal.x == 4 && animal.y == 1 ||
                animal.x == 4 && animal.y == 2 ||
                animal.x == 5 && animal.y == 1 || animal.x == 5 && animal.y == 2 || animal.x == 3 && animal.y == 4 ||
                animal.x == 3 && animal.y == 5 ||
                animal.x == 4 && animal.y == 4 || animal.x == 4 && animal.y == 5 || animal.x == 5 && animal.y == 4 ||
                animal.x == 5 && animal.y == 5) {
                //zone lac
                return LAKE;
            } else if (animal.x == 0 && animal.y == 2 || animal.x == 0 && animal.y == 3 ||
                       animal.x == 0 && animal.y == 4 || animal.x == 1 && animal.y == 3) {
                if (animal.x == 0 && animal.y == 3) {
                    //zone sanctuaire rouge
                    return RED_SANCTUARY;
                } else {
                    //zone piege rouge
                    return RED_TRAP;
                }
            } else if (animal.x == 8 && animal.y == 2 || animal.x == 8 && animal.y == 3 ||
                       animal.x == 8 && animal.y == 4 || animal.x == 7 && animal.y == 3) {
                if (animal.x == 8 && animal.y == 3) {
                    //zone sanctuaire bleu
                    return BLUE_SANCTUARY;
                } else {
                    //zone piege bleu
                    return BLUE_TRAP;
                }
            } else {
                return NONE;
            }


        }
    }


}

void printChessTable() {

    int i, j, m, b, x, y;

    //utiliser le meme prototype que ci-dessous pour les déplacement
    for (b = 0; b < animal_Count; b++) {
        if (animals[b].isAlive) {
            x = animals[b].x;
            y = animals[b].y;
            coordinates[x][y] = animals[b].type;
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
            if (coordinates[i][j] == 0) {
                if (i == 3 && j == 1 || i == 3 && j == 2 || i == 4 && j == 1 || i == 4 && j == 2 ||
                    i == 5 && j == 1 || i == 5 && j == 2 || i == 3 && j == 4 || i == 3 && j == 5 ||
                    i == 4 && j == 4 || i == 4 && j == 5 || i == 5 && j == 4 || i == 5 && j == 5) {
                    color(15, 2);
                    printf("   ");
                    SetConsoleTextAttribute(hConsole, saved_attributes);
                } else if (i == 0 && j == 2 || i == 0 && j == 3 || i == 0 && j == 4 || i == 1 && j == 3) {
                    if (i == 0 && j == 3) {
                        color(15, 4);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    } else {
                        color(15, 6);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    }
                } else if (i == 8 && j == 2 || i == 8 && j == 3 || i == 8 && j == 4 || i == 7 && j == 3) {
                    if (i == 8 && j == 3) {
                        color(15, 1);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    } else {
                        color(15, 6);
                        printf("   ");
                        SetConsoleTextAttribute(hConsole, saved_attributes);
                    }
                } else {
                    printf("   |");
                    continue;
                }
            } else {
                //utiliser le meme prototype que ci-dessous pour l'affichage des pions
                for (m = 0; m < animal_Count; m++) {
                    if (animals[m].x == i && animals[m].y == j) {
                        //Enemy = Blue Team
                        if (animals[m].isAlive) {
                            if (animals[m].isEnemy) {
                                if (animals[m].zone == LAKE) {
                                    color(1, 2);
                                } else if (animals[m].zone == RED_TRAP || animals[m].zone == BLUE_TRAP) {
                                    color(1, 6);
                                } else if (animals[m].zone == RED_SANCTUARY) {
                                    color(1, 4);
                                } else {
                                    color(1, 0);
                                }
                                printf(" %c ", coordinates[i][j]);
                                SetConsoleTextAttribute(hConsole, saved_attributes);
                            } else {
                                if (animals[m].zone == LAKE) {
                                    color(4, 2);
                                } else if (animals[m].zone == RED_TRAP || animals[m].zone == BLUE_TRAP) {
                                    color(4, 6);
                                } else if (animals[m].zone == BLUE_SANCTUARY) {
                                    color(4, 1);
                                } else {
                                    color(4, 0);
                                }
                                printf(" %c ", coordinates[i][j]);
                                SetConsoleTextAttribute(hConsole, saved_attributes);
                            }
                        }

                    }
                }

            }
#else
            if (coordinates[i][j] == 0) {
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
                                printf(" %s%c%s ", Color_Blue, coordinates[i][j], Color_End);
                            } else {
                                printf(" %s%c%s ", Color_Red, coordinates[i][j], Color_End);
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

bool canEat(struct Animal *enemy, struct Animal ally, struct Player *playerTab) {

    if (enemy->isEnemy != ally.isEnemy) {


        if (enemy->index >= ally.index && enemy->zone != LAKE) {

            enemy->isAlive = false;
            coordinates[enemy->x][enemy->y] = 0;
            playerTab->score++;
            return true;

        } else if (enemy->type == 'E' && ally.type == 'R' && ally.zone != LAKE) {

            enemy->isAlive = false;
            coordinates[enemy->x][enemy->y] = 0;
            playerTab->score++;
            return true;

        } else {

            return false;
        }

    } else {
        return false;
    }

}

bool searchIfCanEat(struct Animal animal, bool isEnemy, struct Player *playerTab) {

    int b;

    if (isEnemy) {
        for (b = 0; b < animal_Count; b++) {
            if (animals[b].x == animal.x && animals[b].y == animal.y && animals[b].isAlive) {
                if (animal.canEat) {
                    if (!animals[b].isEnemy) {
                        if (canEat(&animals[b], animal, playerTab)) {
                            return true;

                        } else {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }

    } else {
        for (b = 0; b < animal_Count; b++) {
            if (animals[b].x == animal.x && animals[b].y == animal.y && animals[b].isAlive) {
                if (animal.canEat) {
                    if (animals[b].isEnemy) {
                        if (canEat(&animals[b], animal, playerTab)) {
                            return true;

                        } else {
                            return false;
                        }
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
    }


}

void generateChessSet() {

    setlocale(LC_ALL, "fr_FR.UTF-8");

    Animal animal;
    int l;

    animals = malloc(16 * sizeof(Animal));//Création du tableau d'objets<Animal>
    animalType = malloc(8 * sizeof(char));//nos types d'animaux

    animalType[0] = ELEPHANT;
    animalType[1] = LION;
    animalType[2] = TIGER;
    animalType[3] = PANTHER;
    animalType[4] = DOG;
    animalType[5] = WOLF;
    animalType[6] = CAT;
    animalType[7] = RAT;

    if (!readSave()) {

        players = NULL;
        for (l = 0; l < 16; l++) {//1er joueur

            if (l < 8) {
                animal.isEnemy = false;
                animal.isAlive = true;
                animal.index = l;
                animal.type = animalType[l];
                animal.zone = NONE;
                animal.canEat = true;
                animals[l] = animal;
            } else {

                //second joueur
                animal.isEnemy = true;
                animal.isAlive = true;
                animal.index = l - 8;
                animal.type = animalType[l - 8];
                animal.zone = NONE;
                animal.canEat = true;
                animals[l] = animal;
            }
        }
        setCoord();

    }

    //printChessTable();
    loadGame();
    if (!isWinner) {
        writeSave(animals);
    }

}

