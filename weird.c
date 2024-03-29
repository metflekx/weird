/*
 *  file: weird.c
 *
 *  brief:
 *    -An interpreter for a language called weird compiles the .we
 *    file using a tree-walk. implementation is influenced by the 
 *    book "crafting-interpreters" and consists of functions that
 *    do lexical analysis, parseing, and hopefully IR and 
 *    code gen. phases.
 *
 *    this repo consists of weird.c, weird.h and scan_token.h .
 *    
 *    examples of the valid weird source code exist in this
 *    repository; their name ends with '.we'.
 *
 *  Author: Metflekx
 *
 *  todo:
 *    [*] Lexical Analysis:
 *      [*] Successfully tokenize a file consist of "!*+-/=<> <= == // ops".
 *      [*] Successfully tokenize string literals.
 *      [*] Successfully tokenize number literals.
 *      [*] Successfully tokenize res. keywords and identifiers.
 *      [*] for each token, keep track of line.
 *      [*] error should stdout token's line and lexeme.
 *      [*] Implement Repl.
 *
 *  References:
 *    Notes:
 *    how to throw error correctly from whitin scan_token():
 *    where c is current char that is being read.
 *      char msg[ERROR_MAX_MSG];
 *      puterrmsg(msg, "Unexpected Token", c, *line);
 *      puterr(&error, ERROR_RUNTIME, msg);
 * */

#include "weird.h"

/* Creates a Token { struct token } put the right values in it 
 * and the newly created token into tokens[] */
void put_token(Token tokens[], int *tokensptr, char *srcq,
    int qptr, int start, ETokenType type, int line) {
  int length;
  // obtain length of lexeme
  length = qptr - start;

  // declare token
  Token token;
  // put type
  token.type = type;
  // put lexeme
  strncpy(token.lexeme, srcq + start, length);
  token.lexeme[length] = '\0';
  // put line
  token.line = line;

  // append token to tokens[]
  tokens[++(*tokensptr)] = token;
}

/* Lexes src into tokens */
Error *scan_tokens(Token tokens[], int *tokensptr, char *srcq, 
    int qsize) {
  Error *error;
  int start, qptr, line;

  start = 0;
  qptr = 0;
  line = 1;

  while (qptr <= qsize) { // while not queue end
    start = qptr;
    if ((error = scan_token(
            tokens, tokensptr, srcq, &qptr, qsize, start, &line)) != ok) {
      throwerr(error);
    }
  }

  // set the error
  return error;
}

/* run the code in srcq source queue */
void run(char *srcq, int qsize) {
  int tokensptr;
  Error *error;
  Token tokens[qsize]; // in worst case, each char is a token.
  
  // Sacn tokens while handling error.
  error = ok, tokensptr = -1; // init error , init tokensptr.
  if ((error = scan_tokens(
          tokens, &tokensptr, srcq, qsize)) != ok)
    throwerr(error);
}

/* Compile and runs the code in a file */
void run_file(char *path) {
  int filesize, qsize, c;
  FILE *fileptr;

  // get the filesize
  fileptr = fopen(path, "rb");
  filesize = file_wc(fileptr);

  // declare q
  char srcq[filesize + 1];
  srcq[filesize] = '\0'; // null terminator at end

  // read file's content into srcq
  c = fgetc(fileptr), qsize = -1;
  while (c != EOF) {
    srcq[++qsize] = c;
    c = fgetc(fileptr);
  }
  fclose(fileptr); // done with file
  // run
  run(srcq, qsize);
}

/* Runs the interactive weird. Reads and runs one-line at a 
 * time from stdin */
void repl() {
  printf("Interactive weird.\nenter @ to quit.\n");
  int size = 0;
  char c, srcq[REPL_MAX_LINE];
  while (1) {
    printf("~> ");
    while ((c = getchar()) != '\n') {
      if (c == '@') {
        return;
      }
      else {
        srcq[size++] = c;
      }
    }
    strcat(srcq, REPL_ENDL);
    run(srcq, size);
    bzero(srcq, size);
    size = 0;
  }
}

int main(int argc, char **argv) {
  kmapinit(); // Initialize kmap keywords lookup table
  
  if (argc == 1) {
    // run interactive mode
    repl();
    return 0;
  }
  else if (argc == 2) {
    // run file
    run_file(argv[1]);
  }
  else {
    printf("Usage:\n\tweirdc <source file>.\n");
  }

  return 0;
}
