//
// Created by maxim on 22/01/2023.
//

#ifndef JUNGLEGAME_SAVE_H
#define JUNGLEGAME_SAVE_H

#include <stdio.h>
#include <stdbool.h>
#include "echiquier.h"
#include "sqlite3.h"


static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

sqlite3 *getDbContext();

int createGameSaveTable(sqlite3 *db);

void insertOrUpdateSave(struct Player *players, struct Animal *animals, int nPlayers, int nAnimals);

void selectSavedEntities(struct Player *players, struct Animal *animals);

#endif //JUNGLEGAME_SAVE_H
