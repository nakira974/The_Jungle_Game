//
// Created by maxim on 22/01/2023.
//
#include "database/save.h"
#include "tools/exception.h"
#include "tools/utilities.h"

int insertOrUpdatePlayers(struct Player *players, int nPlayers, sqlite3 *dbContext, sqlite3_stmt *pStmt);
int insertOrUpdateAnimals(struct Animal *animals, int nAnimals, struct Player *players, sqlite3 *dbContext, sqlite3_stmt *pStmt);

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

int selectPlayerId(struct Player player, sqlite3 *dbContext){
    int result = 0;
    char *sql = "SELECT id FROM Player WHERE name= ? ;";
    sqlite3_stmt *pStmt;

    TRY{
        sqlite3_prepare_v2(dbContext, sql, -1, &pStmt, 0);
        sqlite3_bind_text(pStmt, 1, (const char *) player.name, -1, SQLITE_STATIC);
        while (sqlite3_step(pStmt) == SQLITE_ROW) {
            if(sqlite3_column_int(pStmt, 0) != 0){
                result = sqlite3_column_int(pStmt, 0);
            }
        }

        sqlite3_finalize(pStmt);
        return result;
        THROW;
            //TODO Handle sqlite_bind_text exceptions/error codes
    }CATCH{
                fprintf(stdout, "Error while selecting player:%s id", player.name);
                return -1;
    };
    END_TRY;

}

void insertOrUpdateSave(struct Player *players, struct Animal *animals, int nPlayers, int nAnimals) {

    sqlite3 *dbContext = getDbContext();
    sqlite3_stmt *pt_statement;
    createGameSaveTable(dbContext);

    insertOrUpdatePlayers(players, nPlayers, dbContext, pt_statement);
    insertOrUpdateAnimals(animals, nAnimals, players, dbContext, pt_statement);
    sqlite3_close(dbContext);
}

int createGameSaveTable(sqlite3 *db) {
    char *err_msg = 0;
    int rc;

    char *sql = "CREATE TABLE IF NOT EXISTS  Player (\n"
                "    id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
                "    name TEXT NOT NULL ,\n"
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
                "    zone INTEGER NOT NULL,\n"
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
                "END;"
                "CREATE UNIQUE INDEX idx_player_name ON Player (name);\n"
                "CREATE UNIQUE INDEX idx_animal_type ON Animal (isEnemy, type);";
    TRY
            {
                rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
                if (rc != SQLITE_OK) {
                    fprintf(stderr, "SQL error: %s\n", err_msg);
                    sqlite3_free(err_msg);
                    THROW;
                } else {
                    printf((const char *) stdout, "[SQL] GAME TABLES HAS BEEN CREATED");
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

int insertOrUpdateAnimals(struct Animal *animals, int nAnimals, struct Player *players, sqlite3 *dbContext, sqlite3_stmt *pStmt){
    int result = 0;
    char *sql = "INSERT OR REPLACE INTO Animal(type, x, y, isEnemy, isAlive, canEat, table_index, zone, playerId) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_prepare_v2(dbContext, sql, -1, &pStmt, 0);

    for (int i = 0; i < nAnimals; i++) {

        sqlite3_bind_text(pStmt, 1, &animals[i].type, 1, SQLITE_STATIC);
        sqlite3_bind_int(pStmt, 2, animals[i].x);
        sqlite3_bind_int(pStmt, 3, animals[i].y);
        sqlite3_bind_int(pStmt, 4, animals[i].isEnemy);
        sqlite3_bind_int(pStmt, 5, animals[i].isAlive);
        sqlite3_bind_int(pStmt, 6, animals[i].canEat);
        sqlite3_bind_int(pStmt, 7, animals[i].index);
        sqlite3_bind_text(pStmt, 8, (const char *) &animals[i].zone, 1, SQLITE_STATIC);
        //TODO Need to be fixed
        sqlite3_bind_int(pStmt, 9, animals[i].isEnemy ? players[0].id : players[1].id);

        sqlite3_step(pStmt);
        sqlite3_reset(pStmt);

    }

    sqlite3_finalize(pStmt);

    return result;
}

int insertOrUpdatePlayers(struct Player *players, int nPlayers, sqlite3 *dbContext, sqlite3_stmt *pStmt){
    int result = 0;
    char *sql = "INSERT OR REPLACE INTO Player(name, isEnemy, score) VALUES (?, ?, ?);";

    TRY{
        sqlite3_prepare_v2(dbContext, sql, -1, &pStmt, 0);
                for (int i = 0; i < nPlayers; i++) {

                    sqlite3_bind_text(pStmt, 1, (const char *) players[i].name, -1, SQLITE_STATIC);
                    sqlite3_bind_int(pStmt, 2, players[i].isEnemy);
                    sqlite3_bind_int(pStmt, 3, players[i].score);
                    sqlite3_step(pStmt);
                    sqlite3_reset(pStmt);
                }
                result = sqlite3_finalize(pStmt);
                for(int i = 0; i < nPlayers; i++){
                    players[i].id = selectPlayerId(players[i], dbContext);
                }
                return result;
                THROW;
            }CATCH{
                fprintf(stderr,"Error while insertion or updating of players");
                return result;
    }
    END_TRY;

}
int selectSavedEntities(struct Player *players, struct Animal *animals) {

    int result = 0;

    for(int player=0; player < 2; player++) {
        players[player].id = 0;
    }

    sqlite3 *db = getDbContext();
    char *sql = "SELECT * FROM Player LIMIT 2;";

    sqlite3_stmt *pStmt;
    result = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
    if(result)
        return result;

    int i = 0;

    //TODO Throw exceptions here when null entries
    while (sqlite3_step(pStmt) == SQLITE_ROW) {
        if(sqlite3_column_int(pStmt, 0) != 0){
            players[i].id = sqlite3_column_int(pStmt, 0);
            sqlite3_text_column_to_string(sqlite3_column_text(pStmt, 1), (char *) players[i].name);
            players[i].isEnemy = sqlite3_column_int(pStmt, 2);
            players[i].score = sqlite3_column_int(pStmt, 3);
            fprintf(stdout, "USER %s %d %d HAS BEEN SELECTED\n",players[i].name, players[i].isEnemy, players[i].score );
            i++;
        }
    }

    sqlite3_finalize(pStmt);

    for(int player=0; player < 2; player++){
        if(players[player].id != 0){
            char* playerId = toString(players[player].id);
            sql = "SELECT * FROM Animal WHERE playerId= ?";

            result = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);
            if(result)
                return result;
            result = sqlite3_bind_int(pStmt, 1, players[player].id);
            if(result)
                return result;
            i = players[player].isEnemy ? 0 : 8;
            //TODO Throw exceptions here when null entries
            while (sqlite3_step(pStmt) == SQLITE_ROW) {

                animals[i].type = sqlite3_text_column_to_char(sqlite3_column_text(pStmt, 1));
                animals[i].x = sqlite3_column_int(pStmt, 2);
                animals[i].y = sqlite3_column_int(pStmt, 3);
                animals[i].isEnemy = sqlite3_column_int(pStmt, 4);
                animals[i].isAlive = sqlite3_column_int(pStmt, 5);
                animals[i].canEat = sqlite3_column_int(pStmt, 6);
                animals[i].index = sqlite3_column_int(pStmt, 7);
                animals[i].zone = sqlite3_column_int(pStmt, 8);
                i++;
            }

            fprintf(stdout, "USER %s 's animals have been selected\n", playerId);
            sqlite3_finalize(pStmt);
        }

    }

    sqlite3_close(db);

    return result;
}
#pragma clang diagnostic pop

#pragma clang diagnostic pop