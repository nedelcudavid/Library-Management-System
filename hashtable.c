// Copyright 2022 <314CA Nedelcu Andrei David>
#include "hashtable.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define HT_START_SZ 5

hashtable_t *ht_create(uint64_t (*h_key)(void *), int (*cmp)(void *, void *),
					   int k_sz, int v_sz)
{
	hashtable_t *ht = (hashtable_t *)malloc(sizeof(hashtable_t));
	assert(ht);

	ht->size = 0;
	ht->hmax = HT_START_SZ;

	ht->cmp = cmp;
	ht->h_key = h_key;
	ht->k_sz = k_sz;
	ht->v_sz = v_sz;

	ht->buckets = (entry_t **)calloc(ht->hmax, sizeof(entry_t *));

	return ht;
}

static void __ht_resize(hashtable_t *ht, int new_cap)
{
	// Aloca noul vector de bucketuri
	entry_t **new_buckets = (entry_t **)calloc(new_cap, sizeof(entry_t *));
	assert(new_buckets);

	for (int i = 0; i < ht->hmax; i++) {
		while (ht->buckets[i]) {
			// Extrage primul nod din bucket
			entry_t *node = ht->buckets[i];
			ht->buckets[i] = node->next;

			// Introdu nodul in noul vector, pe pozitia corecta
			int idx = ht->h_key(node->key) % new_cap;
			node->next = new_buckets[idx];
			new_buckets[idx] = node;
		}
	}

	free(ht->buckets);
	ht->buckets = new_buckets;

	ht->hmax = new_cap;
}

void *ht_get(hashtable_t *ht, void *key)
{
	// Afla indicele bucketului unde se plaseaza cheia
	int idx = ht->h_key(key) % ht->hmax;

	entry_t *node = ht->buckets[idx];

	// Parcurge elementele bucketului, cautand cheia dorita
	while (node) {
		void *entry_key = node->key;
		if (!ht->cmp(key, entry_key)) {
			// S-a gasit elementul
			return node->value;
		}
		node = node->next;
	}

	// Nu s-a gasit elementul
	return NULL;
}

int ht_has_key(hashtable_t *ht, void *key)
{
	return ht_get(ht, key) != NULL;
}

void ht_put(hashtable_t *ht, void *key, void *value, void (*f_value)(void *))
{
	int idx = ht->h_key(key) % ht->hmax;

	// Verifica daca cheia se gaseste in tabela
	entry_t **node = &ht->buckets[idx];
	while (*node) {
		if (!ht->cmp(key, (*node)->key)) {
			// S-a gasit cheia, inlocuieste valoarea asociata acesteia
			f_value((*node)->value);
			(*node)->value = malloc(ht->v_sz);
			assert((*node)->value);

			memcpy((*node)->value, value, ht->v_sz);
			return;
		}

		node = &(*node)->next;
	}

	// Cheia nu exista, acum node indica catre adresa unde se poate adauga
	// Noua intrare
	*node = (entry_t *)malloc(sizeof(entry_t));
	assert(*node);

	// Creaza noua intrare in tabela
	(*node)->key = malloc(ht->k_sz);
	(*node)->value = malloc(ht->v_sz);
	(*node)->next = NULL;
	assert((*node)->key && (*node)->value);

	memcpy((*node)->key, key, ht->k_sz);
	memcpy((*node)->value, value, ht->v_sz);

	ht->size++;
	if (ht->size > ht->hmax)
		__ht_resize(ht, ht->hmax * 2);
}

void *ht_remove_entry(hashtable_t *ht, void *key)
{
	if (!ht_has_key(ht, key))
		return NULL;

	// Determina bucketul in care se regaseste cheia
	int idx = ht->h_key(key) % ht->hmax;

	entry_t **node = &ht->buckets[idx];
	void *ret_val = NULL;

	// Cauta nodul cu cheia ce se doreste a fi stersa
	while (*node) {
		if (!ht->cmp((*node)->key, key)) {
			// S-a gasit cheia, pastreaza valoarea si inlocuieste nodul
			// In care se afla aceasta cu urmatorul
			ret_val = (*node)->value;

			free((*node)->key);
			entry_t *next = (*node)->next;
			free(*node);
			*node = next;

			break;
		}

		node = &(*node)->next;
	}

	assert(ret_val);
	ht->size--;

	if (ht->hmax > HT_START_SZ && ht->size < ht->hmax / 2)
		__ht_resize(ht, ht->hmax / 2);

	return ret_val;
}

void ht_free(hashtable_t *ht, void (*f_value)(void *))
{
	// Parcurge toate bucketurile din tabela
	for (int i = 0; i < ht->hmax; i++) {
		// Pentru fiecare bucket, extrage si sterge primul element pana cand
		// acesta devine gol
		while (ht->buckets[i]) {
			entry_t *entry = ht->buckets[i];
			entry_t *save = entry->next;

			free(entry->key);
			f_value(entry->value);
			free(entry);

			ht->buckets[i] = save;
		}
	}

	free(ht->buckets);
	free(ht);
}

// Intoarce un vector cu valorile din tabela (sau NULL daca tabela e goala)
void **ht_records(hashtable_t *ht)
{
	if (!ht->size)
		return NULL;

	// Aloca memorie pentru vectorul de intrari
	void **records = (void **)malloc(ht->size * sizeof(void *));
	assert(records);

	int rec_no = 0;

	// Parcurge toate bucketurile; Pentru fiecare nod, copiaza referinta
	// In vector
	for (int i = 0; i < ht->hmax; i++) {
		entry_t *node = ht->buckets[i];

		while (node) {
			records[rec_no++] = node->value;

			node = node->next;
		}
	}

	assert(rec_no == ht->size);
	return records;
}

int ht_get_size(hashtable_t *ht)
{
	return ht->size;
}

int cmpr_strings(void *a, void *b)
{
	return strcmp((char *)a, (char *)b);
}

uint64_t hash_string(void *a)
{
	unsigned char *puchar_a = (unsigned char*) a;
	uint64_t hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}
