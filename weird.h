#ifndef WEIRD_H
#define WEIRD_H

#include <stdio.h>
#include <ctype.h>
#include "kmap.h"

// constnts
#define SIZE_MAX_LEXEME 2048

typedef enum errortype {
  ERROR_RUNTIME,
  ERROR_MAX,
} EErrorType;

typedef struct error {
  EErrorType type;
  char *msg; // consists of line
} Error;

typedef struct token {
  ETokenType type;
  char lexeme[SIZE_MAX_LEXEME];
  int line;
} Token;

// weird.h utility function declaratoins
int file_wc(FILE *fileptr);
// weird.h error function declaratoins
void puterr(Error **error, EErrorType type, char *msg);
void throwerr(Error *error);

/* Gets the word count of the file at path by calling stdlib
 * functions */
int file_wc(FILE *fileptr) {
  long tell;

  fseek(fileptr, 0, SEEK_END);
  tell = ftell(fileptr);
  rewind(fileptr);

  return (int)tell;
}

// ok for no error convention
Error *ok = NULL;

/* allocates memory and make an error ptr then puts it into
 * arg error which passed by reference.
 * example: puts runtime error in &error.
 *  puterr(&error, ERROR_RUNTIME, "testing error now.");
 * */
void puterr(Error **error, EErrorType type, char *msg) {
  (*error) = malloc(sizeof(Error));
  (*error)->type = type;
  (*error)->msg = msg;
}

/* prints error to stdout and exits with exit code 65 */
void throwerr(Error *error) {
  printf("error:\n\t%s\n", error->msg);
  free(error);
  exit(65);
}

// function declarations
void put_token(Token tokens[], int *tokensptr, char *srcq, 
    int qptr, int start, ETokenType type);
Error *scan_token(Token tokens[], int *tokensptr, char *srcq,
    int *qptr, int qsize, int start);
Error *scan_tokens(Token tokens[], int *tokensptr, 
    char *srcq, int qsize);
void run(char *srcq, int qsize);
void run_file(char *path);

#include "scan_token.h"

#endif
