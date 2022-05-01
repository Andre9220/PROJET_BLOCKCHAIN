#ifndef SIMULATIONT_H
#define SIMULATIONT_H

#include "structure_du_block.h"
#include "structure_arbre_arborescente.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gagnant.h"

void submit_vote(Protected *p);
void create_block(CellTree *tree, Key *author, int d);
void add_block(int d, char *name);

CellTree *read_tree();
Key *compute_winner_BT(CellTree *tree, CellKey *candidates, CellKey *voters, int sizeC, int sizeV);

#endif //SIMULATIONT_H