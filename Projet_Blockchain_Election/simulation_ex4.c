#include <stdio.h>
#include <stdlib.h>
#include "manip_struct.h"
#include "time.h"
#include "stockage_dans_les_listes.h"

int recherche_element(int val, int *tab, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (val == tab[i])
        {
            return 1;
        }
    }
    return 0;
}

void generate_random_data(int nv, int nc)
{
    FILE *keys = fopen("keys.txt", "w");
    FILE *candidates = fopen("candidates.txt", "w");
    FILE *declarations = fopen("declarations.txt", "w");
    Key *kptab[nv];
    Key *kstab[nv];
    Key *cptab[nc];
    int nonocc[nc];
    char *mess;
    char *str;
    char *str2;
    int rdm;
    Signature *sgn;
    // Ecriture du fichier keys.txt
    for (int i = 0; i < nv; i++)
    {
        kptab[i] = (Key *)malloc(sizeof(Key));
        kstab[i] = (Key *)malloc(sizeof(Key));
        init_pair_keys(kptab[i], kstab[i], 3, 7);
        str = key_to_str(kstab[i]);
        str2 = key_to_str(kptab[i]);
        fprintf(keys, "%s %s\n", str2, str);
        free(str);
        free(str2);
    }
    // Ecriture du fichier candidates.txt
    for (int i = 0; i < nc; i++)
    {
        srand(time(NULL) * rand());
        rdm = rand() % nv;
        while (recherche_element(rdm, nonocc, i + 1))
        {
            rdm = rand() % nv;
        }
        cptab[i] = kptab[rdm];
        nonocc[i] = rdm;
        str = key_to_str(cptab[i]);
        fprintf(candidates, "%s\n", str);
        free(str);
    }
    // Ecriture du fichier declarations.txt
    for (int i = 0; i < nv; i++)
    {
        srand(time(NULL) * rand());
        int r = rand() % nc;
        char *kpstr = key_to_str(kptab[i]);
        mess = key_to_str(cptab[r]);
        sgn = sign(mess, kstab[i]);
        str = signature_to_str(sgn);
        fprintf(declarations, "%s %s %s\n", kpstr, mess, str);
        free(str);
        free(mess);
        free(kpstr);
        free(sgn);
    }
    // Libération des clés
    for (int i = 0; i < nv; i++)
    {
        free(kstab[i]);
        free(kptab[i]);
    }
    fclose(keys);
    fclose(candidates);
    fclose(declarations);
}
