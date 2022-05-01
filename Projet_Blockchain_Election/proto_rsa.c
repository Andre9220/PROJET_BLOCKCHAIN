#include "proto_rsa.h"

long extended_gcd(long s, long t, long *u, long *v)
{
    if (s == 0)
    {
        *u = 0;
        *v = 1;
        return t;
    }

    long uPrim, vPrim;
    long gcd = extended_gcd((t % s), s, &uPrim, &vPrim);

    *u = vPrim - (t / s) * uPrim;
    *v = uPrim;

    return gcd;
}

void generate_key_values(long p, long q, long *n, long *s, long *u)
{
    *n = p * q;
    long t = (p - 1) * (q - 1);
    long i;
    long v = 0;
    i = rand_long(0, t);

    if (*u < 0)
    {
        *u += t;
    }

    while (extended_gcd(i, t, u, &v) != 1)
    {
        i = rand_long(0, t);
    }
    *s = i;
}

long *encrypt(char *chaine, long s, long n)
{
    int taille = strlen(chaine);

    long *encrypted = (long *)malloc(taille * sizeof(long));
    if (encrypted == NULL)
    {
        printf("L'allocation s'est mal passée \n");
        return NULL;
    }

    for (int i = 0; i < taille; ++i)
    {
        encrypted[i] = modpow((long)chaine[i], s, n);
    }

    return encrypted;
}

char *decrypt(long *crypted, int size, long u, long n)
{

    char *message = (char *)malloc((size + 1) * sizeof(char));

    if (message == NULL)
    {
        printf("L'allocation s'est mal passée \n");
        return NULL;
    }

    for (int i = 0; i < size; i++)
    {
        message[i] = (char)(modpow(crypted[i], u, n));
    }
    message[size] = '\0';

    return message;
}

void print_long_vector(long *result, size_t size)
{
    for (size_t i = 0; i < size - 1; ++i)
    {
        printf("%lx; ", result[i]);
    }

    if (size != 0)
    {
        printf("%lx", result[size - 1]);
    }

    printf("]");
}
