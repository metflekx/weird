/*
* *********************DOCUMENTATION************************
* ----------------------------------------------------------
* @header:
*         int file_wc(FILE *fileptr)
* @brief:
*         Determines the size of the file in bytes.
* @param fileptr:
*         Input file to measure it's size.
* @ret:
*         Size of the file in bytes.
* ----------------------------------------------------------
* @header:
*         void puterrmsg(char src[], char *msg, char token, int line)
* @brief:
*         Generates error (readable) message and puts into src array. 
* @param src:
*         The location for the error message message to be stored.
* @param msg:
*         The error message.
* @param token:
*         The specific token that caused the error.
* @param line:
*         The line number where the error is.
* @ret:
*         Void.
* ----------------------------------------------------------
* @header:
*         void puterr(Error **error, EErrorType type, char msg[])
* @brief:
*         Allocates memory and make an error pointer then puts it 
*         into argument error which passed by reference.
* @param **error:
*         Address of the allocated memory to the error.
* @param type:
*         The type of the error.
* @param msg:
*         The error message.
* @ret:
*         Void.
* ----------------------------------------------------------
* @header:
*         void throwerr(Error *error)
* @brief:
*         Prints error to stdout and exits with exit code 65.
* @param *error:
*         A pointer to an Error struct containing the error message.
* @ret:
*         Void.
* ----------------------------------------------------------
*
*/


#ifndef WEIRD_H
#define WEIRD_H

#include <stdio.h>
#include <ctype.h>
#include "kmap.h"

// constnts
#define SIZE_MAX_LEXEME 2048
#define REPL_MAX_LINE 512
#define REPL_ENDL "\n\r"
#define ERROR_MAX_MSG 512

typedef enum errortype {
  ERROR_RUNTIME,
  ERROR_MAX,
} EErrorType;

typedef struct error {
  EErrorType type;
  char msg[ERROR_MAX_MSG];
} Error;

typedef struct token {
  ETokenType type;
  char lexeme[SIZE_MAX_LEXEME];
  int line;
} Token;

// weird.h utility function declaratoins
int file_wc(FILE *fileptr);
// weird.h error function declaratoins
void puterrmsg(char src[], char *msg, char token, int line);
void puterr(Error **error, EErrorType type, char msg[]);
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

/* Generates error readable error msg puts into src.
 * Just a few boring stdlib calls. */
void puterrmsg(
    char src[], char *msg, char token, int line) {
  char buff[10];
  int i = 0;
  strcpy(src, msg); //"Unexpected Token: \'");
  strcat(src, ": \'");
  sprintf(buff, "%d", line); // itoa
  while (src[i++] != '\0')
    ;
  src[i - 1] = token;
  src[i] = '\0';
  strcat(src, "\' At Line: ");
  strcat(src, buff);
}

/* allocates memory and make an error ptr then puts it into
 * arg error which passed by reference.
 * example: puts runtime error in &error.
 *  puterr(&error, ERROR_RUNTIME, "testing error now.");
 * */
void puterr(Error **error, EErrorType type, char msg[]) {
  (*error) = malloc(sizeof(Error));
  (*error)->type = type;
  strcpy((*error)->msg, msg);
}

/* prints error to stdout and exits with exit code 65 */
void throwerr(Error *error) {
  printf("error:\n\t%s\n", error->msg);
  free(error);
  exit(65);
}

// function declarations
void put_token(Token tokens[], int *tokensptr, char *srcq, 
    int qptr, int start, ETokenType type, int line);
Error *scan_token(Token tokens[], int *tokensptr,
    char *srcq, int *qptr, int qsize, int start, int *line);
Error *scan_tokens(Token tokens[], int *tokensptr, 
    char *srcq, int qsize);
void run(char *srcq, int qsize);
void run_file(char *path);
void repl();

#include "scan_token.h"

#endif
