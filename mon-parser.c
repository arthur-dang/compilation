#include "mon-parser.h"
#include "parser.h"
#include "lexer.h"
#include "datatypes.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void* parse_S(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    void* p1 = parse_EXPR(lex);
    char* p2 = eat(lex, SYM_EOF);

    return cons(p1,cons(p2,list_new()));
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    void* p1 = parse_EXPR(lex);
    char* p2 = eat(lex, SYM_EOF);

    return cons(p1,cons(p2,list_new()));
}
  syntax_error_message(lex, "error while parsing S");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
void* parse_IDENTIFIER(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    char* p1 = eat(lex, SYM_IDENTIFIER);

    return cons(p1,list_new());
}
  syntax_error_message(lex, "error while parsing IDENTIFIER");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
void* parse_INTEGER(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_INTEGER){
    char* p1 = eat(lex, SYM_INTEGER);

    return cons(p1,list_new());
}
  syntax_error_message(lex, "error while parsing INTEGER");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_INTEGER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
void* parse_EXPR(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    void* p1 = parse_TERM(lex);
    void* p2 = parse_TERMS(lex);

    return cons(p1,cons(p2,list_new()));
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    void* p1 = parse_TERM(lex);
    void* p2 = parse_TERMS(lex);

    return cons(p1,cons(p2,list_new()));
}
  syntax_error_message(lex, "error while parsing EXPR");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
void* parse_TERM(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    void* p1 = parse_FACTOR(lex);
    void* p2 = parse_FACTORS(lex);

    return cons(p1,cons(p2,list_new()));
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    void* p1 = parse_FACTOR(lex);
    void* p2 = parse_FACTORS(lex);

    return cons(p1,cons(p2,list_new()));
}
  syntax_error_message(lex, "error while parsing TERM");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
void* parse_TERMS(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_PLUS){
    char* p1 = eat(lex, SYM_PLUS);
    void* p2 = parse_TERM(lex);
    void* p3 = parse_TERMS(lex);

    return cons(p1,cons(p2,cons(p3,list_new())));
}
  if(lex->symbol.tag ==  SYM_MINUS){
    char* p1 = eat(lex, SYM_MINUS);
    void* p2 = parse_TERM(lex);
    void* p3 = parse_TERMS(lex);

    return cons(p1,cons(p2,cons(p3,list_new())));
}
  syntax_error_message(lex, "error while parsing TERMS");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_PLUS));
  printf(", %s", string_of_token(SYM_MINUS));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
void* parse_FACTOR(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    void* p1 = parse_IDENTIFIER(lex);

    return cons(p1,list_new());
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    void* p1 = parse_INTEGER(lex);

    return cons(p1,list_new());
}
  syntax_error_message(lex, "error while parsing FACTOR");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
void* parse_FACTORS(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_ASTERISK){
    char* p1 = eat(lex, SYM_ASTERISK);
    void* p2 = parse_FACTOR(lex);
    void* p3 = parse_FACTORS(lex);

    return cons(p1,cons(p2,cons(p3,list_new())));
}
  if(lex->symbol.tag ==  SYM_DIV){
    char* p1 = eat(lex, SYM_DIV);
    void* p2 = parse_FACTOR(lex);
    void* p3 = parse_FACTORS(lex);

    return cons(p1,cons(p2,cons(p3,list_new())));
}
  syntax_error_message(lex, "error while parsing FACTORS");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_ASTERISK));
  printf(", %s", string_of_token(SYM_DIV));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
