/*
 *  -A interpreter for a language called weird compiles the .we
 *  file using a tree-walk. implementation is influenced by the 
 *  book "crafting-interpreters" and consists of functions that
 *  do lexical analysis, parseing, and hopefully IR and 
 *  code gen. phases.
 *  
 *  examples of the valid weird source code exist in this
 *  repository; their name ends with '.we'.
 *
 *  Author: Metflekx
 *
 *  todo:
 *    [ ] Lexical Analysis:
 *      [*] read a file into a queue of bytes: srcque
 *        [ ] qeueu supports operations: iterate(), lookahead()
 *        , lookaheadahead(), isempty()
 *
 *      [ ] implement run just print to stdout
 *      [ ] write decaration for scan_tokens and it helpers and
 *      design the software architecture.
 *      [ ] implement functions.
 *
 *  References:
 * */

#include <stdio.h>

/* Gets the word count of the file at path by calling stdlib
 * functions */
int file_wc(FILE *fileptr) {
  long tell;

  fseek(fileptr, 0, SEEK_END);
  tell = ftell(fileptr);
  rewind(fileptr);

  return (int)tell;
}

/* Compile and runs the code in a file */
void run_file(char *path) {
  int filesize, i, c;
  char srcqueptr, start, end;
  FILE *fileptr;

  // get the filesize
  fileptr = fopen(path, "rb");
  filesize = file_wc(fileptr);

  // declare queue
  char srcque[filesize + 1];

  // read file's content into srcque
  c = fgetc(fileptr), i = 0;
  while (c != EOF) {
    srcque[i++] = c;
    c = fgetc(fileptr);
  }
  fclose(fileptr); // done with file
  
  // initialize queue (add properties)
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
