#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datatypes.h"
#include "globals.h"
#include "lexer.h"
#include "expr_parser.h"
#include "tree_dump.h"


struct list* eaten_symbols = NULL;

void syntax_error_message(lexer_state* lex, char* message) {
  print_line_number( "syntax error", lex->line_number);
  printf( "%s\n", message);
}

char* eat(lexer_state* lex, enum symbol_t token){
  if (lex->symbol.tag == token){
    char* res = string_of_symbol(lex->symbol);
    eaten_symbols = list_append(eaten_symbols, res);
    next_symbol(lex);
    return res;
  } else {
    syntax_error_message(lex, "unexpected token");
    printf("Expected '%s' but got '%s' instead.\n",
           string_of_token(token), string_of_symbol(lex->symbol));
    exit(1);
    return NULL;
  }
}

void show_lexemes(FILE* fd) {
  struct lexer_state* lex = init_lexer_state(fd);
  next_symbol(lex); // gets the first symbol
  // Now the parser can be run
  while(lex->symbol.tag != SYM_EOF){
    char* sym_string = string_of_symbol(lex->symbol);
    printf("Found symbol '%s'\n", sym_string);
    free(sym_string);
    next_symbol(lex);
  }
  // Free lexer state...
  if(lex->symbol.id)
    free(lex->symbol.id);
  free(lex);
}


struct ast_node* parse_file(FILE* fd) {
  struct lexer_state* lex = init_lexer_state(fd);
  eaten_symbols = NULL;
  next_symbol(lex);
  struct ast_node* res =  parse_S(lex);
  list* n = eaten_symbols;
  while(n){
    free(n->elt);
    n = n->next;
  }
  free_list(eaten_symbols);
  free(lex->symbol.id);
  free(lex);
  return res;
}
