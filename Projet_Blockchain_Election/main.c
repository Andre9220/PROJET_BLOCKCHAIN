#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include "primalite.h"
#include "proto_rsa.h"
#include "manip_struct.h"
#include "simulation_ex4.h"
#include "stockage_dans_les_listes.h"
#include "gagnant.h"
#include "structure_du_block.h"
#include <openssl/sha.h>
#include "simulation_process-vote.h"
#include "structure_arbre_arborescente.h"
#include <assert.h>
#include <time.h>
#include <dirent.h>

#define A 8
#define N 18
#define MIN 0
#define MAX 10000
#define STEP 100

void test_ex1()
{
  srand(time(NULL));
  clock_t tempsINITIAL, tempsFINAL;
  double t1, t2;

  // Testons la fonctions is_prime naive

  int res = 0;
  t1 = 0;

  // La fonction s'assure que les nombres impairs sont premiers et on les affiche
  for (long prime = 3; t1 <= 0.002; prime += 2)
  {
    tempsINITIAL = clock();
    res = is_prime_naive(prime);
    tempsFINAL = clock();
    t1 = ((double)(tempsFINAL - tempsINITIAL)) / CLOCKS_PER_SEC;

    if (res == 1)
    {
      printf("%ld %d %f\n", prime, res, t1);
    }
  }

  FILE *fichier1;
  fichier1 = fopen("ResultatModPow.txt", "w");

  if (fichier1 == NULL)
  {
    printf("Erreur lors de l'ouverture dufichier  ResultatModPow.txt ");
    exit(EXIT_FAILURE);
  }

  //On compare la méthode modpow naive à celle efficace
  for (long i = MIN; i <= MAX; i += STEP)
  {

    tempsINITIAL = clock();
    modpow_naive(A, i, N);
    tempsFINAL = clock();
    t1 = ((double)(tempsFINAL - tempsINITIAL)) / CLOCKS_PER_SEC;

    tempsINITIAL = clock();
    modpow(A, i, N);
    tempsFINAL = clock();
    t2 = ((double)(tempsFINAL - tempsINITIAL)) / CLOCKS_PER_SEC;

    fprintf(fichier1, "%ld %f %f\n", i, t1, t2);
  }
}

void test_ex2()
{
  srand(time(NULL));
  //Generation de cle :
  long p = random_prime_number(3, 7, 5000);
  long q = random_prime_number(3, 7, 5000);
  while (p == q)
  {
    q = random_prime_number(3, 7, 5000);
  }
  long n, s, u;
  generate_key_values(p, q, &n, &s, &u);
  //Pour avoir des cles positives :
  if (u < 0)
  {
    long t = (p - 1) * (q - 1);
    u = u + t; //on aura toujours s*u mod t = 1
  }
  //Afichage des cles en hexadecimal
  printf("cle publique = (%lx , %lx ) \n", s, n);
  printf("cle privee = (%lx , %lx) \n", u, n);

  //Chiffrement:
  char mess[10] = "Hello";
  int len = strlen(mess);
  long *crypted = encrypt(mess, s, n);

  printf("Initial message : %s \n", mess);
  printf("Encoded representation : ");
  print_long_vector(crypted, len);
  printf("\n");
  //Dechiffrement:

  char *decoded = decrypt(crypted, len, u, n);
  printf("Decoded : %s\n", decoded);

  // Libération de la mémoire
  free(crypted);
  free(decoded);
}

void test_ex3()
{
  Key *sKey = (Key *)malloc(sizeof(Key));
  Key *pKey = (Key *)malloc(sizeof(Key));
  init_pair_keys(pKey, sKey, 3, 7);
  printf("pkey: %lx, %lx \n", pKey->val, pKey->n);
  printf("pkey: %lx, %lx \n", sKey->val, pKey->n);

  //Testing Key Serialization
  char *chaine = key_to_str(pKey);
  printf("key_to_str: %s \n", chaine);
  Key *k = str_to_key(chaine);
  printf("str_to_key: %lx, %lx \n", k->val, k->n);

  free(chaine);

  //Testing signature
  //Candidate keys

  Key *pKeyC = (Key *)malloc(sizeof(Key));
  Key *sKeyC = (Key *)malloc(sizeof(Key));
  init_pair_keys(pKeyC, sKeyC, 3, 7);
  //Declaration :
  char *mess = key_to_str(pKeyC);

  char *temp = key_to_str(pKey);

  printf("%s vote pour %s\n", temp, mess);

  free(temp);

  Signature *sgn = sign(mess, sKey);
  printf("signature: ");
  print_long_vector(sgn->tableau, sgn->taille);
  chaine = signature_to_str(sgn);
  printf("\n");
  printf("signature_to_str: %s \n", chaine);

  free(sgn->tableau);
  free(sgn);

  sgn = str_to_signature(chaine);
  printf("str_to_signature:  ");
  print_long_vector(sgn->tableau, sgn->taille);
  printf("\n");

  free(chaine);

  Protected *pr = init_protected(pKey, mess, sgn);
  if (verify(pr))
  {
    printf("Signature valide \n");
  }
  else
  {
    printf("Signature non valide\n");
  }
  chaine = protected_to_str(pr);
  printf("protected_to_str: %s \n", chaine);
  pr = str_to_protected(chaine);
  printf("str to protected: %s %s %s \n", key_to_str(pr->pKey), pr->message, signature_to_str(pr->signature));

  free(pKey);
  free(sKey);
  free(pKeyC);
  free(sKeyC);
  free(k);
  free(pr);
  free(chaine);
  free(mess);
}

void test_read()
{

  printf("Liste citoyens :\n");
  CellKey *cle_vote = read_public_keys("keys.txt");
  print_list_keys(cle_vote);
  delete_list_keys(cle_vote);

  printf("\n");

  printf("Liste candidats:\n");

  CellKey *candidateKeys = read_public_keys("candidates.txt");
  print_list_keys(candidateKeys);
  delete_list_keys(candidateKeys);

  printf("\n");

  CellProtected *declarations = read_protected("declarations.txt");
  puts("AFFICHAGE DE CLÉ \n");
  print_list_protected_keys(declarations);

  delete_fake_protected(&declarations);

  puts("AFFICHAGE DE CLÉ apres LA FRAUDE \n ");
  print_list_protected_keys(declarations);

  delete_list_protected(declarations);
}

void test_hachage()
{

  int nbCandidates = 3, nbElecteurs = 10;

  srand(time(NULL));
  generate_random_data(nbElecteurs, nbCandidates);

  CellKey *candidates = read_public_keys("candidates.txt");
  CellKey *publicKeys = read_public_keys("keys.txt");
  CellProtected *votes = read_protected("declarations.txt");

  HashTable *t2 = create_hashtable(candidates, nbCandidates);
  HashTable *t3 = create_hashtable(publicKeys, 2 * nbElecteurs);
  printf("Tables crées\n");
  afficher_tableH(t2);
  afficher_tableH(t3);

  Key *lol = compute_winner(votes, candidates, publicKeys, nbCandidates * 2, nbElecteurs * 2);
  char *gagnant = key_to_str(lol);
  printf("le gagnant est : %s\n", gagnant);
  printf("FIN COMPUTE_WINNER\n");

  delete_hashtable(t3);
  delete_hashtable(t2);
  delete_list_keys(candidates);
  delete_list_keys(publicKeys);
}

void test_decryptSHA()
{
  const char *s = "Rosetta code";
  printf("\nAffichage du message chiffre de l'enonce :\n");
  unsigned char *d = SHA256((const unsigned char *)s, strlen(s), 0);
  int i;
  for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
  {
    printf("%02x", d[i]);
  }
  putchar('\n');

  //decrypte SHA
  printf("\nAffichage du message chiffre  avec la fonction decrypt_sha: \n");
  unsigned char *res = decrypt_sha(s);
  printf("%s \n", res);
  free(res);
}

void test_block()
{

  Block *b = (Block *)malloc(sizeof(Block));
  b->author = (Key *)malloc(sizeof(Key));
  b->hash = NULL;
  b->previous_hash = NULL;
  b->votes = read_protected("declarations.txt");

  long p = random_prime_number(3, 7, 5000);
  long q = random_prime_number(3, 7, 5000);

  while (p == q)
  {
    q = random_prime_number(3, 7, 5000);
  }

  long n, s, u;
  generate_key_values(p, q, &n, &s, &u);

  if (u < 0)
  {
    long t = (p - 1) * (q - 1);
    u = u + t;
  }

  init_key(b->author, s, n);
  FILE *f = fopen("Graphique_compute.txt", "w");

  if (f == NULL)
  {
    printf("Erreur lors de l'ouverture du fichier ");
    exit(EXIT_FAILURE);
  }
  clock_t temps_INITIAL, temps_FINAL;
  double t1 = 0;

  for (int pow = 0; t1 < 1.0; pow++)
  {
    temps_INITIAL = clock();
    compute_proof_of_work(b, pow);
    temps_FINAL = clock();

    t1 = ((double)(temps_FINAL - temps_INITIAL)) / CLOCKS_PER_SEC;
    fprintf(f, "%d %f\n", pow, t1);

    printf("Verify: %d %d\n\n", b->nonce, verify_block(b, pow));
  }

  fclose(f);

  write_fichier("test_un_block.txt", b);
  Block *b2 = read_block("test_un_block.txt");
  printf("Un peu de patience la lecture  du block est entrain de s'effectuer");
  char *str = block_to_str(b);
  char *str2 = block_to_str(b2);
  puts("\n");
  printf("Block 1 : %s\n", str);
  puts("\n");
  printf("Block 2 : %s\n", str2);

  free(str);
  free(str2);
  free_block(b);
  free_block(b2);
}

#define SIZEV 2000
#define SIZEC 50
void test_final()
{
  srand(time(NULL));

  generate_random_data(1000, 5);

  CellProtected *decl = read_protected("declarations.txt");
  CellKey *candidates = read_public_keys("candidates.txt");
  CellKey *voters = read_public_keys("keys.txt");

  CellTree *tree = NULL, *tmp;
  CellProtected *pr = decl;
  CellKey *author = voters;

  char name[256];
  char name2[256];
  char n[32];
  char ext[8];

  int cpt = 1, i = 1;

  strcpy(ext, ".txt");

  while (pr != NULL)
  {
    submit_vote(pr->data);
    // Creation de un block tout les 10 votants
    if (cpt % 10 == 0)
    {

      strcpy(name, "./Blockchain/Block");
      sprintf(n, "%d", i++);
      strcat(name, n);
      strcat(name, ext);

      strcpy(name2, "Block");
      strcat(name2, n);
      strcat(name2, ext);

      create_block(tree, author->data, 3);
      add_block(3, name2);

      if (tree == NULL)
      {
        tree = create_node(read_block(name));
      }

      else
      {
        tmp = tree;

        while (tmp->firstChild != NULL)
        {
          tmp = tmp->firstChild;
        }

        add_child(tmp, create_node(read_block(name)));
      }
    }

    cpt++;
    author = author->next;
    pr = pr->next;
  }
  puts("\n");
  // Lecture de l'abre
  CellTree *t = read_tree();
  printf("Arbre final d'election  :\n");
  print_tree(t);

  Key *winner = compute_winner_BT(tree, candidates, voters, SIZEC, SIZEV);
  char *str = key_to_str(winner);
  printf("Le vainqueur est: %s\n", str);

  free(str);
  free_tree(tree);
  free_tree(t);
  delete_list_protected(decl);
  delete_list_keys(candidates);
  delete_list_keys(voters);
}

int main(void)
{
  srand(time(NULL));
  printf("========TEST EXERCICE 1======== \n");
  //test_ex1();
  puts("\n");
  printf("========TEST EXERCICE 2======== \n");
  test_ex2();
  puts("\n");
  printf("========TEST EXERCICE 3======== \n");
  test_ex3();
  puts("\n");
  //Test de la fonction 4;
  //generate_random_data(20,5);
  printf("========TEST LECTURE======== \n");
  test_read();
  puts("\n");
  printf("========TEST HACHAGE========= \n");
  test_hachage();
  puts("\n");
  printf("========TEST DECRYPTAGE ======= \n");
  test_decryptSHA();
  puts("\n");
  printf("========TEST BLOCK (patientez quelques secondes SVP)======= \n");
  test_block();
  puts("\n");
  printf("========TEST FINAL ======= \n");
  test_final();

  return 0;
}