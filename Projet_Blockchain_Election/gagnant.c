#include "gagnant.h"
#include "assert.h"

void delete_fake_protected(CellProtected **LCK)
{
    CellProtected *tmp;

    while ((*LCK != NULL) && (verify((*LCK)->data) == 0))
    {
        tmp = *LCK;
        *LCK = (*LCK)->next;
        delete_cell_protected(tmp);
    }

    CellProtected *prec = *LCK;

    if (prec == NULL)
    {
        return;
    }

    CellProtected *c = (*LCK)->next;

    while (c != NULL)
    {
        if (verify(c->data) == 0)
        {
            prec->next = c->next;
            tmp = c;
            c = c->next;
            delete_cell_protected(tmp);
        }

        else
        {
            prec = c;
            c = c->next;
        }
    }
}

HashCell *create_hashcell(Key *key)
{
    HashCell *hashCell = (HashCell *)malloc(sizeof(HashCell));
    if (hashCell == NULL)
    {
        printf("Erreur d'allocation dans la fonction create_hashcell \n");
        return NULL;
    }
    hashCell->key = key;
    hashCell->val = 0;

    return hashCell;
}

int hash_function(Key *key, int size)
{
    double A = (sqrt(5) - 1) / 2;
    long i = key->n;
    int position = (int)(floor(size * (i * A - floor(i * A))));

    return position;
}
int find_position(HashTable *t, Key *key)
{ 
    int n = t->size;
    int h = hash_function(key, n);
    int i = 0;
    int indice = (h + i) % n;

    while ((t->tab)[indice] != NULL)
    {
        if (((((t->tab)[indice])->key)->val == key->val) && ((((t->tab)[indice])->key)->n == key->n))
        {
            return indice;
        }
        i += 1;
        indice = (h + i) % n;
    }

    return indice;
}

HashTable *create_hashtable(CellKey *keys, int size)
{

    //Creation de la table de hachage
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->tab = (HashCell **)malloc(size * sizeof(HashCell *));
    for (int i = 0; i < size; i++)
    {
        table->tab[i] = NULL;
    }
    table->size = size;

    //Inserons les clées dans la liste_keys
    int pos;
    while (keys)
    {
        pos = find_position(table, keys->data);
        HashCell *candidate = create_hashcell(keys->data);
        table->tab[pos] = candidate;
        keys = keys->next;
    }
    return table;
}

void afficher_tableH(HashTable *t)
{
    //fonction de test
    printf("La taille de la table est %d\n", t->size);
    for (int i = 0; i < t->size; i++)
    {
        if (t->tab[i] == NULL)
        {
            printf("tab[%d] est null\n", i);
        }
        else
        {
            char *key = key_to_str(t->tab[i]->key);
            printf("tab[%d] = key : %s, val : %d\n", i, key, t->tab[i]->val);
            free(key);
        }
    }
}

void delete_hash_cellule(HashCell *hashcell)
{

    free(hashcell->key);
    free(hashcell);
}

void delete_hashtable(HashTable *t)
{
    for (int i = 0; i < t->size; i++)
    {
        free(t->tab[i]);
    }
    free(t->tab);
    free(t);
}

Key *compute_winner(CellProtected *decl, CellKey *candidates, CellKey *voters, int sizeC, int sizeV)
{

    HashTable *hc = create_hashtable(candidates, sizeC);
    HashTable *hv = create_hashtable(voters, sizeV);
    delete_fake_protected(&decl); //Suppression des fraudes

    //Parcours des declarations
    int posV, posC;
    while (decl)
    {
        posV = find_position(hv, decl->data->pKey);
        if (hv->tab[posV] != NULL)
        {
            if (hv->tab[posV]->val == 0)
            { //S'il n'as jamais voter
                Key *keyC = str_to_key(decl->data->message);
                posC = find_position(hc, keyC);
                if (hc->tab[posC] != NULL)
                {
                    hv->tab[posV]->val = 1;
                    hc->tab[posC]->val = hc->tab[posC]->val + 1;
                }
            }
        }
        decl = decl->next;
    }

    HashCell *gagnant = NULL;
    int val_gagnant = -1;
    for (int i = 0; i < hc->size; i++)
    {
        if (hc->tab[i] != NULL)
        {
            if (val_gagnant < hc->tab[i]->val)
            {
                gagnant = hc->tab[i];
                val_gagnant = hc->tab[i]->val;
            }
        }
    }
    if (gagnant == NULL)
    {
        fprintf(stderr, "Erreur : Compute_winner : gagnant est NULL");
    }

    Key *g = gagnant->key;
    delete_hashtable(hc);
    delete_hashtable(hv);
    return g;
}