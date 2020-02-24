#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include "globals.h"
#include "lexer.h"

char CHAR_EOF          =  -1; // end of file

char* symbol_name (int symbol) {
  if (symbol == SYM_IDENTIFIER) return "identifier";
  if (symbol == SYM_INTEGER) return "integer";
  if (symbol == SYM_SEMICOLON) return ";";
  if (symbol == SYM_IF) return "if";
  if (symbol == SYM_ELSE) return "else";
  if (symbol == SYM_PLUS) return "+";
  if (symbol == SYM_MINUS) return "-";
  if (symbol == SYM_ASTERISK) return "*";
  if (symbol == SYM_DIV) return "/";
  if (symbol == SYM_EQUALITY) return "==";
  if (symbol == SYM_ASSIGN) return "=";
  if (symbol == SYM_LPARENTHESIS) return "(";
  if (symbol == SYM_RPARENTHESIS) return ")";
  if (symbol == SYM_LBRACE) return "{";
  if (symbol == SYM_RBRACE) return "}";
  if (symbol == SYM_WHILE) return "while";
  if (symbol == SYM_RETURN) return "return";
  if (symbol == SYM_PRINT) return "print";
  if (symbol == SYM_COMMA) return ",";
  if (symbol == SYM_LT) return "<";
  if (symbol == SYM_LEQ) return "<=";
  if (symbol == SYM_GT) return ">";
  if (symbol == SYM_GEQ) return ">=";
  if (symbol == SYM_NOTEQ) return "!=";
  if (symbol == SYM_MOD) return "%";
  return NULL;
}

char* string_of_symbol(symb symbol) {
  switch(symbol.tag){
  case SYM_EOF:
    return strdup("end of file");
  case SYM_IDENTIFIER:
  case SYM_INTEGER:
    return strdup(symbol.id);
  default:
    return strdup(symbol_name(symbol.tag));
  }
}

char* string_of_token(enum symbol_t symbol) {
  if (symbol == SYM_EOF)
    return "end of file";
  else
    return symbol_name(symbol);
}

void print_line_number(char* message, int line) {
  printf("%s: %s in %s in line %d: ",
         myname, message, source_name, line);
}

void lexer_error_message(lexer_state* lex, char* message) {
  print_line_number( "lexer error", lex->line_number);
  printf( "%s\n", message);
}


void lexer_error_character(lexer_state* lex, char character, char expected) {
  print_line_number("lexer error", lex->line_number);
  printf("%c expected but %c found\n", expected, character);
}


int is_character_new_line(char character) {
  return (character == '\n' || character == '\r');
}

int is_character_whitespace(char character) {
  return character == ' ' || character == '\t' || is_character_new_line(character);
}

int identifier_or_keyword(char* identifier) {
  if (!strcmp(identifier, symbol_name(SYM_WHILE)))
    return SYM_WHILE;
  if (!strcmp(identifier, symbol_name(SYM_IF)))
    return SYM_IF;
  if (!strcmp(identifier, symbol_name(SYM_ELSE)))
    return SYM_ELSE;
  if (!strcmp(identifier, symbol_name(SYM_RETURN)))
    return SYM_RETURN;
  if (!strcmp(identifier, symbol_name(SYM_PRINT)))
    return SYM_PRINT;
  else
    return SYM_IDENTIFIER;
}

lexer_state* init_lexer_state(FILE* fd){
  lexer_state* lex = (lexer_state*)malloc(sizeof(lexer_state));
  lex->lexer_fd = fd;
  lex->curpos = 0;
  lex->line_number = 1;
  lex->symbol.tag = SYM_EOF;
  lex->symbol.id = NULL;
  return lex;
}

char get_character_at_position(FILE* fd, long pos) {
  int number_of_read_bytes;
  char character_buffer;
  if(fseek(fd, pos, SEEK_SET)){
    printf("%s: could not fseek at position %ld in input file\n", myname, pos);
    exit(EXITCODE_IOERROR);
  }
  number_of_read_bytes = fread(&character_buffer, sizeof(char), 1, fd);
  if (number_of_read_bytes == 1) {
    return character_buffer;
  } else if (number_of_read_bytes == 0)
    return CHAR_EOF;
  else {
    printf("%s: could not read character at position %ld from input file\n", myname, pos);
    exit(EXITCODE_IOERROR);
  }
}

char get_character(lexer_state* lex) {
  return get_character_at_position(lex->lexer_fd, lex->curpos);
}

void next_character(lexer_state* lex){
  lex->curpos++;
}
void prev_character(lexer_state* lex){
  lex->curpos--;
}

/* Called with lex just after "//"; return with lex just after a line break */
void skip_single_line_comment(lexer_state* lex){
    char character = get_character(lex);
    while (!is_character_new_line(character)){
        next_character(lex);
        character = get_character(lex);
    }
    next_character(lex);
}

/* Called with lex just after "/ *"; return with lex just after a "* /" */
void skip_multi_line_comment(lexer_state* lex){
    int out = 0;
    while (out == 0){
        char character = get_character(lex);
        if (character == '*') {
            next_character(lex);
            character = get_character(lex);
            if(character == '/'){
                out = 1;
            } else {
                prev_character(lex);
            }
        }
        next_character(lex);
    }
}

void find_next_character(lexer_state* lex) {
    // TODO: selon le lexer_state avancer la position dans le fichier jusqu'au prochain caractère significatif
    // 1) Avancer d'une position
    // 2) Regarder si c'est un caractère significatif
    // 3) Itérer jusqu'à ce que ce soit un caractère significatif

    char character = get_character(lex);
    while ( is_character_new_line(character) || is_character_whitespace(character)) {
        next_character(lex);
        character = get_character(lex);
    }
    if (character == '/') {
        next_character(lex);
        character = get_character(lex);
        if (character == '/') {
            next_character(lex);
            skip_single_line_comment(lex);
        }
        else if (character == '*') {
            next_character(lex);
            skip_multi_line_comment(lex);
        }
        else {
            prev_character(lex);
        }
    }

    // Si termine le commentaire par une fin à la ligne faut refaire
    character = get_character(lex);
    while ( is_character_new_line(character) || is_character_whitespace(character)) {
        next_character(lex);
        character = get_character(lex);
    }
}

void next_symbol(lexer_state* lex) {
  int i;
  find_next_character(lex);
  char character = get_character(lex);
  if (character == CHAR_EOF) {
    lex->symbol.tag = SYM_EOF;
  }
  else if (isdigit(character)) { // Check si c'est un chiffre
    // accommodate integer and null for termination
    if(lex->symbol.id) free(lex->symbol.id);
    lex->symbol.id = malloc(MAX_INTEGER_LENGTH + 1);
    i = 0;
    while (isdigit(character)) {
      if (i >= MAX_INTEGER_LENGTH) {
        lexer_error_message(lex, "integer out of bound");
        exit(EXITCODE_SCANNERERROR);
      }
      lex->symbol.id[i] = character;
      i++;
      next_character(lex);
      character = get_character(lex);
    }
    lex->symbol.id[i] =  0; // null-terminated string
    lex->symbol.tag = SYM_INTEGER;
  }
  else if (isalpha(character)) {
      char temp[MAX_IDENTIFIER_LENGTH]= "";
      if(lex->symbol.id) free(lex->symbol.id);
      lex->symbol.id = malloc(MAX_IDENTIFIER_LENGTH + 1);
      i = 0;
      while (isalnum(character) || character == '_' || character == '-') {
          strncat(temp, &character, 1);
          if (i >= MAX_IDENTIFIER_LENGTH) {
              lexer_error_message(lex, "identifier out of bound");
              exit(EXITCODE_SCANNERERROR);
          }
          lex->symbol.id[i] = character;
          i++;
          next_character(lex);
          character = get_character(lex);
      }
      if (strcmp(temp,"if") == 0) {
          lex->symbol.tag = SYM_IF;
          lex->symbol.id = NULL;
      } else if (strcmp(temp,"else") == 0) {
          lex->symbol.tag = SYM_ELSE;
          lex->symbol.id = NULL;
      } else if (strcmp(temp,"return") == 0) {
          lex->symbol.tag = SYM_RETURN;
          lex->symbol.id = NULL;
      } else if (strcmp(temp,"while") == 0) {
          lex->symbol.tag = SYM_WHILE;
          lex->symbol.id = NULL;
      } else if (strcmp(temp,"print") == 0) {
          lex->symbol.tag = SYM_PRINT;
          lex->symbol.id = NULL;
      } else {
          lex->symbol.id[i] = 0; // null-terminated string
          lex->symbol.tag = SYM_IDENTIFIER;
      }
  }
  else if (character == ';') {
    lex->symbol.tag = SYM_SEMICOLON;
    next_character(lex);
  } else if (character  == '('){
    lex->symbol.tag = SYM_LPARENTHESIS;
    next_character(lex);
  } else if (character  == ')'){
    lex->symbol.tag = SYM_RPARENTHESIS;
    next_character(lex);
  } else if (character  == '{'){
    lex->symbol.tag = SYM_LBRACE;
    next_character(lex);
  } else if (character  == '}'){
    lex->symbol.tag = SYM_RBRACE;
    next_character(lex);
  } else if (character  == '+'){
    lex->symbol.tag = SYM_PLUS;
    next_character(lex);
  } else if (character  == '-'){
    lex->symbol.tag = SYM_MINUS;
    next_character(lex);
  } else if (character  == ','){
    lex->symbol.tag = SYM_COMMA;
    next_character(lex);
  } else if (character  == '%'){
    lex->symbol.tag = SYM_MOD;
    next_character(lex);
  } else if (character  == '*'){
    lex->symbol.tag = SYM_ASTERISK;
    next_character(lex);
  } else if (character  == '/'){
    lex->symbol.tag = SYM_DIV;
    next_character(lex);
  }
  else if (character == '<') {
    next_character(lex);
    character = get_character(lex);
    if (character == '=') {
        lex->symbol.tag = SYM_LEQ;
        next_character(lex);
    } else {
        lex->symbol.tag = SYM_LT;
    }
  } else if (character == '>') {
      next_character(lex);
      character = get_character(lex);
      if (character == '=') {
          lex->symbol.tag = SYM_GEQ;
          next_character(lex);
      } else {
          lex->symbol.tag = SYM_GT;
      }
  } else if (character == '=') {
      next_character(lex);
      character = get_character(lex);
      if (character == '=') {
          lex->symbol.tag = SYM_EQUALITY;
          next_character(lex);
      } else {
          lex->symbol.tag = SYM_ASSIGN;
      }
  } else if (character == '!') {
      next_character(lex);
      character = get_character(lex);
      if (character == '=') {
          lex->symbol.tag = SYM_NOTEQ;
          next_character(lex);
      } else {
          print_line_number( "lexer error", lex->line_number);
          printf("found unknown character %c\n", character);
          exit(EXITCODE_SCANNERERROR);
      }
  }
  else {
    print_line_number( "lexer error", lex->line_number);
    printf("found unknown character %c\n", character);
    exit(EXITCODE_SCANNERERROR);
  }
  return;
}
