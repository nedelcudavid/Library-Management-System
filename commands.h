// Copyright 2022 <314CA Nedelcu Andrei David>
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "hashtable.h"

void add_book(hashtable_t *library, char *it);

void get_book(hashtable_t *library, char *it);

void rmv_book(hashtable_t *library, char *it);

void add_def(hashtable_t *library, char *it);

void get_def(hashtable_t *library, char *it);

void rmv_def(hashtable_t *library, char *it);

void add_user(hashtable_t *users, char *it);

void borrow(hashtable_t *users, hashtable_t *library, char *it);

void ret_book(hashtable_t *users, hashtable_t *library, char *it);

void lost(hashtable_t *users, hashtable_t *library, char *it);

#endif  // COMMANDS_H_
