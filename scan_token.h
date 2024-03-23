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
    case '(':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_LEFT_PAREN, *line);
      break;
    case ')':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_RIGHT_PAREN, *line);
      break;
    case '{':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_RIGHT_PAREN, *line);
      break;
    case '}':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_RIGHT_PAREN, *line);
      break;
    case ',':
      put_token(tokens, tokensptr, srcq, *qptr, start,
          TOKEN_COMMA, *line);
      break;
    case '.':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_DOT, *line);
      break;
    case '-':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_MINUS, *line);
      break;
    case '+':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_PLUS, *line);
      break;
    case ';':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_SEMICOLON, *line);
      break;
    case '*':
      put_token(tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_STAR, *line);
      break;
    case '!': // check if the lexeme is continued '!=' or just '!'.
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_BANG_EQUAL, *line):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_BANG, *line);
      break;
    case '=':
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_EQUAL_EQUAL, *line):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_EQUAL, *line);
      break;
    case '<':
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_LESS_EQUAL, *line):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_LESS, *line);
      break;
    case '>':
      (*qptr <= qsize && srcq[(*qptr)] == '=')?
      put_token(
          tokens, tokensptr, srcq, ++(*qptr), start, 
          TOKEN_GREATER_EQUAL, *line):
      put_token(
          tokens, tokensptr, srcq, *qptr, start, 
          TOKEN_GREATER, *line);
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
            TOKEN_SLASH, *line);
      }

      break;
    case '"': // Handling string literals
      while (*qptr <= qsize && srcq[*qptr] != '"') {
        (*qptr)++;
      }

      if (*qptr > qsize) { // unfinished str but ptr at end
        char msg[ERROR_MAX_MSG];
        puterrmsg(
            msg, "Unterminated String Literal", c, *line);
        puterr(&error, ERROR_RUNTIME, msg);
      }
      else {
        put_token(
            tokens, tokensptr, srcq, ++(*qptr), start, 
            TOKEN_STRING, *line);
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
              TOKEN_IDENTIFIER, *line);
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
        puterrmsg(msg, "Unexpected Token", c, *line);
        puterr(&error, ERROR_RUNTIME, msg);
        break;
      }
  }

  return error;
}

#endif
