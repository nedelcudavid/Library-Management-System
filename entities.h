// Copyright 2022 <314CA Nedelcu Andrei David>
#ifndef ENTITIES_H_
#define ENTITIES_H_

#include "hashtable.h"

typedef struct user_t {
    char *name;
    int points;
    int borrowed_deadline;
    char *borrowed_title;
} user_t;

typedef struct book_t {
    char *title;
    hashtable_t *contents;
    int total_points;
    int ratings;
    int borrowed;
} book_t;

void init_user(user_t *user, char *name);

void init_book(book_t *book, char *title);

void free_user(void *user);

void free_book(void *book);

int cmp_user(const void *u1, const void *u2);

int cmp_book(const void *b1, const void *b2);

void user_print(user_t *user);

void book_print(book_t *book);

double get_rating(book_t *book);

#endif  // ENTITIES_H_
