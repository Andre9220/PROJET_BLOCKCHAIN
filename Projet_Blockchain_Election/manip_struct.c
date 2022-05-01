#include "manip_struct.h"

void init_key(Key *key, long val, long n)
{
  key->val = val;
  key->n = n;
}

void init_pair_keys(Key *pKey, Key *sKey, int low_size, int up_size)
{

  long public_valeur, private_valeur, t;
  long p = random_prime_number(low_size, up_size, 5000);
  long q = random_prime_number(low_size, up_size, 5000);

  while (q == p)
  {
    q = random_prime_number(low_size, up_size, 5000);
  }
  long n = p * q;

  generate_key_values(p, q, &n, &public_valeur, &private_valeur);

  if (private_valeur < 0)
  {
    t = (p - 1) * (q - 1);
    private_valeur = private_valeur + t;
  }
  init_key(pKey, public_valeur, n);
  init_key(sKey, private_valeur, n);
}

char *key_to_str(Key *key)
{
  char *s = (char *)malloc(256 * sizeof(char));
  if (s == NULL)
  {
    printf("Erreur d allocation key_to_str \n");
  }
  sprintf(s, "(%lx,%lx)", key->val, key->n);
  return s;
}

Key *str_to_key(char *str)
{
  Key *key = (Key *)malloc(sizeof(Key));

  if (key == NULL)
  {
    printf("Erreur lors de l'allocation dans la fonction str_to_key ");
    exit(EXIT_FAILURE);
  }

  if (sscanf(str, "(%lx,%lx)", &(key->val), &(key->n)) != 2)
  {
    printf("Erreur dans la focntion str_to_key");
    exit(EXIT_FAILURE);
  }

  return key;
}

Signature *init_signature(long *content, int size)
{

  Signature *signature = (Signature *)malloc(sizeof(Signature));

  if (signature == NULL)
  {
    printf("Erreur lors de l'allocation de la signature dans la fonction init_signature");
    exit(EXIT_FAILURE);
  }

  signature->tableau = content;
  signature->taille = size;

  return signature;
}

Signature *sign(char *mess, Key *sKey)
{
  long *signature = encrypt(mess, sKey->val, sKey->n);
  Signature *res = init_signature(signature, strlen(mess));
  return res;
}

char *signature_to_str(Signature *sgn)
{
  char *result = malloc(10 * sgn->taille * sizeof(char));
  result[0] = '#';
  int pos = 1;
  char buffer[156];
  for (int i = 0; i < sgn->taille; i++)
  {
    sprintf(buffer, "%lx", sgn->tableau[i]);
    for (int j = 0; j < strlen(buffer); j++)
    {
      result[pos] = buffer[j];
      pos = pos + 1;
    }
    result[pos] = '#';
    pos = pos + 1;
  }
  result[pos] = '\0';
  result = realloc(result, (pos + 1) * sizeof(char));
  return result;
}

Signature *str_to_signature(char *str)
{
  size_t len = strlen(str);
  long *content = (long *)malloc(sizeof(long) * len);
  int num = 0;
  char buffer[256];
  int pos = 0;
  for (size_t i = 0; i < len; i++)
  {
    if (str[i] != '#')
    {
      buffer[pos] = str[i];
      pos = pos + 1;
    }
    else
    {
      if (pos != 0)
      {
        buffer[pos] = '\0';
        sscanf(buffer, "%lx", &(content[num]));
        num = num + 1;
        pos = 0;
      }
    }
  }
  content = realloc(content, num * sizeof(long));
  return init_signature(content, num);
}

Protected *init_protected(Key *pKey, char *mess, Signature *sgn)
{
  Protected *protected = (Protected *)malloc(sizeof(Protected));

  if (protected == NULL)
  {
    printf(" Erreur lors l'allocation de la fonction init_protected \n ");
    return NULL;
  }

protected
  ->signature = sgn;
protected
  ->pKey = pKey;
protected
  ->message = strdup(mess);

  return protected;
}

int verify(Protected *pr)
{

  char *messagedecrypte = decrypt(pr->signature->tableau, pr->signature->taille, pr->pKey->val, pr->pKey->n);

  if (strcmp(messagedecrypte, pr->message) == 0)
  {
    return 1;
  }

  free(messagedecrypte);
  return 0;
}

char *protected_to_str(Protected *pr)
{
  char *str = (char *)malloc(256 * sizeof(char));

  if (str == NULL)
  {
    printf(" Erreur lors l'allocation de la fonction protected_to_str \n ");
    exit(EXIT_FAILURE);
  }

  int cpt = 0;
  char *kstr = key_to_str(pr->pKey);
  char *sstr = signature_to_str(pr->signature);

  // La clé est d'abord ajoutée dans la chaîne de caractères
  for (int i = 0; i < strlen(kstr); i++)
  {
    str[cpt++] = kstr[i];
  }

  str[cpt++] = ' ';

  // Le message est ensuite ajoutée dans la chaîne de caractères
  for (int i = 0; i < strlen(pr->message); i++)
  {
    str[cpt++] = (pr->message)[i];
  }

  str[cpt++] = ' ';

  // La signature  est ensuite ajoutée dans la chaîne de caractères
  for (int i = 0; i < strlen(sstr); i++)
  {
    str[cpt++] = sstr[i];
  }

  str[cpt] = '\0';

  free(kstr);
  free(sstr);

  return str;
}

Protected *str_to_protected(char *str)
{
  int i = 0, cpt = 0;

  char *mess = (char *)malloc(256 * sizeof(char));
  char *kstr = (char *)malloc(256 * sizeof(char));
  char *sstr = (char *)malloc(256 * sizeof(char));

  if (mess == NULL || kstr == NULL || sstr == NULL)
  {
    printf(" Erreur lors l'allocation de la fonction str_to_protected \n ");
    exit(EXIT_FAILURE);
  }

  // La clé est d'abord lu puis stocké
  while ((str[cpt] != ' ') && (str[cpt] != '\0'))
  {
    kstr[i++] = str[cpt++];
  }

  kstr[i] = '\0';
  cpt++;
  i = 0;

  /// Le message est  lu puis stocké
  while ((str[cpt] != ' ') && (str[cpt] != '\0'))
  {
    mess[i++] = str[cpt++];
  }

  mess[i] = '\0';
  cpt++;
  i = 0;

  // La  signature est  lu puis stocké
  while ((str[cpt] != ' ') && (str[cpt] != '\0'))
  {
    sstr[i++] = str[cpt++];
  }

  sstr[i] = '\0';

  Key *pKey = str_to_key(kstr);
  Signature *sgn = str_to_signature(sstr);

  free(kstr);
  free(sstr);

  Protected *pr = init_protected(pKey, mess, sgn);
  free(mess);

  return pr;
}

void free_signature(Signature *sgn)
{
  free(sgn->tableau);
  free(sgn);
}
