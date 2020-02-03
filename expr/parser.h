#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

struct ast_node* parse_file(FILE*);
void show_lexemes(FILE* fd);
void syntax_error_message(lexer_state* lex, char* message);

char* eat(lexer_state*, enum symbol_t tok);

#endif
