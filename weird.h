#ifndef WEIRD_H
#define WEIRD_H

#include <stdio.h>
#include <stdlib.h>

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
  char *lexeme;
  int line;
} Token;

// ok for no error convention
Error *ok = NULL;

// weird.h function declaratoins
void throwerr(Error *error);

/* prints error to stdout and exits with exit code 69 */
void throwerr(Error *error) {
  printf("error:\n\t%s\n", error->msg);
  free(error);
  exit(65);
}

// weird.c function declarations
Error *scan_tokens(char *srcq, int qsize, int *start, int *end, int *qptr);
int file_wc(FILE *fileptr);
void run_file(char *path);

#endif
