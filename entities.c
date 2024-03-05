// Copyright 2022 <314CA Nedelcu Andrei David>
#include "entities.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_user(user_t *user, char *name)
{
    user->borrowed_deadline = 0;
    user->borrowed_title = NULL;
    user->name = dup_str(name);
    user->points = 100;
}

void init_book(book_t *book, char *title)
{
    book->borrowed = 0;
    book->contents = ht_create(hash_string, cmpr_strings, DEF_LEN, DEF_LEN);
    book->ratings = 0;
    book->total_points = 0;
    book->title = dup_str(title);
}

void free_user(void *user)
{
    free(((user_t *)user)->name);
    free(((user_t *)user)->borrowed_title);
    free(user);
}

void free_book(void *book)
{
    free(((book_t *)book)->title);
    ht_free(((book_t *)book)->contents, free);
    free(book);
}

// Compara utilizatorii in vederea sortarii in clasament
int cmp_user(const void *u1, const void *u2)
{
    user_t *user1 = *(user_t **)u1;
    user_t *user2 = *(user_t **)u2;

    int rc = user2->points - user1->points;
    if (rc)
        return rc;
    return strcmp(user1->name, user2->name);
}

// Compara cartile in vederea sortarii in clasament
int cmp_book(const void *b1, const void *b2)
{
    book_t *book1 = *(book_t **)b1;
    book_t *book2 = *(book_t **)b2;

    double rating1 = get_rating(book1);
    double rating2 = get_rating(book2);

    if (rating1 > rating2)
        return -1;
    if (rating1 < rating2)
        return 1;
    int rc = book2->ratings - book1->ratings;

    if (rc)
        return rc;

    return strcmp(book1->title, book2->title);
}

void user_print(user_t *user)
{
    printf("Name:%s Points:%d\n", user->name, user->points);
}

void book_print(book_t *book)
{
    printf("Name:%s Rating:%.3lf Purchases:%d\n", book->title,
           get_rating(book), book->ratings);
}

double get_rating(book_t *book)
{
    if (!book->ratings)
        return 0;
    return (double)book->total_points / book->ratings;
}
