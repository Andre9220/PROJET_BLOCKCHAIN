#ifndef STRUCTURE_ARBRE_ARBO_H
#define STRUCTURE_ARBRE_ARBO_H

#include "structure_du_block.h"

typedef struct block_tree_cell
{
    Block *block;
    struct block_tree_cell *father;
    struct block_tree_cell *firstChild;
    struct block_tree_cell *nextBro;
    int height;
} CellTree;

CellTree *create_node(Block *b);
int update_height(CellTree *father, CellTree *child);
void add_child(CellTree *father, CellTree *child);
void print_tree(CellTree *racine);

void delete_node(CellTree *node);
void delete_tree(CellTree *tree);

void free_node(CellTree *node);
void free_tree(CellTree *tree);

CellTree *highest_child(CellTree *cell);
CellTree *last_node(CellTree *tree);

void fusion_cell_protected(CellProtected *first, CellProtected *second);
CellProtected *fusion_highest_CP(CellTree *racine);
#endif //STRUCTURE_ARBRE_ARBO_H
