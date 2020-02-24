#include "expr_parser.h"
#include "parser.h"
#include "lexer.h"
#include "datatypes.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct list* parse_FUNCTION(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    char* p1 = eat(lex, SYM_IDENTIFIER);
    eat(lex, SYM_LPARENTHESIS);
    struct list* p3 = parse_ARGS(lex);
    eat(lex, SYM_RPARENTHESIS);
    eat(lex, SYM_LBRACE);
    struct list* p6 = parse_IBLOCK(lex);
    eat(lex, SYM_RBRACE);

     return cons(make_string_leaf(strdup(p1)), cons(make_node(AST_NODE_FUNARGS, p3), cons(make_node(AST_NODE_FUNBODY, cons(make_node(AST_IBLOCK, p6), NULL)), NULL))); 
}
  syntax_error_message(lex, "error while parsing FUNCTION");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct list* parse_ARGS(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    struct ast_node* p1 = parse_ARG(lex);
    struct list* p2 = parse_ARGP(lex);

     return cons(p1, p2); 
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    struct ast_node* p1 = parse_ARG(lex);
    struct list* p2 = parse_ARGP(lex);

     return cons(p1, p2); 
}
  if(lex->symbol.tag ==  SYM_RPARENTHESIS){

     return NULL; 
}
  syntax_error_message(lex, "error while parsing ARGS");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf(", %s", string_of_token(SYM_RPARENTHESIS));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct list* parse_ARGP(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_RPARENTHESIS){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_COMMA){
    eat(lex, SYM_COMMA);
    struct ast_node* p2 = parse_ARG(lex);
    struct list* p3 = parse_ARGP(lex);

     return cons(p2, p3); 
}
  syntax_error_message(lex, "error while parsing ARGP");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_RPARENTHESIS));
  printf(", %s", string_of_token(SYM_COMMA));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct list* parse_IBLOCK(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    struct ast_node* p1 = parse_STATEMENT(lex);
    struct list* p2 = parse_IBLOCK(lex);

     return cons(p1,p2); 
}
  if(lex->symbol.tag ==  SYM_IF){
    struct ast_node* p1 = parse_STATEMENT(lex);
    struct list* p2 = parse_IBLOCK(lex);

     return cons(p1,p2); 
}
  if(lex->symbol.tag ==  SYM_RBRACE){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_WHILE){
    struct ast_node* p1 = parse_STATEMENT(lex);
    struct list* p2 = parse_IBLOCK(lex);

     return cons(p1,p2); 
}
  if(lex->symbol.tag ==  SYM_RETURN){
    struct ast_node* p1 = parse_STATEMENT(lex);
    struct list* p2 = parse_IBLOCK(lex);

     return cons(p1,p2); 
}
  if(lex->symbol.tag ==  SYM_PRINT){
    struct ast_node* p1 = parse_STATEMENT(lex);
    struct list* p2 = parse_IBLOCK(lex);

     return cons(p1,p2); 
}
  syntax_error_message(lex, "error while parsing IBLOCK");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_IF));
  printf(", %s", string_of_token(SYM_RBRACE));
  printf(", %s", string_of_token(SYM_WHILE));
  printf(", %s", string_of_token(SYM_RETURN));
  printf(", %s", string_of_token(SYM_PRINT));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct list* parse_CMPP(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_SEMICOLON){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_EQUALITY){
    eat(lex, SYM_EQUALITY);
    struct ast_node* p2 = parse_CMP(lex);
    struct list* p3 = parse_CMPP(lex);

     return cons(make_node(AST_CEQ, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_RPARENTHESIS){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_LT){
    eat(lex, SYM_LT);
    struct ast_node* p2 = parse_CMP(lex);
    struct list* p3 = parse_CMPP(lex);

     return cons(make_node(AST_CLT, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_LEQ){
    eat(lex, SYM_LEQ);
    struct ast_node* p2 = parse_CMP(lex);
    struct list* p3 = parse_CMPP(lex);

     return cons(make_node(AST_CLE, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_GT){
    eat(lex, SYM_GT);
    struct ast_node* p2 = parse_CMP(lex);
    struct list* p3 = parse_CMPP(lex);

     return cons(make_node(AST_CGT, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_GEQ){
    eat(lex, SYM_GEQ);
    struct ast_node* p2 = parse_CMP(lex);
    struct list* p3 = parse_CMPP(lex);

     return cons(make_node(AST_CGE, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_NOTEQ){
    eat(lex, SYM_NOTEQ);
    struct ast_node* p2 = parse_CMP(lex);
    struct list* p3 = parse_CMPP(lex);

     return cons(make_node(AST_CNEQ, NULL), cons(p2, p3)); 
}
  syntax_error_message(lex, "error while parsing CMPP");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_SEMICOLON));
  printf(", %s", string_of_token(SYM_EQUALITY));
  printf(", %s", string_of_token(SYM_RPARENTHESIS));
  printf(", %s", string_of_token(SYM_LT));
  printf(", %s", string_of_token(SYM_LEQ));
  printf(", %s", string_of_token(SYM_GT));
  printf(", %s", string_of_token(SYM_GEQ));
  printf(", %s", string_of_token(SYM_NOTEQ));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct list* parse_TERMP(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_SEMICOLON){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_PLUS){
    eat(lex, SYM_PLUS);
    struct ast_node* p2 = parse_TERM(lex);
    struct list* p3 = parse_TERMP(lex);

     return cons(make_node(AST_EADD, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_MINUS){
    eat(lex, SYM_MINUS);
    struct ast_node* p2 = parse_TERM(lex);
    struct list* p3 = parse_TERMP(lex);

     return cons(make_node(AST_ESUB, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_EQUALITY){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_RPARENTHESIS){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_LT){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_LEQ){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_GT){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_GEQ){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_NOTEQ){

     return NULL; 
}
  syntax_error_message(lex, "error while parsing TERMP");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_SEMICOLON));
  printf(", %s", string_of_token(SYM_PLUS));
  printf(", %s", string_of_token(SYM_MINUS));
  printf(", %s", string_of_token(SYM_EQUALITY));
  printf(", %s", string_of_token(SYM_RPARENTHESIS));
  printf(", %s", string_of_token(SYM_LT));
  printf(", %s", string_of_token(SYM_LEQ));
  printf(", %s", string_of_token(SYM_GT));
  printf(", %s", string_of_token(SYM_GEQ));
  printf(", %s", string_of_token(SYM_NOTEQ));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct list* parse_FACTORP(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_SEMICOLON){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_PLUS){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_MINUS){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_ASTERISK){
    eat(lex, SYM_ASTERISK);
    struct ast_node* p2 = parse_FACTOR(lex);
    struct list* p3 = parse_FACTORP(lex);

     return cons(make_node(AST_EMUL, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_DIV){
    eat(lex, SYM_DIV);
    struct ast_node* p2 = parse_FACTOR(lex);
    struct list* p3 = parse_FACTORP(lex);

     return cons(make_node(AST_EDIV, NULL), cons(p2, p3)); 
}
  if(lex->symbol.tag ==  SYM_EQUALITY){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_RPARENTHESIS){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_LT){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_LEQ){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_GT){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_GEQ){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_NOTEQ){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_MOD){
    eat(lex, SYM_MOD);
    struct ast_node* p2 = parse_FACTOR(lex);
    struct list* p3 = parse_FACTORP(lex);

     return cons(make_node(AST_EMOD, NULL), cons(p2, p3)); 
}
  syntax_error_message(lex, "error while parsing FACTORP");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_SEMICOLON));
  printf(", %s", string_of_token(SYM_PLUS));
  printf(", %s", string_of_token(SYM_MINUS));
  printf(", %s", string_of_token(SYM_ASTERISK));
  printf(", %s", string_of_token(SYM_DIV));
  printf(", %s", string_of_token(SYM_EQUALITY));
  printf(", %s", string_of_token(SYM_RPARENTHESIS));
  printf(", %s", string_of_token(SYM_LT));
  printf(", %s", string_of_token(SYM_LEQ));
  printf(", %s", string_of_token(SYM_GT));
  printf(", %s", string_of_token(SYM_GEQ));
  printf(", %s", string_of_token(SYM_NOTEQ));
  printf(", %s", string_of_token(SYM_MOD));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_S(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    struct list* p1 = parse_FUNCTION(lex);
    eat(lex, SYM_EOF);

     return make_node(AST_NODE_FUNDEF, p1); 
}
  syntax_error_message(lex, "error while parsing S");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_ARG(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    char* p1 = eat(lex, SYM_IDENTIFIER);

     return make_string_leaf(strdup(p1)); 
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    char* p1 = eat(lex, SYM_INTEGER);

     return make_int_leaf(atoi(p1)); 
}
  syntax_error_message(lex, "error while parsing ARG");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_STATEMENT(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    struct ast_node* p1 = parse_AFFECTATION(lex);
    eat(lex, SYM_SEMICOLON);

     return p1; 
}
  if(lex->symbol.tag ==  SYM_IF){
    struct ast_node* p1 = parse_IFELSE(lex);

     return p1; 
}
  if(lex->symbol.tag ==  SYM_WHILE){
    struct ast_node* p1 = parse_WHILE(lex);

     return p1; 
}
  if(lex->symbol.tag ==  SYM_RETURN){
    struct ast_node* p1 = parse_RETURN(lex);
    eat(lex, SYM_SEMICOLON);

     return p1; 
}
  if(lex->symbol.tag ==  SYM_PRINT){
    struct ast_node* p1 = parse_PRINT(lex);
    eat(lex, SYM_SEMICOLON);

     return p1; 
}
  syntax_error_message(lex, "error while parsing STATEMENT");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_IF));
  printf(", %s", string_of_token(SYM_WHILE));
  printf(", %s", string_of_token(SYM_RETURN));
  printf(", %s", string_of_token(SYM_PRINT));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_AFFECTATION(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    char* p1 = eat(lex, SYM_IDENTIFIER);
    eat(lex, SYM_ASSIGN);
    struct ast_node* p3 = parse_EXPR(lex);

     return make_node(AST_IASSIGN, cons(make_string_leaf(strdup(p1)), cons(p3, NULL))); 
}
  syntax_error_message(lex, "error while parsing AFFECTATION");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_RETURN(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_RETURN){
    eat(lex, SYM_RETURN);
    struct ast_node* p2 = parse_EXPR(lex);

     return make_node(AST_IRETURN, cons(p2, NULL)); 
}
  syntax_error_message(lex, "error while parsing RETURN");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_RETURN));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_PRINT(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_PRINT){
    eat(lex, SYM_PRINT);
    struct ast_node* p2 = parse_EXPR(lex);

     return make_node(AST_IPRINT, cons(p2, NULL)); 
}
  syntax_error_message(lex, "error while parsing PRINT");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_PRINT));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_IFELSE(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IF){
    eat(lex, SYM_IF);
    eat(lex, SYM_LPARENTHESIS);
    struct ast_node* p3 = parse_EXPR(lex);
    eat(lex, SYM_RPARENTHESIS);
    eat(lex, SYM_LBRACE);
    struct list* p6 = parse_IBLOCK(lex);
    eat(lex, SYM_RBRACE);
    struct ast_node* p8 = parse_ELSE(lex);

     return make_node(AST_IIFTHENELSE, cons(p3, cons(make_node(AST_IBLOCK, p6), cons(p8, NULL)))); 
}
  syntax_error_message(lex, "error while parsing IFELSE");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IF));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_ELSE(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_IF){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_ELSE){
    eat(lex, SYM_ELSE);
    eat(lex, SYM_LBRACE);
    struct list* p3 = parse_IBLOCK(lex);
    eat(lex, SYM_RBRACE);

     return make_node(AST_IBLOCK, p3); 
}
  if(lex->symbol.tag ==  SYM_RBRACE){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_WHILE){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_RETURN){

     return NULL; 
}
  if(lex->symbol.tag ==  SYM_PRINT){

     return NULL; 
}
  syntax_error_message(lex, "error while parsing ELSE");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_IF));
  printf(", %s", string_of_token(SYM_ELSE));
  printf(", %s", string_of_token(SYM_RBRACE));
  printf(", %s", string_of_token(SYM_WHILE));
  printf(", %s", string_of_token(SYM_RETURN));
  printf(", %s", string_of_token(SYM_PRINT));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_WHILE(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_WHILE){
    eat(lex, SYM_WHILE);
    eat(lex, SYM_LPARENTHESIS);
    struct ast_node* p3 = parse_EXPR(lex);
    eat(lex, SYM_RPARENTHESIS);
    eat(lex, SYM_LBRACE);
    struct list* p6 = parse_IBLOCK(lex);
    eat(lex, SYM_RBRACE);

     return make_node(AST_IWHILE, cons(p3, cons(make_node(AST_IBLOCK, p6), NULL))); 
}
  syntax_error_message(lex, "error while parsing WHILE");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_WHILE));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_EXPR(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    struct ast_node* p1 = parse_CMP(lex);
    struct list* p2 = parse_CMPP(lex);

     return make_node(AST_CMPS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    struct ast_node* p1 = parse_CMP(lex);
    struct list* p2 = parse_CMPP(lex);

     return make_node(AST_CMPS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_MINUS){
    struct ast_node* p1 = parse_CMP(lex);
    struct list* p2 = parse_CMPP(lex);

     return make_node(AST_CMPS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_LPARENTHESIS){
    struct ast_node* p1 = parse_CMP(lex);
    struct list* p2 = parse_CMPP(lex);

     return make_node(AST_CMPS, cons(p1,p2)); 
}
  syntax_error_message(lex, "error while parsing EXPR");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf(", %s", string_of_token(SYM_MINUS));
  printf(", %s", string_of_token(SYM_LPARENTHESIS));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_CMP(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    struct ast_node* p1 = parse_TERM(lex);
    struct list* p2 = parse_TERMP(lex);

     return make_node(AST_TERMS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    struct ast_node* p1 = parse_TERM(lex);
    struct list* p2 = parse_TERMP(lex);

     return make_node(AST_TERMS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_MINUS){
    struct ast_node* p1 = parse_TERM(lex);
    struct list* p2 = parse_TERMP(lex);

     return make_node(AST_TERMS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_LPARENTHESIS){
    struct ast_node* p1 = parse_TERM(lex);
    struct list* p2 = parse_TERMP(lex);

     return make_node(AST_TERMS, cons(p1,p2)); 
}
  syntax_error_message(lex, "error while parsing CMP");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf(", %s", string_of_token(SYM_MINUS));
  printf(", %s", string_of_token(SYM_LPARENTHESIS));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_TERM(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    struct ast_node* p1 = parse_FACTOR(lex);
    struct list* p2 = parse_FACTORP(lex);

     return make_node(AST_FACTORS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    struct ast_node* p1 = parse_FACTOR(lex);
    struct list* p2 = parse_FACTORP(lex);

     return make_node(AST_FACTORS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_MINUS){
    struct ast_node* p1 = parse_FACTOR(lex);
    struct list* p2 = parse_FACTORP(lex);

     return make_node(AST_FACTORS, cons(p1,p2)); 
}
  if(lex->symbol.tag ==  SYM_LPARENTHESIS){
    struct ast_node* p1 = parse_FACTOR(lex);
    struct list* p2 = parse_FACTORP(lex);

     return make_node(AST_FACTORS, cons(p1,p2)); 
}
  syntax_error_message(lex, "error while parsing TERM");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf(", %s", string_of_token(SYM_MINUS));
  printf(", %s", string_of_token(SYM_LPARENTHESIS));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_FACTOR(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    char* p1 = eat(lex, SYM_IDENTIFIER);

     return make_string_leaf(strdup(p1)); 
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    char* p1 = eat(lex, SYM_INTEGER);

     return make_int_leaf(atoi(p1)); 
}
  if(lex->symbol.tag ==  SYM_MINUS){
    eat(lex, SYM_MINUS);
    struct ast_node* p2 = parse_FACTIR(lex);

     return p2; 
}
  if(lex->symbol.tag ==  SYM_LPARENTHESIS){
    eat(lex, SYM_LPARENTHESIS);
    struct ast_node* p2 = parse_EXPR(lex);
    eat(lex, SYM_RPARENTHESIS);

     return p2; 
}
  syntax_error_message(lex, "error while parsing FACTOR");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf(", %s", string_of_token(SYM_MINUS));
  printf(", %s", string_of_token(SYM_LPARENTHESIS));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
struct ast_node* parse_FACTIR(lexer_state* lex){
  if(lex->symbol.tag ==  SYM_IDENTIFIER){
    char* p1 = eat(lex, SYM_IDENTIFIER);

     return make_node(AST_ESUB, cons(make_string_leaf(strdup(p1)), NULL)); 
}
  if(lex->symbol.tag ==  SYM_INTEGER){
    char* p1 = eat(lex, SYM_INTEGER);

     return make_node(AST_ESUB, cons(make_int_leaf(atoi(p1)), NULL)); 
}
  syntax_error_message(lex, "error while parsing FACTIR");
  printf("Expected one of ");
  printf("{ %s", string_of_token(SYM_IDENTIFIER));
  printf(", %s", string_of_token(SYM_INTEGER));
  printf("}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
