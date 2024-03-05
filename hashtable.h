// Copyright 2022 <314CA Nedelcu Andrei David>
#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdint.h>

typedef struct entry_t entry_t;

struct entry_t {
	void *key, *value;
	struct entry_t *next;
};

typedef struct hashtable_t hashtable_t;

struct hashtable_t {
	entry_t **buckets;
	int size, hmax;
	int k_sz, v_sz;

	uint64_t (*h_key)(void *);
	int (*cmp)(void *, void *);
};

hashtable_t *ht_create(uint64_t (*h_key)(void *), int (*cmp)(void *, void *),
					   int k_sz, int v_sz);

void *ht_get(hashtable_t *ht, void *key);

int ht_has_key(hashtable_t *ht, void *key);

void ht_put(hashtable_t *ht, void *key, void *value, void (*f_value)(void *));

void *ht_remove_entry(hashtable_t *ht, void *key);

int ht_get_size(hashtable_t *ht);

void ht_free(hashtable_t *ht, void (*f_value)(void *));

void **ht_records(hashtable_t *ht);

uint64_t hash_string(void *str);

int cmpr_strings(void *a, void *b);

#endif  // HASHTABLE_H_
