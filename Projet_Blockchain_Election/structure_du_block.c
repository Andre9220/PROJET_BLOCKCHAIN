#include "structure_du_block.h"
#include <openssl/sha.h>
#include <assert.h>

void write_fichier(char *filename, Block *block)
{
  FILE *f = fopen(filename, "w+");
  if (f == NULL)
  {
    fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
    return;
  }
  char *auteur = key_to_str(block->author); //on récupère l'auteur pour l'écrire dans le fichier plus simplement
  fprintf(f, "%s %s %s %d\n", auteur, block->hash, block->previous_hash, block->nonce);
  CellProtected *liste_v = block->votes;
  while (liste_v)
  {
    char *chaine = protected_to_str(liste_v->data);
    fprintf(f, "%s\n", chaine);
    liste_v = liste_v->next;
    free(chaine);
  }
  free(auteur);
  fclose(f);
}

Block *read_block(char *filename)
{
  FILE *f = fopen(filename, "r");

  if (f == NULL)
  {
    fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", filename);
    exit(EXIT_FAILURE);
  }

  Block *block = (Block *)malloc(sizeof(Block));

  if (block == NULL)
  {
    printf("Erreur lors de l'allocation du block\n");
    exit(EXIT_FAILURE);
  }

  unsigned char *hashage = (unsigned char *)malloc(256 * sizeof(unsigned char));
  unsigned char *prec_hash = (unsigned char *)malloc(256 * sizeof(unsigned char));

  if ((hashage == NULL) || (prec_hash == NULL))
  {
    printf("Erreur lors de l'allocation du block\n");
    exit(EXIT_FAILURE);
  }

  char auteur[256];
  char buffer[256];
  char str_to_pr[256];
  CellProtected *liste = NULL;

  if (fgets(buffer, 256, f) != NULL)
  {
    if (sscanf(buffer, "%s %s %s %d", auteur, hashage, prec_hash, &(block->nonce)) != 4)
    {
      printf("Erreur lors du sscanf\n");
      exit(EXIT_FAILURE);
    }
  }

  block->author = str_to_key(auteur);
  block->hash = hashage;

  //dans le cas où le précédent de la table de hachage n'existe pas, alors on le met à NULL
  if (strcmp(prec_hash, "NULL") == 0)
  {
    block->previous_hash = NULL;
    free(prec_hash);
  }

  else
  {
    block->previous_hash = prec_hash;
  }

  //on lit les autres lignes du fichier tant qu'elles existent
  while (fgets(buffer, 256, f) != NULL)
  {
    if (sscanf(buffer, "%[^\n]", str_to_pr) != 1)
    {
      printf("Format invalide");
      exit(EXIT_FAILURE);
    }
    //on crée et ajoute chaque déclaration dans la liste
    ajout_cle_en_tete(&liste, str_to_protected(str_to_pr));
  }

  block->votes = liste;
  fclose(f);

  return block;
}

char *block_to_str(Block *block)
{
  char *chaine = (char *)malloc(10486 * sizeof(char));

  if (chaine == NULL)
  {
    printf("Erreur lors de l'allocation");
    exit(EXIT_FAILURE);
  }

  int cpt = 0;
  char *auteur = key_to_str(block->author);
  //on va ajouter chaque caractère de la clé (auteur) dans la chaine
  for (int i = 0; i < strlen(auteur); i++)
  {
    chaine[cpt++] = auteur[i];
  }

  chaine[cpt++] = ' ';
  free(auteur);
  //si elle existe, on va ajouter la valeur de la fonction de hachage du block précédent
  if (block->previous_hash != NULL)
  {
    for (int i = 0; i < strlen(block->previous_hash); i++)
    {
      chaine[cpt++] = (block->previous_hash)[i];
    }

    chaine[cpt++] = ' ';
  }
  char *pr_to_str;
  CellProtected *cell = block->votes;

  //on parcourt les votes du block
  while (cell != NULL)
  {
    pr_to_str = protected_to_str(cell->data);
    //on ajoute les caractères de chaque déclaration dans la chaine
    for (int i = 0; i < strlen(pr_to_str); i++)
    {
      chaine[cpt++] = pr_to_str[i];
    }
    //on libère la chaine pr_to_str et on ajoute un espace entre chaque déclaration
    free(pr_to_str);
    chaine[cpt++] = ' ';
    cell = cell->next;
  }
  chaine[cpt] = '\0'; //fin de la chaine
  return chaine;
}

unsigned char *decrypt_sha(const char *chaine)
{
  unsigned char *d = SHA256(chaine, strlen(chaine), 0);
  unsigned char temp[8];

  //chaine2 sera la chaine que l'on va retourner
  unsigned char *chaine2 = (unsigned char *)malloc(256 * sizeof(unsigned char));

  //on vérifie si pas d'erreurs lors de l'allocation
  if (chaine2 == NULL)
  {
    printf("Erreur lors de l'allocation");
    exit(EXIT_FAILURE);
  }
  //fin de chaine2
  chaine2[0] = '\0';
  //on ajoute chaque caractère de d dans chaine2 au format hexadécimal
  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    sprintf(temp, "%02x", d[i]);
    strcat(chaine2, temp);
  }

  return chaine2;
}

int verify_block(Block *b, int d)
{
  char *chaine = block_to_str(b);
  unsigned char *hashage = decrypt_sha(chaine);
  for (int i = 0; i < 4 * d; i++)
  { //on vérifie qu'il y a bien 4d 0 successifs
    if (hashage[i] != 0)
    {
      return 1; //ne rempli pas les conditions du block donc on renvoie 1 (pas valide)
    }
  }
  free(chaine);
  free(hashage);
  return 0; //sinon on renvoie 0 si le block est valide
}

void compute_proof_of_work(Block *b, int d)
{
  b->nonce = 0;

  if (b->hash == NULL)
  {
    char *chaine = block_to_str(b);
    b->hash = decrypt_sha(chaine);
    free(chaine);
  }

  unsigned char temp[256];
  unsigned char nonce[128];
  int cpt = 0, i = 0;
  //on va créer une sous chaine et la hachée
  sprintf(nonce, "%d", b->nonce);
  strcpy(temp, b->hash);
  strcat(temp, nonce);
  unsigned char *decrypt = decrypt_sha(temp);

  //on vérifie que la valeur hachée possède bien d 0 successifs
  while (cpt < d)
  {
    if (decrypt[i++] == '0')
    {
      cpt++; //si on trouve un 0 on ajoute 1 au compteur
    }
    else
    //si on ne trouve pas de 0 alors on réinitialise le compteur à 0, on incrémente b->nonce de 1, et on crée une nouvelle sous-chaine
    {
      b->nonce++;
      i = 0;
      cpt = 0;
      free(decrypt);
      sprintf(nonce, "%d", b->nonce);
      strcpy(temp, b->hash);
      strcat(temp, nonce);
      decrypt = decrypt_sha(temp);
    }
  }
  printf("La sous chaine finale qui permet de rendre le block valide : %d %s\n", b->nonce, decrypt);
  free(decrypt);
}

void free_block(Block *b) //on libère l'espace mémoire du block
{
  if (!b)
    return;

  free(b->author);
  delete_list_protected(b->votes);
  free(b->hash);
  free(b->previous_hash);
  free(b);
}

void delete_block(Block *b)
{
  if (b->hash != NULL)
  {
    free(b->hash);
  }
  if (b->previous_hash != NULL)
  {
    free(b->previous_hash);
  }

  CellProtected *cell = b->votes;
  CellProtected *temp;
  //on supprime les votes du block
  while (cell != NULL)
  {
    temp = cell;
    cell = cell->next;
    free(temp);
  }

  free(b);
}
