// Copyright 2022 <314CA Nedelcu Andrei David>
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DELIM "\n "

// Implementare C99 pt strdup()
char *dup_str(const char *string)
{
    char *dup = (char *)malloc(strlen(string) + 1);
    assert(dup);
    return strcpy(dup, string);
}

char *get_next_arg(char **iterator, char *buffer)
{
    char *begin = *iterator;

    // Cauta inceputul cuvantului
    // Sari peste whitespace
    while (*begin && strchr(DELIM, *begin))
        begin++;

    if (*begin == '\0')
        return NULL;
    int has_quotes = *begin == '"';

    char *end;
    if (has_quotes) {
        // Treci de ghilimelele de inceput
        begin++;

        end = begin;
        // Cauta ghilimelele de final

        while (*end && *end != '"')
            end++;
    } else {
        end = begin;
        while (*end && !strchr(DELIM, *end))
            end++;
    }

    if (has_quotes && *end == '\0') {
        // Nu s-au gasit ghilimelele de final
        return NULL;
    }

    strncpy(buffer, begin, end - begin);

    if (has_quotes) {
        // Treci de ghilimelele de final
        end++;
    }

    // Seteaza pozitia iteratorului pentru urmatorul apel
    *iterator = end;
    return buffer;
}

void ban_hammer(char *username)
{
    printf("The user %s has been banned from this library.\n", username);
}
