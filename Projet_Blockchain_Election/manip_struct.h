#ifndef KEY_H
#define KEY_H

#include "proto_rsa.h"

typedef struct
{
    long val;
    long n;
} Key;

void init_key(Key *key, long val, long n);
void init_pair_keys(Key *pKey, Key *sKey, int low_size, int up_size);
char *key_to_str(Key *key);
Key *str_to_key(char *str);

typedef struct
{
    long *tableau;
    int taille;
} Signature;

Signature *init_signature(long *content, int size);
Signature *str_to_signature(char *str);
Signature *sign(char *mess, Key *sKey);
char *signature_to_str(Signature *sgn);

typedef struct
{
    Key *pKey;
    Signature *signature;
    char *message;
} Protected;

Protected *init_protected(Key *pKey, char *mess, Signature *sgn);
int verify(Protected *pr);
char *protected_to_str(Protected *pr);
Protected *str_to_protected(char *str);
void free_signature(Signature *sgn);

#endif //KEY_H