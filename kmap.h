#ifndef kmap_h
#define kmap_h

#include <string.h>
#include <stdlib.h>
#include <time.h> // for hash function

#define MAX_KEY 32
#define kmap_SIZE 32

typedef enum ETokenType {
  // Single-char tokens.
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
  // One or two character tokens.
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,
  // Literals.
  TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
  // Keywords.
  TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,
  TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
  TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
  TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
  TOKEN_ERROR, TOKEN_EOF
} ETokenType;

typedef struct keyword{
  char key[MAX_KEY];
  ETokenType type;
  struct keyword *next;
} Keyword;

Keyword *kmap[kmap_SIZE];

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

/* Checksum _hash function */
unsigned int _hash(char *key) {
  int p, a, b, k;
  srand(time(NULL));
  k = key[0];
  p = 37; // prime number > universe
  a = rand() % p;
  b = rand() % p;
  return ((a * k + b) % p) % kmap_SIZE;
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

  for (i = 0; i < kmap_SIZE; i++) {
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
  for (int i = 0; i < kmap_SIZE; i++)
    kmap[i] = NULL;
}

/* put a keyword to kmap, Linear probing. */
void kmapput(Keyword *kmap[], Keyword *keyword) {
  int i, index;
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
  char *kwords[18] = { // declare list of all kmaps
    "and", "class", "else", "false", "for", "fun", "if",
    "nil", "or", "print", "return", "super", "this",
    "true", "var", "while", "error", "eof"};

  _initkmap(kmap);
  printf("from %i to %i\n", TOKEN_AND, TOKEN_EOF);
  for (j = 0, i = TOKEN_AND; i <= TOKEN_EOF; j++, i++) {
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
