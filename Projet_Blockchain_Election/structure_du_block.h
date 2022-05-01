#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <stdio.h>
#include "proto_rsa.h"
#include "stockage_dans_les_listes.h"
#include <openssl/sha.h>

typedef struct block
{
    Key *author;
    CellProtected *votes;
    unsigned char *hash;
    unsigned char *previous_hash;
    int nonce;
} Block;

void write_fichier(char *filename, Block *block);
Block *read_block(char *filename);
char *block_to_str(Block *block);
unsigned char *decrypt_sha(const char *chaine);
int verify_block(Block *b, int d);
void compute_proof_of_work(Block *b, int d);
void free_block(Block *b);
void delete_block(Block *b);
#endif //BLOCK_H
