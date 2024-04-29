/* 
 *  file: scan_token.h
 *
 *  brief:
 *    - A component of PL/0 lexer.
 *    By Iterating thru queue of ascii characters 
 *    containing the raw content of a file while 
 *    keep tracking of the current line. scans 
 *    each token according to PL/0 grammer: 
 *
 *  Author: Metflekx
 * */


#ifndef SCAN_TOKEN_H
#define SCAN_TOKEN_H

/* reads one token, checks if is correct token reads it 
 * into tokens else populates the error with the right msg */
Error *scan_token(Token tokens[], int *tokensptr, char *srcq,
    int *qptr, int qsize, int start, int *line) {
  char c;
  Error *error;

  error = ok;
  c = srcq[(*qptr)++];
  switch (c) {
    case '{': // Handle comments
      while (srcq[(*qptr)++] != '}') {
        if (*qptr > qsize) { // EOF
          char msg[ERROR_MAX_MSG];
          puterrmsg(msg, "Unterminated Comment.", c, *line);
          puterr(&error, ERROR_RUNTIME, msg);
        }
        if (srcq[*qptr] == '\n') { // new line
          (*line)++;
        }
      }
      break;
    case '.':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_DOT, *line);
      break;
    case '=':
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_EQUAL, *line);
      break;
    case ',':
      put_token(tokens, tokensptr, srcq, *qptr, start,
          TOKEN_COMMA, *line);
      break;
    case ';':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_SEMICOLON, *line);
      break;
    case '#':
      break;
    case '<':
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_LESS, *line);
      break;
    case '>':
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_GREATER, *line);
      break;
    case '+':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_PLUS, *line);
      break;
    case '-':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_MINUS, *line);
      break;
    case '*':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_MULTIPLY, *line);
      break;
    case '/':
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_DIVIDE, *line);
      break;
    case '(':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_LPAREN, *line);
      break;
    case ')':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_RPAREN, *line);
      break;
    case ':':
      if (srcq[*qptr] != '=') { // Invalid token
        char msg[ERROR_MAX_MSG];
        puterrmsg(
            msg, "Unexpected token: :", srcq[*qptr], *line);
        puterr(&error, ERROR_RUNTIME, msg);
      }
      else {
        put_token(tokens, tokensptr, srcq, *qptr, start,
            TOKEN_ASSIGN, *line);
      }
      break;
    default:
      if (isdigit(c)) { // Handle numeric literals
        // Read and iterate src queue while numeric.
        while (*qptr <= qsize && isdigit(srcq[(*qptr)++]))
          ;
        (*qptr)--;
        put_token( // Put the numeric literal token.
            tokens, tokensptr, srcq, *qptr, start,
            TOKEN_NUMBER, *line);

        break;
      }

      // Handle Identifiers and Keywords
      if (isalpha(c)) {
        while (*qptr <= qsize && isalnum(srcq[(*qptr)++]))
          ;
        (*qptr)--; // while had to lookahead
        
        // Get Substr check if is keyword Else is identifier
        int length = *qptr - start; // length of substr
        char substr[length];
        Keyword *kword;
        strncpy(substr, srcq + start, length);
        substr[length] = '\0'; // null terminated substr
        
        // Check if kword
        if ((kword = kmaplookup(kmap, substr))) {  
          put_token(
              tokens, tokensptr, srcq, *qptr, start, 
              kword->type, *line);
        }
        else { // Is an identifier
          put_token(
              tokens, tokensptr, srcq, *qptr, start, 
              TOKEN_IDENT, *line);
        }
        break;
      }

      if (isspace(c)) {
        if (c == '\n') {
          (*line)++;
        }
        break;
      }
      else {
        // Put Error
        char msg[ERROR_MAX_MSG];
        puterrmsg(msg, "unexpected token", c, *line);
        puterr(&error, ERROR_RUNTIME, msg);
        break;
      }
  }

  return error;
}

#endif
