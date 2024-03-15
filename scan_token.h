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
    case '/':
      if (*qptr <= qsize && srcq[(*qptr)] == '/') {
        while(srcq[(*qptr)] != '\n') {
          (*qptr)++;
        }
      }
      else {
        put_token(
            tokens, tokensptr, srcq, *qptr, start, 
            TOKEN_SLASH);
      }
      break;
    default:
      if (isspace(c)) {
        break;
      }
      else {
        puterr(&error, ERROR_RUNTIME, "Unexpected Token.");
        break;
      }
  }

  return error;
}

#endif
