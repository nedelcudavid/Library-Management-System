// Copyright 2022 <314CA Nedelcu Andrei David>
#include "commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashtable.h"
#include "utils.h"
#include "entities.h"

void add_book(hashtable_t *library, char *it)
{
    // Parseaza titlul cartii si numarul de definitii
    char title[BOOK_LEN] = {0};
    get_next_arg(&it, title);

    char def_count_buf[NUM_LEN] = {0};
    get_next_arg(&it, def_count_buf);
    int def_count = atoi(def_count_buf);

    // Initializeaza cartea
    book_t book;
    init_book(&book, title);

    // Adauga definitiile in carte
    for (int i = 0; i < def_count; i++) {
        char def_key[DEF_LEN] = {0}, def_val[DEF_LEN] = {0};
        scanf("%s %s\n", def_key, def_val);

        ht_put(book.contents, def_key, def_val, free);
    }

    // Adauga cartea in biblioteca
    ht_put(library, title, &book, free_book);
}

void get_book(hashtable_t *library, char *it)
{
    // Parseaza titlul cartii
    char title[BOOK_LEN] = {0};
    get_next_arg(&it, title);

    // Verifica daca cartea exista in biblioteca
    if (!ht_has_key(library, title)) {
        fputs(MSG_NOBOOK, stdout);
        return;
    }

    book_t *book = (book_t *)ht_get(library, title);
    book_print(book);
}

void rmv_book(hashtable_t *library, char *it)
{
    // Parseaza titlul cartii
    char title[BOOK_LEN] = {0};
    get_next_arg(&it, title);

    // Verifica daca cartea exista in biblioteca
    if (!ht_has_key(library, title)) {
        fputs(MSG_NOBOOK, stdout);
        return;
    }

    book_t *book = (book_t *)ht_remove_entry(library, title);
    free_book(book);
}

void add_def(hashtable_t *library, char *it)
{
    // Parseaza titlul cartii si definitia
    char title[BOOK_LEN] = {0}, def_key[DEF_LEN] = {0}, def_val[DEF_LEN] = {0};
    get_next_arg(&it, title);
    get_next_arg(&it, def_key);
    get_next_arg(&it, def_val);

    // Verifica daca cartea exista in biblioteca
    if (!ht_has_key(library, title)) {
        fputs(MSG_NOBOOK, stdout);
        return;
    }

    // Cauta cartea si adauga definitia
    book_t *book = (book_t *)ht_get(library, title);
    ht_put(book->contents, def_key, def_val, free);
}

void get_def(hashtable_t *library, char *it)
{
    // Parseaza titlul cartii si cheia definitiei
    char title[BOOK_LEN] = {0}, def_key[DEF_LEN] = {0};
    get_next_arg(&it, title);
    get_next_arg(&it, def_key);

    // Verifica daca cartea exista in biblioteca
    if (!ht_has_key(library, title)) {
        fputs(MSG_NOBOOK, stdout);
        return;
    }

    book_t *book = (book_t *)ht_get(library, title);
    // Verifica daca definitia exista in carte

    if (!ht_has_key(book->contents, def_key)) {
        fputs(MSG_NODEF, stdout);
        return;
    }

    printf("%s\n", (char *)ht_get(book->contents, def_key));
}

void rmv_def(hashtable_t *library, char *it)
{
    // Parseaza titlul cartii si cheia definitiei
    char title[BOOK_LEN] = {0}, def_key[DEF_LEN] = {0};
    get_next_arg(&it, title);
    get_next_arg(&it, def_key);

    // Verifica daca cartea exista in biblioteca
    if (!ht_has_key(library, title)) {
        fputs(MSG_NOBOOK, stdout);
        return;
    }

    book_t *book = (book_t *)ht_get(library, title);
    // Verifica daca definitia exista in carte

    if (!ht_has_key(book->contents, def_key)) {
        fputs(MSG_NODEF, stdout);
        return;
    }

    free(ht_remove_entry(book->contents, def_key));
}

void add_user(hashtable_t *users, char *it)
{
    // Parseaza numele utilizatorului
    char username[USERNAME_LEN] = {0};
    get_next_arg(&it, username);

    // Verifica daca utilizatorul exista in sistem
    if (ht_has_key(users, username)) {
        fputs(MSG_USERREG, stdout);
        return;
    }

    // Creaza utilizatorul
    user_t user;
    init_user(&user, username);

    // Adauga utilizatorul in sistem
    ht_put(users, username, &user, free_user);
}

void borrow(hashtable_t *users, hashtable_t *library, char *it)
{
    // Parseaza argumentele
    char username[USERNAME_LEN] = {0}, title[BOOK_LEN] = {0};
    char days_buf[NUM_LEN] = {0};
    get_next_arg(&it, username);
    get_next_arg(&it, title);
    get_next_arg(&it, days_buf);
    int days = atoi(days_buf);

    // Verifica daca utilizatorul e inregistrat
    if (!ht_has_key(users, username)) {
        fputs(MSG_USERNOTREG, stdout);
        return;
    }

    // Verifica daca utilizatorul e banat
    user_t *user = (user_t *)ht_get(users, username);
    if (user->points < 0) {
        fputs(MSG_USERBANNED, stdout);
        return;
    }

    if (user->borrowed_title != NULL) {
        fputs(MSG_USERBORROW, stdout);
        return;
    }

    // Verifica daca cartea exista
    if (!ht_has_key(library, title)) {
        fputs(MSG_NOBOOK, stdout);
        return;
    }

    // Verifica daca cartea este imprumutata
    book_t *book = (book_t *)ht_get(library, title);
    if (book->borrowed) {
        fputs(MSG_BOOKBORROW, stdout);
        return;
    }

    // Imprumuta cartea
    user->borrowed_title = dup_str(title);
    user->borrowed_deadline = days;

    book->borrowed = 1;
}

void ret_book(hashtable_t *users, hashtable_t *library, char *it)
{
    // Parseaza argumentele
    char username[USERNAME_LEN] = {0}, title[BOOK_LEN] = {0};
    char days_buf[NUM_LEN] = {0}, rating_buf[NUM_LEN] = {0};
    get_next_arg(&it, username);
    get_next_arg(&it, title);
    get_next_arg(&it, days_buf);
    get_next_arg(&it, rating_buf);
    int days = atoi(days_buf);
    int rating = atoi(rating_buf);

    // Verifica daca utilizatorul este banat
    user_t *user = (user_t *)ht_get(users, username);
    if (user->points < 0) {
        fputs(MSG_USERBANNED, stdout);
        return;
    }

    // Verifica daca utilizatorul returneaza cartea specificata
    if (user->borrowed_title == NULL || strcmp(user->borrowed_title, title)) {
        fputs(MSG_BOOKNOMATCH, stdout);
        return;
    }

    // Actualizeaza scorul utilizatorului
    int diff = user->borrowed_deadline - days;
    if (diff > 0) {
        user->points += diff;
    } else if (diff < 0) {
        user->points += 2 * diff;   // Diferenta e negativa, punctajul se scade

        if (user->points < 0)
            ban_hammer(username);
    }
    free(user->borrowed_title);
    user->borrowed_title = NULL;
    user->borrowed_deadline = 0;

    // Actualizeaza ratingul cartii
    book_t *book = (book_t *)ht_get(library, title);
    book->ratings++;
    book->total_points += rating;
    book->borrowed = 0;
}

void lost(hashtable_t *users, hashtable_t *library, char *it)
{
    // Parseaza argumentele
    char username[USERNAME_LEN] = {0}, title[BOOK_LEN] = {0};
    get_next_arg(&it, username);
    get_next_arg(&it, title);

    // Verifica daca utilizatorul exista in sistem
    if (!ht_has_key(users, username)) {
        fputs(MSG_USERNOTREG, stdout);
        return;
    }

    // Verifica daca utilizatorul e banat
    user_t *user = (user_t *)ht_get(users, username);
    if (user->points < 0) {
        fputs(MSG_USERBANNED, stdout);
        return;
    }

    // Sterge cartea din sistem
    free_book(ht_remove_entry(library, title));

    // Actualizeaza statisticile userului
    user->borrowed_deadline = 0;
    user->points -= 50;
    free(user->borrowed_title);
    user->borrowed_title = NULL;

    if (user->points < 0)
        ban_hammer(username);
}
