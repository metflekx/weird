#ifndef WEIRD_H
#define WEIRD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
  TOKEN_ERROR,
  TOKEN_EOF
} ETokenType;

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
void put_token(Token tokens[], int *tokensptr, char *srcq, int qptr, int start, ETokenType type);
Error *scan_token(Token tokens[], int *tokensptr, char *srcq, int *qptr, int qsize, int start);
Error *scan_tokens(Token tokens[], int *tokensptr, char *srcq, int qsize);
void run(char *srcq, int qsize);
void run_file(char *path);

#include "scan_token.h"

#endif
