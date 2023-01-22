//
// Created by maxim on 22/01/2023.
//
#include "../headers/save.h"
#include "../headers/exception.h"


sqlite3 *getDbContext() {
    sqlite3 *db;
    int rc;

    TRY
            {
                rc = sqlite3_open("./data/save.dat", &db);
                if (rc) {
                    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                    sqlite3_close(db);
                    THROW;
                } else {
                    printf((const char *) stdout, "[SQL] GAME DATABASE INSTANCE HAS BEEN CREATED");
                }
            }
        CATCH
            {
                printf("SqlException has been thrown\r\n");
                return NULL;
            }

    END_TRY;

    return db;
}


void insertOrUpdateSave(struct Player *players, struct Animal *animals, int nPlayers, int nAnimals) {

    sqlite3 *dbContext = getDbContext();
    createGameSaveTable(dbContext);

    char *sql = "INSERT OR REPLACE INTO Player(name, isEnemy, score) VALUES (?, ?, ?);";

    sqlite3_stmt *pStmt;
    sqlite3_prepare_v2(dbContext, sql, -1, &pStmt, 0);

    for (int i = 0; i < nPlayers; i++) {

        sqlite3_bind_text(pStmt, 1, players[i].name, -1, SQLITE_STATIC);
        sqlite3_bind_int(pStmt, 2, players[i].isEnemy);
        sqlite3_bind_int(pStmt, 3, players[i].score);

        sqlite3_step(pStmt);
        sqlite3_reset(pStmt);

    }

    sqlite3_finalize(pStmt);

    sql = "INSERT OR REPLACE INTO Animal(type, x, y, isEnemy, isAlive, canEat, table_index, zone, playerId) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_prepare_v2(dbContext, sql, -1, &pStmt, 0);

    for (int i = 0; i < nAnimals; i++) {

        sqlite3_bind_text(pStmt, 1, &animals[i].type, 1, SQLITE_STATIC);
        sqlite3_bind_int(pStmt, 2, animals[i].x);
        sqlite3_bind_int(pStmt, 3, animals[i].y);
        sqlite3_bind_int(pStmt, 4, animals[i].isEnemy);
        sqlite3_bind_int(pStmt, 5, animals[i].isAlive);
        sqlite3_bind_int(pStmt, 6, animals[i].canEat);
        sqlite3_bind_int(pStmt, 7, animals[i].index);
        sqlite3_bind_text(pStmt, 8, &animals[i].zone, 1, SQLITE_STATIC);
        sqlite3_bind_int(pStmt, 9, animals[i].index);

        sqlite3_step(pStmt);
        sqlite3_reset(pStmt);

    }

    sqlite3_finalize(pStmt);

    sqlite3_close(dbContext);
}

int createGameSaveTable(sqlite3 *db) {
    char *err_msg = 0;
    int rc;

    char *sql = "CREATE TABLE IF NOT EXISTS  Player (\n"
                "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                "    name TEXT NOT NULL,\n"
                "    isEnemy BOOLEAN NOT NULL CHECK (isEnemy IN (0, 1)),\n"
                "    score INTEGER NOT NULL\n"
                ");"
                "CREATE TABLE IF NOT EXISTS  Animal (\n"
                "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                "    type TEXT NOT NULL,\n"
                "    x INTEGER NOT NULL,\n"
                "    y INTEGER NOT NULL,\n"
                "    isEnemy BOOLEAN NOT NULL CHECK (isEnemy IN (0, 1)),\n"
                "    isAlive BOOLEAN NOT NULL CHECK (isAlive IN (0, 1)),\n"
                "    canEat BOOLEAN NOT NULL CHECK (canEat IN (0, 1)),\n"
                "    table_index INTEGER NOT NULL,\n"
                "    zone TEXT NOT NULL,\n"
                "    playerId INTEGER NOT NULL,\n"
                "    FOREIGN KEY(playerId) REFERENCES Player(id)\n"
                ");"
                "CREATE TRIGGER IF NOT EXISTS delete_player_animals_after_delete_player\n"
                "AFTER DELETE ON Player\n"
                "BEGIN\n"
                "    DELETE FROM Animal WHERE playerId = old.id;\n"
                "END;"
                "CREATE TRIGGER IF NOT EXISTS delete_animal_when_not_isAlive\n"
                "AFTER UPDATE OF isAlive ON Animal\n"
                "WHEN NEW.isAlive = 1\n"
                "BEGIN\n"
                "    DELETE FROM Animal WHERE id = NEW.id;\n"
                "END;";
    TRY
            {
                rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "SQL error: %s\n", err_msg);
                    sqlite3_free(err_msg);
                    THROW;
                } else {
                    printf((const char *) stdout, "[SQL] GAME TABLE HAS BEEN CREATED");
                }
            }


        CATCH
            {
                printf("SqlException has been thrown\r\n");
                return 1;
            }

    END_TRY;

    return 0;
}
