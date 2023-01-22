//
// Created by maxim on 22/01/2023.
//

#ifndef JUNGLEGAME_SAVE_H
#define JUNGLEGAME_SAVE_H

#include <stdio.h>
#include <stdbool.h>
#include "echiquier.h"

__attribute__((aligned(16))) char* sqlite_statement;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int getDbContext();

bool checkIfTableExists(char *tableName);

bool checkIfDatabaseExists(char *databaseName);

void createGameSaveStatement(struct Player *players, struct Animal *animals);

int executeGameSaveStatement();

#endif //JUNGLEGAME_SAVE_H
