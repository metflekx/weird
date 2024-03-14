/*
 *  file: main.c
 *
 *  brief:
 *    -An interpreter for a language called weird compiles the .we
 *    file using a tree-walk. implementation is influenced by the 
 *    book "crafting-interpreters" and consists of functions that
 *    do lexical analysis, parseing, and hopefully IR and 
 *    code gen. phases.
 *    
 *    examples of the valid weird source code exist in this
 *    repository; their name ends with '.we'.
 *
 *  Author: Metflekx
 *
 *  todo:
 *    [ ] Lexical Analysis:
 *      [*] read a file into a queue of bytes: srcqueue
 *        [*] qeueu supports operations: iterate(), lookahead()
 *        , lookaheadahead(), isempty()
 *      [*] implement run just print to stdout
 *      [ ] write decaration for scan_tokens and it helpers and
 *      design the software architecture.
 *      [ ] implement Error struct.
 *      [ ] implement functions.
 *
 *  References:
 * */

#include "weird.h"

/* Gets the word count of the file at path by calling stdlib
 * functions */
int file_wc(FILE *fileptr) {
  long tell;

  fseek(fileptr, 0, SEEK_END);
  tell = ftell(fileptr);
  rewind(fileptr);

  return (int)tell;
}

/* Lexes src into tokens */
Error *scan_tokens(char *srcq, int qsize, int *start, int *end, int *qptr) {
  Error *error;

  // init error
  error = ok;

  // set the error
  error = malloc(sizeof(Error));
  error->type = ERROR_RUNTIME;
  error->msg = "testing error now.";

  return error;
}

Error *run(char *srcq, int qsize) {
  Error *error;
  int start, end, qptr;

  error = ok;

  start = 0;
  end = 0;
  qptr = start;

  if ((error = scan_tokens(srcq, qsize, &start, &end, &qptr)) != ok)
    throwerr(error);

  while (qptr <= qsize)
    fputc((int) srcq[qptr++], stdout);

  return error;
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
  c = fgetc(fileptr), qsize = 0;
  while (c != EOF) {
    srcq[qsize++] = c;
    c = fgetc(fileptr);
  }
  fclose(fileptr); // done with file
  // run
  Error *error;
  if ((error = run(srcq, qsize)) != ok) {
    throwerr(error); 
  }
}

int main(int argc, char **argv) {
  int filewc;

  // get number of char in words to read

  if (argc == 1) {
    // run interpreter
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
