#include "structure_arbre_arborescente.h"
#include "assert.h"

CellTree *create_node(Block *b)
{
  CellTree *cell = (CellTree *)malloc(sizeof(CellTree));
  if (cell == NULL)
  {
    printf("Erreur lors de l'allocation dans create_node");
  }
  cell->block = b;
  cell->father = NULL;
  cell->firstChild = NULL;
  cell->nextBro = NULL;
  cell->height = 0;

  return cell;
}

int update_height(CellTree *father, CellTree *child)
{
  if (father->height <= child->height)
  {
    father->height = child->height + 1;
    return 1;
  }

  //Sinon, le père a toujours une hauteur supérieure à celle de son enfant
  return 0;
}

// Q 8.3
void add_child(CellTree *father, CellTree *child)
{
  if ((father == NULL) || (child == NULL))
  {
    fprintf(stderr, "Erreur dans la fonction add_child : père ou enfant NULL \n");
    return;
  }
  //Dans un premier temps, nous allons ajouter en tête le fils
  child->nextBro = father->firstChild;
  father->firstChild = child;

  // Nous allons ensuite modifier la hauteur des parents
  CellTree *father_temp = father;
  CellTree *child_temp = father->firstChild;
  while (father_temp)
  {
    update_height(father_temp, child_temp);
    father_temp = father_temp->father;
    child_temp = child_temp->father;
  }
}

//Q 8.4
void print_tree(CellTree *racine)
{
  // Cas où l'arbre est vide
  if (racine == NULL)
  {
    return;
  }

  printf("hauteur du block : %d, et son identifiant : %s \n", racine->height, racine->block->hash);

  print_tree(racine->nextBro);
  print_tree(racine->firstChild);
}

// Q 8.5.1
void delete_node(CellTree *node)
{
  delete_block(node->block);
  free(node);
}

//Q 8.5.2
void delete_tree(CellTree *tree)
{ // changer
  if (tree != NULL)
  {
    CellTree *brothers = tree->nextBro;
    CellTree *children = tree->firstChild;

    //on supprime le noeud courant
    delete_node(tree);

    //on supprime ses frères puis ses fils
    delete_tree(brothers);
    delete_tree(children);
  }
}

void free_node(CellTree *node)
{
  free_block(node->block);
  free(node);
}

void free_tree(CellTree *tree)
{
  if (tree != NULL)
  {
    free_tree(tree->firstChild);
    free_tree(tree->nextBro);
    free_node(tree);
  }
}

// Q 8.6
CellTree *highest_child(CellTree *cell)
{
  // Cas arbre vide ou branche
  if ((cell == NULL) || (cell->firstChild == NULL))
  {
    return NULL;
  }

  CellTree *highest_child = cell->firstChild;
  CellTree *brother = highest_child->nextBro;

  while (brother != NULL)
  {
    if (brother->height > highest_child->height)
    {
      highest_child = brother;
    }

    brother = brother->nextBro;
  }

  return highest_child;
}

// Q 8.7
CellTree *last_node(CellTree *tree)
{

  if (tree == NULL)
  {
    return NULL;
  }

  if (tree->firstChild == NULL)
  {
    return tree;
  }

  CellTree *highestChild = highest_child(tree);

  return last_node(highestChild);
}

// Q 8.8

void fusion_cell_protected(CellProtected *first, CellProtected *second)
{
  if (first == NULL)
  {
    first = second;
    return;
  }
  CellProtected *tmp = first;
  while (tmp->next != NULL)
  {
    tmp = tmp->next;
  }
  tmp->next = second;
}

// Q 8.9

CellProtected *fusion_highest_CP(CellTree *racine)
{
  if (racine == NULL)
  {
    return NULL;
  }

  CellProtected *liste = NULL;
  fusion_list_protected(&liste, &((racine->block)->votes));

  CellTree *highestChild = highest_child(racine);
  while (highestChild != NULL)
  {
    fusion_list_protected(&liste, &((highestChild->block)->votes));
    highestChild = highest_child(highestChild);
  }

  return liste;
}
