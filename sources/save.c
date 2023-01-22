//
// Created by maxim on 22/01/2023.
//
#include "../headers/save.h"
#include "../headers/exception.h"


char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void swap(char* a, char* b)
{
    char t = *a;
    *a = *b;
    *b = t;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wint-to-pointer-cast"
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap((char *) *(str + start), (char *) *(str + end));
        start++;
        end--;
    }
}
#pragma clang diagnostic pop



char* intToString(int num)
{
    int i = 0;
    int isNegative = 0;
    char *str;
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str = (char*)malloc(2);
        str[0] = '0';
        str[1] = '\0';
        return str;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && num > -10)
    {
        isNegative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/10;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    return str;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wpointer-sign"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpointer-to-int-cast"
char sqlite3_text_column_to_char(const unsigned char* text)
{
    return (char )text;
}
#pragma clang diagnostic pop

void sqlite3_text_column_to_string(const unsigned char* column_value, char *string)
{
    strcpy(string, column_value);
}


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

        sqlite3_bind_text(pStmt, 1, (const char *) players[i].name, -1, SQLITE_STATIC);
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
        sqlite3_bind_text(pStmt, 8, (const char *) &animals[i].zone, 1, SQLITE_STATIC);
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"

_Noreturn void selectSavedEntities(struct Player *players, struct Animal *animals) {

    for(int player=0; player < 2; player++) {
        players[player].id = 0;
    }
        sqlite3 *db = getDbContext();
    char *sql = "SELECT * FROM Player LIMIT 2;";

    sqlite3_stmt *pStmt;
    sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);

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
            sqlite3_stmt *pCurrentStatement;
            char* playerId = intToString(players[player].id);
            sql = "SELECT * FROM Animal WHERE playerId=";
            sql =concat(sql, playerId);
            sql = concat(sql, ";");
            //TODO Fixer l'exception d'accès concurrent à la db ???fix
            sqlite3_prepare_v2(db, sql, -1, &pCurrentStatement, 0);
            i = 0;
            //TODO Throw exceptions here when null entries
            while (sqlite3_step(pCurrentStatement) == SQLITE_ROW) {

                animals[i].type = sqlite3_text_column_to_char(sqlite3_column_text(pCurrentStatement, 1));
                animals[i].x = sqlite3_column_int(pCurrentStatement, 2);
                animals[i].y = sqlite3_column_int(pCurrentStatement, 3);
                animals[i].isEnemy = sqlite3_column_int(pCurrentStatement, 4);
                animals[i].isAlive = sqlite3_column_int(pCurrentStatement, 5);
                animals[i].canEat = sqlite3_column_int(pCurrentStatement, 6);
                animals[i].index = sqlite3_column_int(pCurrentStatement, 7);
                animals[i].zone = sqlite3_column_int(pCurrentStatement, 8);

                i++;

            }
            sqlite3_finalize(pStmt);

            sqlite3_close(db);
        }

    }

}
#pragma clang diagnostic pop

#pragma clang diagnostic pop