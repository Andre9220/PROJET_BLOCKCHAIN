#include "simulation_process-vote.h"
#include <dirent.h>
#include "gagnant.h"
#include "assert.h"

void submit_vote(Protected *p)
{
  FILE *fichier = fopen("./Blockchain/Pending_votes.txt", "a");
  if (!fichier)
  {
    fprintf(stderr, "Erreur d'ecriture du fichier , dans la fonction submit_vote\n");
    return;
  }
  char *vote = protected_to_str(p);
  fprintf(fichier, "%s\n", vote);
  free(vote);
  fclose(fichier);
}

void create_block(CellTree *tree, Key *author, int d)
{
  Block *b = (Block *)malloc(sizeof(Block));

  if (b == NULL)
  {
    printf("Erreur d'allocation du block dans la focntion create_block \n");
    return;
  }

  CellProtected *pr = read_protected("./Blockchain/Pending_votes.txt");
  Key *key = (Key *)malloc(sizeof(Key));
  if (key == NULL)
  {
    printf("Erreur d'allocation de la clé  dans la focntion create_block \n");
    return;
  }

  init_key(key, author->val, author->n);
  b->author = key;
  b->votes = pr;

  if (tree != NULL)
  {
    CellTree *lastNode = last_node(tree);

    // On effectue une liaison avec le bloc actuel avec le dernier bloc de la chaîne la plus longue
    b->previous_hash = strdup((lastNode->block)->hash);
  }

  else
  {
    b->previous_hash = NULL;
  }

  b->hash = NULL;
  compute_proof_of_work(b, d);

  remove("./Blockchain/Pending_votes.txt");

  write_fichier("./Blockchain/Pending_block.txt", b);

  free_block(b);
}

void add_block(int d, char *name)
{
  Block *b = read_block("./Blockchain/Pending_block.txt");

  int verified = verify_block(b, d);
  if (verified)
  {
    char chaine[256] = "\0";
    strcat(chaine, "./Blockchain/");
    strcat(chaine, name);
    write_fichier(chaine, b);
  }
  free_block(b);
  remove("./Blockchain/Pending_block.txt");
}

CellTree *read_tree()
{
  DIR *repertoire = opendir("./Blockchain/");
  int cpt = 0, i = 0;

  if (repertoire != NULL)
  {
    struct dirent *dir;

    //on parcourt pour connaitre le nombre de fichier
    while (dir = readdir(repertoire))
    {
      if ((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..")))
      {
        if ((strcmp(dir->d_name, "Pending_block.txt") != 0) && (strcmp(dir->d_name, "Pending_votes.txt") != 0))
        {
          cpt++;
        }
      }
    }

    // On réinitialise la position du pointeur du flux dir au debut du repertoire
    rewinddir(repertoire);

    // On alloue de l'espace mémoire pour les noeuds de l'arbre

    CellTree **tree = (CellTree **)malloc(cpt * sizeof(CellTree *));

    if (tree == NULL)
    {
      fprintf(stderr, "%s; %s; l.%d: CellTree Array Allocation Error\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
      exit(EXIT_FAILURE);
    }

    Block *block;
    CellTree *t;
    int ind = 0;
    char name[256];

    while (dir = readdir(repertoire))
    {
      if ((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..")))
      {
        if ((strcmp(dir->d_name, "Pending_block.txt") != 0) && (strcmp(dir->d_name, "Pending_votes.txt") != 0))
        {
          strcpy(name, "./Blockchain/");
          strcat(name, dir->d_name);
          block = read_block(name);
          t = create_node(block);
          tree[ind++] = t;
        }
      }
    }

    //on parcourt le tableau tree pour relier les enfants avec leur père
    for (int i = 0; i < cpt; i++)
    {
      for (int j = 0; j < cpt; j++)
      {
        //dans le cas où tree[j] est un enfant de tree[i]
        if ((((tree[j])->block)->previous_hash != NULL) && (strcmp(((tree[j])->block)->previous_hash, ((tree[i])->block)->hash) == 0))
        {
          // Adding it as its child
          add_child(tree[i], tree[j]);
        }
      }
    }

    // Dans le cas où on trouve la racine de l'arbre
    CellTree *temp;
    int i;
    for (i = 0; i < cpt; i++)
    {
      if ((tree[i])->father == NULL)
      {
        temp = tree[i];
      }
    }

    closedir(repertoire);

    free(tree);
    return temp;
  }
}

Key *compute_winner_BT(CellTree *tree, CellKey *candidates, CellKey *voters, int sizeC, int sizeV)
{
  CellProtected *decl = fusion_highest_CP(tree);
  delete_fake_protected(&decl);
  Key *winner = compute_winner(decl, candidates, voters, sizeC, sizeV);
  delete_list_protected(decl);

  return winner;
}