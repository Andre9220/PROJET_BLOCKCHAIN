#ifndef GAGNANT_H
#define GAGNANT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "stockage_dans_les_listes.h"

typedef struct hashcell
{
    Key *key;
    int val;
} HashCell;

typedef struct hashtable
{
    HashCell **tab;
    int size;
} HashTable;

void delete_fake_protected(CellProtected **LCK);
HashCell *create_hashcell(Key *key);
int hash_function(Key *key, int size);
int find_position(HashTable *t, Key *key);
HashTable *create_hashtable(CellKey *keys, int size);
void delete_hash_cellule(HashCell *hashcell);
void delete_hashtable(HashTable *t);

Key *compute_winner(CellProtected *decl, CellKey *candidates, CellKey *voters, int sizeC, int sizeV);
void afficher_tableH(HashTable *t);

#endif //GAGNANT_H