#include <stdlib.h>
#include <string.h>
#include "elang.h"
#include "cfg.h"
#include "rtl.h"

rtl_op* new_rtl_op_mov(int rd, int rs){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RMOV;
  op->mov.rd = rd;
  op->mov.rs = rs;
  return op;
}

rtl_op* new_rtl_op_label(int label){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RLABEL;
  op->label.lab = label;
  return op;
}

rtl_op* new_rtl_op_print(int rs){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RPRINT;
  op->print.rs = rs;
  return op;
}

rtl_op* new_rtl_op_return(int rs){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RRET;
  op->ret.rs = rs;
  return op;
}

rtl_op* new_rtl_op_goto(int succ){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RGOTO;
  op->rgoto.succ = succ;
  return op;
}

rtl_op* new_rtl_op_branch(int rs, int succtrue, int succfalse){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RBRANCH;
  op->branch.rs = rs;
  op->branch.succtrue = succtrue;
  op->branch.succfalse = succfalse;
  return op;
}

rtl_op* new_rtl_op_binop(binop_t b, int rd, int rs1, int rs2){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RBINOP;
  op->binop.binop = b;
  op->binop.rd = rd;
  op->binop.rs1 = rs1;
  op->binop.rs2 = rs2;
  return op;
}

rtl_op* new_rtl_op_unop(unop_t u, int rd, int rs){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RUNOP;
  op->unop.unop = u;
  op->unop.rd = rd;
  op->unop.rs = rs;
  return op;
}

rtl_op* new_rtl_op_imm(int rd, int imm){
  rtl_op* op = malloc(sizeof(rtl_op));
  op->type = RIMM;
  op->imm.rd = rd;
  op->imm.imm = imm;
  return op;
}

typedef struct symtable {
  struct symtable* next;
  char* var;
  int reg;
} symtable;

void free_symtable(symtable* s){
  if(s){
    free(s->var);
    free_symtable(s->next);
    free(s);
  }
}

symtable* symt = NULL;

int curreg = 0;

int new_reg(){
  return curreg++;
}

int get_reg_for_var(char* v){
     // À compléter !
     return -1;
}

typedef struct expr_compiled {
  int r;
  list* ops;
} expr_compiled;

expr_compiled* new_expr_compiled(){
  expr_compiled* ec = (expr_compiled*)malloc(sizeof(expr_compiled));
  ec->ops = NULL;
  return ec;
}

expr_compiled* rtl_ops_of_expression(struct expression* e){
  return NULL;
}

list* rtl_ops_of_cfg_node(node_t* c){
  return NULL;
}

rtl* rtl_of_cfg_graph(cfg* c){
     return NULL;
}

rtl_prog* rtl_of_cfg_prog(cfg_prog* cfg){
  struct rtl_prog* rtl = (struct rtl_prog*)malloc(sizeof(struct rtl_prog));
  rtl->fname = strdup(cfg->fname);
  rtl->args = NULL;
  free_symtable(symt);
  return rtl;
}


void free_rtl_op(rtl_op* n){
  if(!n) return;
  free(n);
}

void free_rtl_ops(list* l){
  if(!l) return;
  free_rtl_ops(l->next);
  free_rtl_op(l->elt);
}

void free_rtl_graph(rtl* r){
  if(r){
    free_rtl_graph(r->next);
    free_rtl_ops(r->ops);
    //free_list(r->ops);
    free(r);
  }
}

void free_rtl(rtl_prog* rtl){
  if(rtl){
    free(rtl->fname);
    free_list(rtl->args);
    free_rtl_graph(rtl->graph);
    free(rtl);
  }
}
