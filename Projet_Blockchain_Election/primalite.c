#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int is_prime_naive(long p)
{
    if (p < 2)
    {
        return 0;
    }

    if (p % 2 == 0)
    {
        return 0;
    }

    for (int i = 3; i < p; ++i)
    {
        if (p % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

long modpow_naive(long a, long m, long n)
{
    long res = 1; // Initialisation du resultat
    int i = 0;

    while (i < m)
    {
        res *= a;
        res = res % n;
        i++;
    }
    return res;
}

int modpow(long a, long m, long n)
{
    // Cas de base
    if (m == 0)
    {
        return 1;
    }
    if (a == 0)
    {
        return 0;
    }
    //Si m est pair
    if (m % 2 == 0)
    {
        int temp = modpow(a, m / 2, n);

        return (temp * temp) % n;
    }

    //Si m est impair
    else
    {
        int temp = modpow(a, m - 1, n);
        return (temp * a) % n;
    }
}

int witness(long a, long b, long d, long p)
{
    long x = modpow(a, d, p);
    if (x == 1)
    {
        return 0;
    }
    for (long i = 0; i < b; i++)
    {
        if (x == p - 1)
        {
            return 0;
        }
        x = modpow(x, 2, p);
    }
    return 1;
}
long rand_long(long low, long up)
{
    return rand() % (up - low + 1) + low;
}
int is_prime_miller(long p, int k)
{
    if (p == 2)
    {
        return 1;
    }
    if (!(p & 1) || p <= 1)
    { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1))
    { //tant que d n’est pas impair
        d = d / 2;
        b = b + 1;
    }
    // On genere k valeurs pour a, et on teste si c’est un temoin :
    long a;
    int i;
    for (i = 0; i < k; i++)
    {
        a = rand_long(2, p - 1);
        if (witness(a, b, d, p))
        {
            return 0;
        }
    }
    return 1;
}

long random_prime_number(int low_size, int up_size, int k)
{
    /* retourne un nombre premier de taille comprise entre low_size et up_size */
    long val_low = 1;
    int t;
    // on obtient la valeure minimal
    for (t = 0; t < low_size; t++)
    {
        val_low *= 2;
    }
    // on obtient la valeure maximale
    long val_up = val_low;
    while (t < up_size)
    {
        val_up *= 2;
        t++;
    }
    val_up--;

    // on cherche le nb premier entre les valeurs obtenus
    long p = rand_long(val_low, val_up);
    while (is_prime_miller(p, k) != 1)
    {
        p = rand_long(val_low, val_up);
    }
    return p;
}
