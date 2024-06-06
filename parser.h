/* file: parser.h
 * functions for parsing the tokenised input 
 * of program according to PL/0 grammer. 
 * unlike implemetations in lexer module, this
 * parser does not return error back up the
 * funcitnos stack. instead the match function
 * will exit the program if any error detected.
 *
 * Author: Metflex.
 *
 *
 * TODO:
 *  [ ] fix the main and procedures hirearchy, 
 *      allow no nested procedures.
 * */

#include "gen.h"

// array ptr
static int parserptr;

/* parser function declarations */
void match(Token tok, ETokenType expect);
void condition(Token tokens[], int size);
void factor(Token tokens[], int size);
void term(Token tokens[], int size);
void expression(Token tokens[], int size);
void statement(Token tokens[], int size);
void block(Token tokens[], int size);
void parse(Token tokens[], int size);

/* if tok does not match the expected
 * tokentype, throws an error and
 * exits the program */
void match(
    Token tok, ETokenType expect) {
  if (tok.type != expect) {
    Error *error;
    // make, throw error and exit
    char msg[ERROR_MAX_MSG];
    puterrmsg(
        msg, "syntax error", *tok.lexeme, tok.line);
    puterr(&error, ERROR_RUNTIME, msg);
    throwerr(error);
  }
}

/* parses a condition. if cond. 
 * is not valid call to match will
 * error */
void condition(Token tokens[], int size) {
  expression(tokens, size);

  gensymbol(tokens, parserptr);

  char msg[ERROR_MAX_MSG]; // error msg buff
  Error *error = ok; // init error
  
  switch (tokens[parserptr].type) {
    case TOKEN_ODD:
      match(tokens[parserptr++], TOKEN_ODD);
      expression(tokens, size);
      break;
    case TOKEN_EQUAL:
      match(tokens[parserptr++], TOKEN_EQUAL);
      break;
    case TOKEN_HASH:
      match(tokens[parserptr++], TOKEN_HASH);
      break;
    case TOKEN_LESS:
      match(tokens[parserptr++], TOKEN_LESS);
      break;
    case TOKEN_GREATER:
      match(tokens[parserptr++], TOKEN_GREATER);
      break;
    default: // error and exit
      puterrmsg(
          msg, "expected condition got: ", 
          tokens[parserptr].lexeme[0], 
          tokens[parserptr].line);
      puterr(&error, ERROR_RUNTIME, msg);
      throwerr(error);
      break;
  }

  expression(tokens, size);
}

/* parses a factor */
void factor(Token tokens[], int size) {
  char msg[ERROR_MAX_MSG]; // init error msg buf
  Error *error = ok; // init error

  switch (tokens[parserptr].type) {
    case (TOKEN_IDENT):
    case (TOKEN_NUMBER):
      gensymbol(tokens, parserptr);
      parserptr++;
      break;
    case (TOKEN_LPAREN):

      // gen and match paren.
      gensymbol(tokens, parserptr);
      match(tokens[parserptr++], TOKEN_LPAREN);

      expression(tokens, size);

      gensymbol(tokens, parserptr);
      match(tokens[parserptr++], TOKEN_RPAREN);

      break;

    default: // error and exit
      puterrmsg(
          msg, "expected factor got: ", 
          tokens[parserptr].lexeme[0], 
          tokens[parserptr].line);
      puterr(&error, ERROR_RUNTIME, msg);
      throwerr(error);
      break;
  }
}

/* parses a term */
void term(Token tokens[], int size) {
  factor(tokens, size);
  while (
      tokens[parserptr].type == TOKEN_MULTIPLY\
      || tokens[parserptr].type == TOKEN_DIVIDE) {
    gensymbol(tokens, parserptr);
    parserptr++;
    factor(tokens, size);
  }
}

/* parses a expression. */
void expression(Token tokens[], int size) {
  if (
      tokens[parserptr].type == TOKEN_PLUS\
      || tokens[parserptr].type == TOKEN_MINUS) {
    gensymbol(tokens, parserptr);
    parserptr++;
  }

  term(tokens, size);

  while (
      tokens[parserptr].type == TOKEN_PLUS\
      || tokens[parserptr].type == TOKEN_MINUS) {
    gensymbol(tokens, parserptr);
    parserptr++;
    term(tokens, size);
  }
}

/* parses a statement. If statement
 * is not valid according to PL/0 
 * grammer calls to match will error. */
void statement(
    Token tokens[], int size) {
  switch (tokens[parserptr].type) {
    case TOKEN_IDENT:
      gensymbol(tokens, parserptr); // wr: ident
      parserptr++; // skip ident
      
      gensymbol(tokens, parserptr); // wr: = 
      match(tokens[parserptr++], TOKEN_ASSIGN);
      
      expression(tokens, size);
      break;

    case TOKEN_CALL:
      parserptr++;
      gencall(tokens, parserptr);
      match(tokens[parserptr++], TOKEN_IDENT);
      break;

    case TOKEN_BEGIN: // multiple begins can be nested
      do { // parse st. , if ';' again parse st.
        if (tokens[parserptr].type == TOKEN_SEMICOLON) {
          gensemicol(); 
        }
        else {
          gensymbol(tokens, parserptr); // wr: { 
        }

        parserptr++; // skip 'begin' or ';'
        statement(tokens, size); // recurse
      } while (tokens[parserptr].type == TOKEN_SEMICOLON);
      
      gensymbol(tokens, parserptr); // wr: }
      match(tokens[parserptr++], TOKEN_END);

      break;

    case TOKEN_IF:
      gensymbol(tokens, parserptr); // wr: if (
      parserptr++; // skip 'if'
      
      condition(tokens, size);

      gensymbol(tokens, parserptr); // wr: { 
      match(tokens[parserptr++], TOKEN_THEN);

      statement(tokens, size);
      break;

    case TOKEN_WHILE:
      gensymbol(tokens, parserptr); // wr: while (
      parserptr++; // skip 'while'
      
      condition(tokens, size);
      
      gensymbol(tokens, parserptr); // wr: { 
      match(tokens[parserptr++], TOKEN_DO);

      statement(tokens, size);
      break;

    default:
      break;
  }
}

/* recursively interprets a block, 
 * if validatoins according to 
 * PL/0 grammer fails. then the
 * call to match function exits 
 * throws an error and the program. */
void block(
    Token tokens[], int size) {
  // boolean to control writing all
  // procedures before int main()
  int ismain = 0;

  if (tokens[parserptr].type == TOKEN_CONST) {
    do {
      parserptr++; // skip const | comma
      
      // wr: const long = 
      genconst(tokens, parserptr); 
      match(tokens[parserptr++], TOKEN_IDENT);

      // symbol = is already written.
      match(tokens[parserptr++], TOKEN_EQUAL);

      gensymbol(tokens, parserptr);
      gensemicol(); // wr: num = 
      
      match(tokens[parserptr++], TOKEN_NUMBER);

    } while (
        tokens[parserptr++].type == TOKEN_COMMA);
  }
  
  if (tokens[parserptr].type == TOKEN_VAR) {
    do {
      parserptr++; // skip var | comma
      
      genvar(tokens, parserptr); // wr var ident;
      match(tokens[parserptr++], TOKEN_IDENT);

    } while (
        tokens[parserptr].type == TOKEN_COMMA);

    match(tokens[parserptr++], TOKEN_SEMICOLON);
  }

  while (tokens[parserptr].type == TOKEN_PROCEDURE) {
    parserptr++; // skip procedure
    
    genproc(tokens, parserptr, ismain);
    match(tokens[parserptr++], TOKEN_IDENT);
    match(tokens[parserptr++], TOKEN_SEMICOLON);
    block(tokens, size);
    match(tokens[parserptr++], TOKEN_SEMICOLON);
    ismain = 1;
  }
  if (ismain) {
    genproc(tokens, parserptr, ismain);
  }
  statement(tokens, size);

  genepilogue(ismain);
}

/* validates each token */
void parse(Token tokens[], int size) {
  
  // check for program = block "." .
  // parser ptr is initially at 0
  parserptr = 0; // set the global iterator to 0
  block(tokens, size); // block
  match(tokens[size], TOKEN_DOT); // "."
  
  genend();
}
