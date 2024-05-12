/*
 * file: gen.h
 *  brief:
 *    - Code generator module.
 *      Functions within this file
 *      get called by the parser to
 *      generate the corresponding C
 *      code as an output.
 *  Author: Metflex
 */

#include "weird.h"
#include <stdarg.h>

/* code gen. function declarations */
void aout(char *fmt, ...);
void gencall(Token tokens[], int index);
void genconst(Token tokens[], int index);
void gennewl(Token tokens[], int index);
void genepilogue(int ismain);
void genproc(
    Token tokens[], int index, int ismain);
void gensemicol();
void gensymbol(Token tokens[], int index);
void genvar(Token tokens[], int index);
void genend();

/* For now, just writes all
 * generated C code to stdout */
void aout(char *fmt, ...) {
  va_list ap;

  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  va_end(ap);
}

void gencall(Token tokens[], int index) {
  aout("%s()", tokens[index]);
}

/* Generates and writes equivalent C code
 * for const */
void genconst(Token tokens[], int index) {
  aout("const long %s = ", tokens[index].lexeme);
}

void gennewl(Token tokens[], int index) {
  aout("\n");
}

void genepilogue(int ismain) {
  if (ismain)
    aout("return 0;\n}\n");
  else
    aout("return;\n}\n");
}

/* generates and writes equivalent c code
 * for a PL/0 procedure. checks and 
 * generates code according to wether 
 * the main function should be written or
 * another procedure . */
void genproc(
    Token tokens[], int index, int ismain) {
  if (ismain) {
    aout(
        "int main(int argc, char *argv[]) {\n");
  }
  else {
    aout(
        "void %s(void) {\n", tokens[index].lexeme);
  }
}

void gensemicol() {
  aout(";\n");
}

void gensymbol(Token tokens[], int index) {
  switch (tokens[index].type) {
	  case TOKEN_IDENT:
	  case TOKEN_NUMBER:
	  	aout("%s", tokens[index].lexeme);
	  	break;
	  case TOKEN_BEGIN:
	  	aout("{\n");
	  	break;
	  case TOKEN_END:
	  	aout(";\n}\n");
	  	break;
	  case TOKEN_IF:
	  	aout("if(");
	  	break;
	  case TOKEN_THEN:
	  case TOKEN_DO:
	  	aout(")");
	  	break;
	  case TOKEN_ODD:
	  	aout("(");
	  	break;
	  case TOKEN_WHILE:
	  	aout("while(");
	  	break;
	  case TOKEN_EQUAL:
	  	aout("==");
	  	break;
	  case TOKEN_COMMA:
	  	aout(",");
	  	break;
	  case TOKEN_ASSIGN:
	  	aout("=");
	  	break;
	  case TOKEN_HASH:
	  	aout("!=");
	  	break;
	  case TOKEN_LESS:
	  	aout("<");
	  	break;
	  case TOKEN_GREATER:
	  	aout(">");
	  	break;
	  case TOKEN_PLUS:
	  	aout("+");
	  	break;
	  case TOKEN_MINUS:
	  	aout("-");
	  	break;
	  case TOKEN_MULTIPLY:
	  	aout("*");
	  	break;
	  case TOKEN_DIVIDE:
	  	aout("/");
	  	break;
	  case TOKEN_LPAREN:
	  	aout("(");
	  	break;
	  case TOKEN_RPAREN:
	  	aout(")");
      break;
    default:
      break;
	}
}

/* Generates and writes equivalent c code
 * for var */
void genvar(Token tokens[], int index) {
  aout("long %s;\n", tokens[index].lexeme);
}

/* Informs user that parser is 
 * finished */
void genend() {
  aout("\nFINISHED.\n");
}

