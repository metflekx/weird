#ifndef SCAN_TOKEN_H
#define SCAN_TOKEN_H

/* reads one token, checks if is correct token reads it 
 * into tokens else populates the error with the right msg */
Error *scan_token(Token tokens[], int *tokensptr, char *srcq,
    int *qptr, int qsize, int start) {
  char c;
  Error *error;

  error = ok;
  c = srcq[(*qptr)++];
  switch (c) {
    case '(':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_LEFT_PAREN);
      break;
    case ')':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_RIGHT_PAREN);
      break;
    case '{':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_RIGHT_PAREN);
      break;
    case '}':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_RIGHT_PAREN);
      break;
    case ',':
      put_token(tokens, tokensptr, srcq, *qptr, start,
          TOKEN_COMMA);
      break;
    case '.':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_DOT);
      break;
    case '-':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_MINUS);
      break;
    case '+':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_PLUS);
      break;
    case ';':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_SEMICOLON);
      break;
    case '*':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_STAR);
      break;
    case '!': // check if the lexeme is continued '!=' or just '!'.
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_BANG_EQUAL):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_BANG);
      break;
    case '=':
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_EQUAL_EQUAL):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_EQUAL);
      break;
    case '<':
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_LESS_EQUAL):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_LESS);
      break;
    case '>':
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_GREATER_EQUAL):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_GREATER);
      break;
    case '/': // Handling comments and Division
      if (*qptr <= qsize && srcq[(*qptr)] == '/') {
        while(*qptr <= qsize && srcq[(*qptr)] != '\n') {
          (*qptr)++;
        }
      }
      else {
        put_token(
            tokens, tokensptr, srcq, *qptr, start, 
            TOKEN_SLASH);
      }
      break;
    case '"': // Handling string literals
      while (*qptr <= qsize && srcq[*qptr] != '"') {
        (*qptr)++;
      }
      if (*qptr > qsize) { // unfinished str but ptr at end
        puterr(&error, ERROR_RUNTIME, 
            "Unterminated String Literal.");
      }
      else {
        put_token(
            tokens, tokensptr, srcq, ++(*qptr), start, 
            TOKEN_STRING);
      }
      break;
    default:
      if (isdigit(c)) { // Handle numeric literals
        // Read and iterate src queue while numeric.
        while (*qptr <= qsize && isdigit(srcq[(*qptr)++]))
          ;
        // Handle floats
        if (*qptr <= qsize && srcq[(*qptr)-1] == '.') {
          while (*qptr <= qsize && isdigit(srcq[(*qptr)++]))
            ;
        }
        (*qptr)--; // While loop had to lookahead

        put_token( // Put the numeric literal token.
            tokens, tokensptr, srcq, *qptr, start,
            TOKEN_NUMBER);

        break;
      }
      if (isspace(c)) {
        break;
      }
      else {
        puterr(&error, ERROR_RUNTIME, "Unexpected token.");
        break;
      }
  }

  return error;
}

#endif
