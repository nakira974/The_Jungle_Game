//
// Created by maxim on 22/01/2023.
//

#include <malloc.h>
#include <string.h>
#include "tools/utilities.h"
#include "tools/exception.h"

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
void reverse(const char str[], int length)
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


char *toString(int num)
{
    int i = 0, j = 0, temp = 0, len = 0;
    char *str = NULL;

    temp = num;
    if (num < 0)
    {
        num = -num;
        len++;
    }
    while (temp)
    {
        len++;
        temp /= 10;
    }
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (str == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    if (num == 0)
    {
        str[i++] = '0';
    }
    while (num)
    {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    if (temp < 0)
    {
        str[i++] = '-';
    }
    str[i] = '\0';
    i--;
    while (j < i)
    {
        temp = str[j];
        str[j] = str[i];
        str[i] = temp;
        j++;
        i--;
    }
    return str;
}

char* intToString(int num)
{
    int i = 0;
    int isNegative = 0;
    char *str = NULL;
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
        //TODO Need to be fixed
        TRY{
            str[i++] = (rem > 9) ? rem - 10 + 'a' : rem + '0';
            THROW;
        }CATCH{
            fprintf(stderr, "Process individual digits has failed for %d", num);
            return NULL;
        }
        END_TRY;
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

