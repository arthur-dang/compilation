#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ast.h"
#include "tree_dump.h"
#include "elang.h"
#include "elang_print.h"

struct expression* new_expr(){
  return (struct expression*)malloc(sizeof(struct expression));
}


struct expression* copy_expression(struct expression* e){
  struct expression* ne = new_expr();
  ne->etype = e->etype;
  switch(e->etype){
  case EINT:
    ne->eint.i = e->eint.i;
    break;
  case EVAR:
    ne->var.s = strdup(e->var.s);
    break;
  case EUNOP:
    ne->unop.unop = e->unop.unop;
    ne->unop.e = copy_expression(e->unop.e);
    break;
  case EBINOP:
    ne->binop.binop = e->binop.binop;
    ne->binop.e1 = copy_expression(e->binop.e1);
    ne->binop.e2 = copy_expression(e->binop.e2);
    break;
  }
  return ne;
}

struct expression* make_expr_binop(enum binop_t bin, struct expression* e1, struct expression* e2){
  struct expression* e = new_expr();
  e->etype = EBINOP;
  e->binop.binop = bin;
  e->binop.e1 = e1;
  e->binop.e2 = e2;
  return e;
}

struct expression* make_expr_unop(enum unop_t un, struct expression* e1){
  struct expression* e = new_expr();
  e->etype = EUNOP;
  e->unop.unop = un;
  e->unop.e = e1;
  return e;
}

struct expression* make_expr_int(int i){
  struct expression* e = new_expr();
  e->etype = EINT;
  e->eint.i = i;
  return e;
}

struct expression* make_expr_var(char* s){
  struct expression* e = new_expr();
  e->etype = EVAR;
  e->var.s = s;
  return e;
}

enum binop_t binop_of_ast(enum ast_tag t){
  switch(t){
  case AST_EADD: return EADD;
  case AST_ESUB: return ESUB;
  case AST_EMOD: return EMOD;
  case AST_EMUL: return EMUL;
  case AST_EDIV: return EDIV;
  case AST_CEQ: return CEQ;
  case AST_CNEQ: return CNEQ;
  case AST_CLE: return CLE;
  case AST_CLT: return CLT;
  case AST_CGE: return CGE;
  case AST_CGT: return CGT;
  default:
    printf("In binop_of_ast, unexpected tag: %d\n", t);
    exit(1);
  }
}

enum unop_t unop_of_ast(enum ast_tag t){
  switch(t){
  case AST_ENEG: return ENEG;
  default:
    printf("In unop_of_ast, unexpected tag: %d\n", t);
    exit(1);
  }
}

struct expression* make_expr(struct ast_node* ast);

struct expression* make_terms(struct list* l){
  struct expression* res = NULL;
  // À compléter !
  return res;
}

struct expression* make_expr(struct ast_node* ast){
  switch(ast->tag){
  case AST_TERMS:
  case AST_EQS:
  case AST_CMPS:
  case AST_FACTORS:
    return make_terms(ast->children);
  case AST_ENEG:
    return make_expr_unop(ENEG, make_expr(list_nth(ast->children,0)));
  case AST_INT:
    return make_expr_int(int_of_int_leaf(ast));
  case AST_STRING:
    return make_expr_var(string_of_string_leaf(ast));
  default:
    printf("In make_expr, unexpected tag: %s\n", get_tag_name(ast->tag));
    exit(1);
  }
}

struct instruction* new_instr(){
  return (struct instruction*)malloc(sizeof(struct instruction));
}

struct instruction* make_instr_assign(char* var, struct expression* e){
  struct instruction* i = new_instr();
  i->type = IASSIGN;
  i->iassign.var = var;
  i->iassign.e = e;
  return i;
}

struct instruction* make_instr_return(struct expression* e){
  struct instruction* i = new_instr();
  i->type = IRETURN;
  i->ireturn.e = e;
  return i;
}

struct instruction* make_instr_print(struct expression* e){
  struct instruction* i = new_instr();
  i->type = IPRINT;
  i->iprint.e = e;
  return i;
}

struct instruction* make_instr_ifthenelse(struct expression* cmp,
                                         struct instruction* ithen,
                                         struct instruction *ielse){
  struct instruction* i = new_instr();
  i->type = IIFTHENELSE;
  i->iif.cmp = cmp;
  i->iif.ithen = ithen;
  i->iif.ielse = ielse;
  return i;
}

struct instruction* make_instr_while(struct expression* cmp,
                                     struct instruction* ii){
  struct instruction* i = new_instr();
  i->type = IWHILE;
  i->iwhile.cmp = cmp;
  i->iwhile.i = ii;
  return i;
}

struct instruction* make_instr(struct ast_node* ast){

  switch(ast->tag){
  case AST_IBLOCK:
    {
         // À compléter !
         return NULL;
    }
  case AST_IASSIGN:
    return make_instr_assign(string_of_string_leaf(list_nth(ast->children,0)),
                             make_expr(list_nth(ast->children,1)));
  case AST_IRETURN:
    return make_instr_return(make_expr(list_nth(ast->children,0)));
  case AST_IPRINT:
    return make_instr_print(make_expr(list_nth(ast->children,0)));
  case AST_IIFTHENELSE:
    return make_instr_ifthenelse(make_expr(list_nth(ast->children,0)),
                                 make_instr(list_nth(ast->children,1)),
                                 make_instr(list_nth(ast->children,2)));
  case AST_IWHILE:
    return make_instr_while(make_expr(list_nth(ast->children,0)),
                            make_instr(list_nth(ast->children,1)));
  default:
    printf("In make_instr, unexpected tag: %s\n", get_tag_name(ast->tag));
    exit(1);
  }
}

struct eprog* make_eprog(struct ast_node* ast){
  /* if(!ast){ */
  /*   fprintf(stderr, "make_eprog: the AST is NULL! Aborting...\n"); */
  /*   exit(1); */
  /* } */
  /* struct eprog* p = (struct eprog*)malloc(sizeof(struct eprog)); */
  /* assert(ast->tag == AST_NODE_FUNDEF); */
  /* assert(list_length(ast->children) == 3); */
  /* p->fname = string_of_string_leaf(list_nth(ast->children,0)); */
  /* p->args = NULL; */
  /* struct ast_node* nargs = list_nth(ast->children,1); */
  /* assert(nargs->tag == AST_NODE_FUNARGS); */
  /* list* largs = nargs->children; */
  /* while(largs){ */
  /*   p->args = list_append(p->args, string_of_string_leaf(largs->elt)); */
  /*   largs = largs->next; */
  /* } */
  /* struct ast_node* body = list_nth(ast->children,2); */
  /* assert(body->tag == AST_NODE_FUNBODY); */
  /* p->body = make_instr(list_nth(body->children,0)); */
  /* return p; */
  return NULL;
}

void free_expression(struct expression* e){
  if(e){
    switch(e->etype){
    case EINT:
      break;
    case EVAR:
      free(e->var.s);
      break;
    case EUNOP:
      free_expression(e->unop.e);
      break;
    case EBINOP:
      free_expression(e->binop.e1);
      free_expression(e->binop.e2);
      break;
    }
    free(e);
  }
}


void free_instruction(struct instruction* i){
  if(i){
    switch(i->type){
    case IIFTHENELSE:
      free_expression(i->iif.cmp);
      free_instruction(i->iif.ithen);
      free_instruction(i->iif.ielse);
      break;
    case IWHILE:
      free_expression(i->iwhile.cmp);
      free_instruction(i->iwhile.i);
      break;
    case IASSIGN:
      free(i->iassign.var);
      free_expression(i->iassign.e);
      break;
    case IRETURN:
      free_expression(i->ireturn.e);
      break;
    case IPRINT:
      free_expression(i->iprint.e);
      break;
    case IBLOCK:
      {
        list* n = i->iblock.l;
        while(n){
          free_instruction(n->elt);
          n = n -> next;
        }
        free_list(i->iblock.l);
      }
    }
    free(i);
  }
}

void free_eprog(struct eprog* p){
  if(p){
    free(p->fname);
    list* largs = p->args;
    while(largs){
      free(largs->elt);
      largs = largs->next;
    }
    free_list(p->args);
    free_instruction(p->body);
    free(p);
  }
}
