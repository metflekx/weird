/*
* *********************DOCUMENTATION************************
* ----------------------------------------------------------
*  @header: 
*         unsigned int _hash(char *key)
*  @brief: 
*         Calcluates a hash value based on the input key.
*         The resulting hash value is used to determine the index
*         in hash map where the key-value pair will be stored.
*  @param *key:
*         A string input for which the hash value is to be calculated.
*  @ret:
*         A randomly generated key for the hash.
* ----------------------------------------------------------
*  @header:
*         void _print(Keyword *kmap[])
*  @brief:
*         Prints the keyword hash map to stdout. If there is no keyword
*         at an index, "---" is printed.
*  @param *kmap:
*         An array of pointers representing the hash map.
*  @ret:
*         Void.
* ----------------------------------------------------------
*  @header:
*         void _initkmap(Keyword *kmap[])
*  @brief:
*         Initialise the hashmap to point to null, instead of some 
*         garbage values.
*  @param *kmap:
*         An array of pointers representing the hash map.
*  @ret:
*         Void.
* ----------------------------------------------------------      
* @header:
*         void kmapput(Keyword *kmap[], Keyword *keyword)
* @brief:
*         Stores the keyword in the hash map by finding an empty index in
*         the array.
* @param *kmap:
*         An array of pointers representing the hash map.
* @param *keyword:
*         The keyword which needs to be stored in the hash map.
* @ret:
*         Void.
* ----------------------------------------------------------
* @header:
*         Keyword *kmaplookup(Keyword *kmap[], char *query)
* @brief:
*         Looks for the given query (key) in the hash map, and retrieves it.
* @param *kmap:
*         An array of pointers representing the hash map.
* @param *query:
*         A string representing the data to retrieve from the hash map.
* @ret:
*         If the keyword is found in the hash map, a pointer to the keyword is 
*         returned, else NULL is returned.
* ----------------------------------------------------------
* @header:
*         Keyword *kmaprm(Keyword *kmap[], char *query)
* @brief:
*         Looks for the given query (key) in the hash map, and deletes it.
* @param *kmap:
*         An array of pointers representing the hash map.
* @param *query:
*         A string representing the data to delete from the hash map.
* @ret:
*         If the keyword is found in the hash map, a pointer to the keyword is 
*         returned and the keyword is deleted, else NULL is returned.
* ----------------------------------------------------------
* @header:
*         Keyword **kmapinit()
* @brief:
*         Initialises the hash map with predefined legal keywords.
* @ret:
*       A pointer to the initailised hash map.
* ----------------------------------------------------------
*
*/


#ifndef kmap_h
#define kmap_h

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> // for hash function

#define MAX_KEY 32

typedef enum ETokenType {
  TOKEN_IDENT, TOKEN_NUMBER, TOKEN_CONST,
  TOKEN_VAR, TOKEN_PROCEDURE, TOKEN_CALL,
  TOKEN_BEGIN, TOKEN_END, TOKEN_IF, TOKEN_THEN, 
  TOKEN_WHILE, TOKEN_DO, TOKEN_ODD, TOKEN_DOT, 
  TOKEN_EQUAL, TOKEN_COMMA, TOKEN_SEMICOLON, 
  TOKEN_ASSIGN, TOKEN_HASH, TOKEN_LESS, 
  TOKEN_GREATER, TOKEN_PLUS, TOKEN_MINUS, 
  TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_LPAREN, 
  TOKEN_RPAREN} ETokenType;

#define KMAPSIZ (TOKEN_ODD - TOKEN_VAR) + 1

typedef struct keyword{
  char key[MAX_KEY];
  ETokenType type;
  struct keyword *next;
} Keyword;

Keyword *kmap[KMAPSIZ];

// Private functions
unsigned int _hash(char *key);
Keyword *_kword(char *key, ETokenType type);
void _print(Keyword *kmap[]);
void _initkmap(Keyword *kmap[]);
// Public functions
void kmapput(Keyword *kmap[], Keyword *keyword);
Keyword *kmaplookup(Keyword *kmap[], char *query);
Keyword *kmaprm(Keyword *kmap[], char *query);
Keyword **kmapinit();

/* hash function */
unsigned int _hash(char *key) {
  int p, a, b, k;
  srand(time(NULL));
  k = key[0];
  p = 37; // prime number > universe
  a = rand() % p;
  b = rand() % p;
  return ((a * k + b) % p) % KMAPSIZ;
}

Keyword *_kword(char *key, ETokenType type) {
  Keyword *kword = (Keyword*) malloc(sizeof(Keyword));
  kword->type = type;
  strcpy(kword->key, key);
  kword->next = NULL;
  return kword;
}

/* Print kmap to stdout */
void _print(Keyword *kmap[]) {
  int i;
  Keyword *iterator;

  for (i = 0; i < KMAPSIZ; i++) {
    printf("%2.i\t", i);
    if(kmap[i]) {
      iterator = kmap[i];
      while (iterator) {
        printf("%s -> ", iterator->key);
        iterator = iterator->next;
      }
    }
    else {
      printf("---");
    }
    printf("\n");
  }
}

/* Set all entries to point to NULL */
void _initkmap(Keyword *kmap[]) {
  for (int i = 0; i < KMAPSIZ; i++)
    kmap[i] = NULL;
}

/* put a keyword to kmap, Linear probing. */
void kmapput(Keyword *kmap[], Keyword *keyword) {
  int index;
  Keyword *iterator;

  index = _hash(keyword->key);
  if (!kmap[index]) {
    kmap[index] = keyword;
  }
  else {
    iterator = kmap[index];
    while (iterator->next) {
      iterator = iterator->next;
    }
    iterator->next = keyword;
    keyword->next = NULL; // makes sure Kword is not bad
  }
}

Keyword *kmaplookup(Keyword *kmap[], char *query) {
  int index;
  Keyword *iterator;

  index = _hash(query); // get the _hash val.
  
  iterator = kmap[index];
  while (iterator) {
    if (strcmp(iterator->key, query) == 0) {
      return iterator;
    }
    iterator = iterator->next;
  }

  return NULL;
}

Keyword *kmaprm(Keyword *kmap[], char *query) {
  int index;
  Keyword *iterator, *deleted;

  index = _hash(query); // get the _hash val.
  if (!kmap[index]) 
    return NULL;

  if (strcmp(kmap[index]->key, query) == 0) {
    // Found at root
    deleted = kmap[index]; 
    kmap[index] = kmap[index]->next;
    return deleted;
  }
  
  iterator = kmap[index]; // look and delete if found
  while (iterator->next) {
    if (strcmp(iterator->next->key, query) == 0) {
      // Found in chain
      deleted = iterator->next;
      iterator->next = iterator->next->next;
      return deleted;
    }
    iterator = iterator->next;
  }

  return NULL; // Not Found
}

/* initialize the kmap of weird legal keywords */
Keyword **kmapinit() {
  int i, j;
  char *kwords[11] = { // declare list of all kmaps
    "var", "procedure", "call", "begin", "end", 
    "if", "then", "while", "do", "odd"};

  _initkmap(kmap);
  for (j = 0, i = TOKEN_VAR; i <= TOKEN_ODD; j++, i++) {
    kmapput(kmap, _kword(kwords[j], i));
  }
  return kmap;
}

// uncomment for debug
// int main(void) {
//   _print(
//     kmapinit());
// 
//   return 0;
// }

#endif
