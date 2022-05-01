#include "stockage_dans_les_listes.h"
#include "assert.h"

CellKey *create_cell_key(Key *key)
{

  CellKey *cell_key = (CellKey *)malloc(sizeof(CellKey));

  if (cell_key == NULL)
  {
    printf("Erreur de l'allocation dans la fonction create_cell_key \n ");
    return NULL;
  }

  cell_key->data = key;
  cell_key->next = NULL;

  return cell_key;
}

void ajout_cle_en_tete(CellKey **LCK, Key *key)
{
  if (key == NULL)
  {
    return;
  }
  CellKey *cell = create_cell_key(key);
  cell->next = *LCK;
  *LCK = cell;
}

CellKey *read_public_keys(char *filename)
{
  FILE *fichier = fopen(filename, "r");

  if (fichier == NULL)
  {
    fprintf(stderr, "Impossible d'ouvrir le fichier nom_fichier.txt \n");
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  char Keystr[256];
  CellKey *liste = NULL;

  while (fgets(buffer, 256, fichier))
  {
    if (sscanf(buffer, "%s", Keystr) != 1)
    {
      printf("ERREUR DANS LA CLÉ Lors de la lecture ");
      exit(EXIT_FAILURE);
    }

    ajout_cle_en_tete(&liste, str_to_key(Keystr));
  }

  fclose(fichier);
  return liste;
}

void print_list_keys(CellKey *LCK)
{
  while (LCK)
  {
    printf("%s\n", key_to_str(LCK->data));
    LCK = LCK->next;
  }
}

void delete_cell_key(CellKey *c)
{

  if (c == NULL)
  {
    return;
  }
  free(c->data);
  c->next = NULL;
  free(c);
}

void delete_list_keys(CellKey *LCK)
{
  CellKey *tmp;

  while (LCK != NULL)
  {
    tmp = LCK;
    LCK = LCK->next;
    delete_cell_key(tmp);
  }
}

CellProtected *create_cell_protected(Protected *pr)
{

  CellProtected *cell_protected = (CellProtected *)malloc(sizeof(CellProtected));

  if (cell_protected == NULL)
  {
    printf("Erreur de l'allocation dans la fonction create_cell_protected \n");
    return NULL;
  }

  cell_protected->data = pr;
  cell_protected->next = NULL;

  return cell_protected;
}

void insert_cell_protected(CellProtected **list, Protected *pr)
{
  CellProtected *cell = create_cell_protected(pr);
  cell->next = *list;
  *list = cell;
}

CellProtected *read_protected(char *filename)
{
  FILE *f = fopen(filename, "r");

  if (f == NULL)
  {
    fprintf(stderr, "Impossible d'ouvrir le fichier nom_fichier.txt \n");
    exit(EXIT_FAILURE);
  }

  char buffer[256];
  char prstr[256];
  CellProtected *list = NULL;

  while (fgets(buffer, 256, f) != NULL)
  {
    if (sscanf(buffer, "%[^\n]", prstr) != 1)
    {
      printf("ERREUR DANS LA CLÉ Lors de la lecture ");
      exit(EXIT_FAILURE);
    }

    insert_cell_protected(&list, str_to_protected(prstr));
  }

  fclose(f);
  return list;
}

void print_list_protected_keys(CellProtected *LPK)
{
  while (LPK)
  {

    char *str = protected_to_str(LPK->data);
    printf("%s\n", str);
    free(str);
    LPK = LPK->next;
  }
}

void delete_cell_protected(CellProtected *c)
{

  if (c == NULL)
  {
    return;
  }
  free(c->data);
  free(c);
}

void delete_list_protected(CellProtected *LPK)
{
  CellProtected *tmp;

  while (LPK)
  {
    tmp = LPK;
    LPK = LPK->next;
    delete_cell_protected(tmp);
  }
}

void fusion_list_protected(CellProtected **list1, CellProtected **list2)
{

  if (*list1 == NULL)
  {
    *list1 = *list2;
    *list2 = NULL;
  }

  CellProtected *c = *list1;

  while (c->next != NULL)
  {
    c = c->next;
  }

  c->next = *list2;
  *list2 = NULL;
}
