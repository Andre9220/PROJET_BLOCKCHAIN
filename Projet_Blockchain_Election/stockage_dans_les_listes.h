#ifndef STOCKAGEINLIST_H
#define STOCKAGEINLIST_H

#include <stdlib.h>
#include <stdio.h>
#include "manip_struct.h"

typedef struct cellKey
{
    Key *data;
    struct cellKey *next;
} CellKey;

CellKey *create_cell_key(Key *key);
void ajout_cle_en_tete(CellKey **LCK, Key *key);

CellKey *read_public_keys(char *nom_fichier);
void print_list_keys(CellKey *LCK);
void delete_cell_key(CellKey *c);
void delete_list_keys(CellKey *LCK);

typedef struct cellProtected
{
    Protected *data;
    struct cellProtected *next;
} CellProtected;

CellProtected *create_cell_protected(Protected *pr);

void insert_cell_protected(CellProtected **list, Protected *pr);
CellProtected *read_protected(char *filename);

void print_list_protected_keys(CellProtected *LPK);
void delete_cell_protected(CellProtected *c);
void delete_list_protected(CellProtected *LPK);
void fusion_list_protected(CellProtected **list1, CellProtected **list2);

#endif //STOCKAGEINLIST_H