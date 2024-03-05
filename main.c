// Copyright 2022 <314CA Nedelcu Andrei David>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashtable.h"
#include "entities.h"
#include "utils.h"
#include "commands.h"

#define LINESZ 256

void show_top_books(hashtable_t *library)
{
    // Obtine o lista cu cartile
    book_t **book_list = (book_t **)ht_records(library);

    printf("Books ranking:\n");

    // Verifica daca lista e goala
    if (!book_list)
        return;

    // Sorteaza lista dupa criteriile cerute si afiseaza elementele
    qsort(book_list, ht_get_size(library), sizeof(book_t *), cmp_book);

    int book_cnt = ht_get_size(library);

    for (int i = 0; i < book_cnt; i++) {
        printf("%d. ", i + 1);
        book_print(book_list[i]);
    }

    free(book_list);
}

void show_top_users(hashtable_t *users)
{
    // Obtine o lista cu utilizatorii
    user_t **user_list = (user_t **)ht_records(users);

    printf("Users ranking:\n");

    // Verifica daca lista e goala
    if (!user_list)
        return;

    // Sorteaza lista dupa criteriile cerute si afiseaza elementele
    qsort(user_list, ht_get_size(users), sizeof(user_t *), cmp_user);

    int user_cnt = ht_get_size(users);

    for (int i = 0; i < user_cnt; i++) {
        if (user_list[i]->points < 0)
            break;      // Din acest punct toti utilizatorii sunt banati

        printf("%d. ", i + 1);
        user_print(user_list[i]);
    }

    free(user_list);
}

int main()
{
    // Initializeaza tabelele de cautare pentru utilizatori si carti
    hashtable_t *users = ht_create(hash_string, cmpr_strings,
                                   USERNAME_LEN, sizeof(user_t));
    hashtable_t *library = ht_create(hash_string, cmpr_strings,
                                     BOOK_LEN, sizeof(book_t));
    // Aloca memorie pentru comanda
    char *line = (char *)malloc(LINESZ);
    assert(line);
    char command[CMD_LEN], *it;

    while (fgets(line, LINESZ - 1, stdin)) {
        // Parseaza numele comenzii
        it = line;
        memset(command, 0, CMD_LEN);
        get_next_arg(&it, command);

        // Executa comanda
        if (strcmp(command, "ADD_BOOK") == 0) {
            add_book(library, it);
        } else if (strcmp(command, "GET_BOOK") == 0) {
            get_book(library, it);
        } else if (strcmp(command, "RMV_BOOK") == 0) {
            rmv_book(library, it);
        } else if (strcmp(command, "ADD_DEF") == 0) {
            add_def(library, it);
        } else if (strcmp(command, "GET_DEF") == 0) {
            get_def(library, it);
        } else if (strcmp(command, "RMV_DEF") == 0) {
            rmv_def(library, it);
        } else if (strcmp(command, "ADD_USER") == 0) {
            add_user(users, it);
        } else if (strcmp(command, "BORROW") == 0) {
            borrow(users, library, it);
        } else if (strcmp(command, "RETURN") == 0) {
            ret_book(users, library, it);
        } else if (strcmp(command, "LOST") == 0) {
            lost(users, library, it);
        } else if (strcmp(command, "EXIT") == 0) {
            break;
        }
    }

    // Afiseaza clasamentele
    show_top_books(library);
    show_top_users(users);

    // Elibereaza din memorie comanda si tabelele de cautare
    ht_free(users, free_user);
    ht_free(library, free_book);
    free(line);

    return 0;
}
