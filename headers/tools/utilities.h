//
// Created by maxim on 22/01/2023.
//

#ifndef JUNGLEGAME_UTILITIES_H
#define JUNGLEGAME_UTILITIES_H
char* toString(int nb);
char* concat(char *s1, char *s2);
void swap(char* a, char* b);
void reverse(const char str[], int length);
char* intToString(int num);
char sqlite3_text_column_to_char(const unsigned char* text);
void sqlite3_text_column_to_string(const unsigned char* column_value, char *string);

#endif //JUNGLEGAME_UTILITIES_H
