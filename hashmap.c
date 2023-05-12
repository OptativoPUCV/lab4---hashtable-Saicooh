#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap
{
    Pair **buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair *createPair(char *key, void *value)
{
  Pair *new = (Pair *) malloc(sizeof(Pair));
  new -> key = key;
  new -> value = value;
  return new;
}

long hash(char *key, long capacity)
{
  unsigned long hash = 0;
  char *ptr;
  
  for (ptr = key; *ptr != '\0'; ptr++) 
    hash += hash * 32 + tolower(*ptr);
  
  return hash % capacity;
}

int is_equal(void *key1, void *key2)
{
  if (key1 == NULL || key2 == NULL) return 0;
  if (strcmp((char*) key1, (char*) key2) == 0) return 1;
  
  return 0;
}

HashMap *createMap(long capacity)
{
  HashMap *mapa = malloc(sizeof(HashMap));
  
  Pair **buckets = calloc(capacity, sizeof(HashMap));

  mapa -> buckets = buckets;
  mapa -> size = 0;
  mapa -> capacity = capacity;
  mapa -> current = -1;
  
  return mapa;
}

void insertMap(HashMap *map, char *key, void *value)
{
  long indice = hash(key, map -> capacity);
  
  while (map -> buckets[indice] != NULL && map -> buckets[indice] -> key != NULL)
  {
    if (is_equal(key, map -> buckets[indice] -> key)) return;
    indice = (indice + 1) % map -> capacity;
  }
 
  map -> buckets[indice] = createPair(key, value);

  map -> size++;
  map -> current = indice;
}

Pair *searchMap(HashMap *map,  char *key)
{
  long indice = hash(key, map -> capacity);

  while (map -> buckets[indice] != NULL && map -> buckets[indice] -> key != NULL)
  {
    if (is_equal(key, map -> buckets[indice] -> key))
    {
      map -> current = indice;
      return map -> buckets[indice];
    }
    indice = (indice + 1) % map -> capacity;
  }
  return NULL;
}

void eraseMap(HashMap *map, char *key)
{    
  long indice = hash(key, map -> capacity);

  while (map -> buckets[indice] != NULL && map -> buckets[indice] -> key != NULL)
  {
    if (is_equal(key, map -> buckets[indice] -> key))
    {
      map -> current = indice;
      map -> size--;
      map -> buckets[indice] -> key = NULL;
      return;
    }
    indice = (indice + 1) % map -> capacity;
  }
}

Pair *firstMap(HashMap *map)
{
  return NULL;
}

Pair *nextMap(HashMap *map)
{

  return NULL;
}

void enlarge(HashMap *map)
{
  long capacidadAnterior = map -> capacity;
  map -> capacity *= 2;
  Pair **bucketsAnteriores = map -> buckets;

  map -> buckets = calloc(map -> capacity, sizeof(Pair));

  long i;

  for (i = 0 ; i < capacidadAnterior ; i++)
  {
    Pair *par = bucketsAnteriores;
    if(par && par -> key) insertMap(map, par -> key, par -> value);
  }

  free(bucketsAnteriores);

  enlarge_called = 1; // no borrar (testing purposes)
}


