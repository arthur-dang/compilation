#include "expr_parser.h"
#include "parser.h"
#include "lexer.h"
#include "datatypes.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void* parse_S(lexer_state* lex){
  syntax_error_message(lex, "error while parsing S");
  printf("Expected one of ");
  printf("{}");
  printf(" but got '%s' instead.\n", string_of_symbol(lex->symbol));
  exit(1);
}
