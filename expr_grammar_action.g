tokens SYM_EOF SYM_IDENTIFIER SYM_INTEGER SYM_SEMICOLON SYM_IF SYM_ELSE SYM_PLUS SYM_MINUS SYM_ASTERISK SYM_DIV SYM_EQUALITY SYM_ASSIGN SYM_LPARENTHESIS SYM_RPARENTHESIS SYM_LBRACE SYM_RBRACE SYM_WHILE SYM_RETURN SYM_PRINT SYM_COMMA SYM_LT SYM_LEQ SYM_GT SYM_GEQ SYM_NOTEQ SYM_MOD
non-terminals<struct ast_node*> S ARG STATEMENT AFFECTATION RETURN PRINT IFELSE WHILE EXPR CMP TERM FACTOR
non-terminals<struct list*> FUNCTION ARGS ARGP IBLOCK IF ELSE CMPP TERMP FACTORP
rules

S -> FUNCTION SYM_EOF { return make_node(AST_NODE_FUNDEF, $1); }
FUNCTION -> SYM_IDENTIFIER SYM_LPARENTHESIS ARGS SYM_RPARENTHESIS SYM_LBRACE IBLOCK SYM_RBRACE { return cons(make_string_leaf(strdup($1)), cons(make_node(AST_NODE_FUNARGS, $3), cons(make_node(AST_NODE_FUNBODY, cons(make_node(AST_IBLOCK, $6), NULL)), NULL))); }
ARGS -> ARG ARGP { return cons($1, $2); }
ARGS -> { return NULL; }
ARGP -> SYM_COMMA ARG ARGP { return cons($2, $3); }
ARGP -> { return NULL; }
ARG -> SYM_IDENTIFIER { return make_string_leaf(strdup($1)); }
ARG -> SYM_INTEGER { return make_int_leaf(atoi($1)); }

IBLOCK -> STATEMENT IBLOCK { return cons($1,$2); }
IBLOCK -> { return NULL; }
STATEMENT -> AFFECTATION SYM_SEMICOLON { return $1; }
STATEMENT -> RETURN SYM_SEMICOLON { return $1; }
STATEMENT -> PRINT SYM_SEMICOLON { return $1; }
STATEMENT -> WHILE { return $1; }
STATEMENT -> IFELSE { return $1; }

AFFECTATION -> SYM_IDENTIFIER SYM_ASSIGN EXPR { return make_node(AST_IASSIGN, cons(make_string_leaf(strdup($1)), cons($3, NULL))); }

RETURN -> SYM_RETURN EXPR { return make_node(AST_IRETURN, cons($2, NULL)); }

PRINT -> SYM_PRINT EXPR { return make_node(AST_IPRINT, cons($2, NULL)); }

WHILE -> SYM_WHILE SYM_LPARENTHESIS EXPR SYM_RPARENTHESIS SYM_LBRACE IBLOCK SYM_RBRACE { return make_node(AST_IWHILE, cons($3, $6)); }

IFELSE -> IF ELSE { return make_node(AST_IIFTHENELSE, cons($1, $2)); }
IF -> SYM_IF SYM_LPARENTHESIS EXPR SYM_RPARENTHESIS SYM_LBRACE IBLOCK SYM_RBRACE { return cons($3, $6); }
ELSE -> SYM_ELSE SYM_LBRACE IBLOCK SYM_RBRACE { return $3; }
ELSE -> { return NULL; }

EXPR -> CMP CMPP { return make_node(AST_CMPS, cons($1,$2)); }
CMPP -> SYM_EQUALITY CMP CMPP { return cons(make_node(AST_CEQ, NULL), cons($2, $3)); }
CMPP -> SYM_LT CMP CMPP { return cons(make_node(AST_CLT, NULL), cons($2, $3)); }
CMPP -> SYM_LEQ CMP CMPP { return cons(make_node(AST_CLE, NULL), cons($2, $3)); }
CMPP -> SYM_GT CMP CMPP { return cons(make_node(AST_CGT, NULL), cons($2, $3)); }
CMPP -> SYM_GEQ CMP CMPP { return cons(make_node(AST_CGE, NULL), cons($2, $3)); }
CMPP -> SYM_NOTEQ CMP CMPP { return cons(make_node(AST_CNEQ, NULL), cons($2, $3)); }
CMPP -> { return NULL; }
CMP -> TERM TERMP { return make_node(AST_TERMS, cons($1,$2)); }
TERMP -> SYM_PLUS TERM TERMP { return cons(make_node(AST_EADD, NULL), cons($2, $3)); }
TERMP -> SYM_MINUS TERM TERMP { return cons(make_node(AST_ESUB, NULL), cons($2, $3)); }
TERMP -> { return NULL; }
TERM -> FACTOR FACTORP { return make_node(AST_FACTORS, cons($1,$2)); }
FACTORP -> SYM_ASTERISK FACTOR FACTORP { return cons(make_node(AST_EMUL, NULL), cons($2, $3)); }
FACTORP -> SYM_DIV FACTOR FACTORP { return cons(make_node(AST_EDIV, NULL), cons($2, $3)); }
FACTORP -> SYM_MOD FACTOR FACTORP { return cons(make_node(AST_EMOD, NULL), cons($2, $3)); }
FACTORP -> { return NULL; }
FACTOR -> SYM_IDENTIFIER { return make_string_leaf(strdup($1)); }
FACTOR -> SYM_INTEGER { return make_int_leaf(atoi($1)); }
FACTOR -> SYM_LPARENTHESIS EXPR SYM_RPARENTHESIS { return $2; }
