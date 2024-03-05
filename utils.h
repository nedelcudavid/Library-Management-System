// Copyright 2022 <314CA Nedelcu Andrei David>
#ifndef UTILS_H_
#define UTILS_H_

#define USERNAME_LEN 20
#define BOOK_LEN 40
#define DEF_LEN 20
#define CMD_LEN 10
#define NUM_LEN 10

#define MSG_NOBOOK      "The book is not in the library.\n"
#define MSG_NODEF       "The definition is not in the book.\n"
#define MSG_USERREG     "User is already registered.\n"
#define MSG_USERNOTREG  "You are not registered yet.\n"
#define MSG_USERBANNED  "You are banned from this library.\n"
#define MSG_BOOKBORROW  "The book is borrowed.\n"
#define MSG_USERBORROW  "You have already borrowed a book.\n"
#define MSG_BOOKNOMATCH "You didn't borrow this book.\n"

char *dup_str(const char *string);

char *get_next_arg(char **iterator, char *buffer);

void ban_hammer(char *username);

#endif  // UTILS_H_
