#include "elang.h"
#include "elang_gen.h"
#include "cfg.h"
#include <stdlib.h>
#include <string.h>
#include "datatypes.h"


void free_node(node_t* n){
  if(!n) return;
  switch(n->type){
  case NODE_ASSIGN:
    free(n->assign.var);
    free_expression(n->assign.e);
    break;
  case NODE_PRINT:
    free_expression(n->print.e);
    break;
  case NODE_RETURN:
    free_expression(n->ret.e);
    break;
  case NODE_COND:
    free_expression(n->cond.cond);
    break;
  default:
    break;
  }
  free(n);
}

void free_graph(cfg* c){
  if(c){
    free_graph(c->next);
    free_node(c->node);
    free(c);
  }
}

void print_graph(cfg* c){
  if(c)
    print_graph(c->next);
}

void free_cfg(cfg_prog* cfg){
  if(cfg){
    free(cfg->fname);
    list* largs = cfg->args;
    while(largs){
      free(largs->elt);
      largs = largs->next;
    }
    free_list(cfg->args);
    free_graph(cfg->graph);
    free(cfg);
  }
}


node_t* new_cfg_node(){
  node_t* n = (node_t*)malloc(sizeof(node_t));
  return n;
}

int curid = 0;

int newid(){
  return curid++;
}

cfg* new_cfg(){
  cfg* c = (cfg*)malloc(sizeof(cfg));
  c->next = NULL;
  c->node = NULL;
  return c;
}

int add_node(cfg** c, int id, node_t* n){
  cfg* new = new_cfg();
  new->next = *c;
  new->id = id;
  new->node = n;
  *c = new;
  return id;
}

int add_instr(cfg** c, struct instruction* i, int succ);

int add_instrs(cfg** c, list* n, int succ){
  if(!n) return succ;
  succ = add_instrs(c, n->next, succ);
  return add_instr(c, n->elt, succ);
}

int add_instr(cfg** c, struct instruction* i, int succ){
  switch(i->type){
  case IBLOCK:
    {
      return add_instrs(c, i->iblock.l, succ);
    }
    break;
  case IWHILE:
    {
      int id = newid();
      node_t* n = new_cfg_node();
      n->type = NODE_COND;
      n->cond.cond = copy_expression(i->iwhile.cmp);
      n->cond.succ1 = add_instr(c, i->iwhile.i, id);
      n->cond.succ2 = succ;
      return add_node(c, id, n);
    }
    break;
  case IIFTHENELSE:
    {
      node_t* n = new_cfg_node();
      n->type = NODE_COND;
      n->cond.cond = copy_expression(i->iif.cmp);
      n->cond.succ1 = add_instr(c, i->iif.ithen, succ);
      n->cond.succ2 = add_instr(c, i->iif.ielse, succ);
      return add_node(c, newid(), n);
    }
    break;
  case IASSIGN:
    {
      node_t* n = new_cfg_node();
      n->type = NODE_ASSIGN;
      n->assign.var = strdup(i->iassign.var);
      n->assign.e = copy_expression(i->iassign.e);
      n->assign.succ = succ;
      return add_node(c, newid(), n);
    }
    break;
  case IRETURN:
    {
      node_t* n = new_cfg_node();
      n->type = NODE_RETURN;
      n->ret.e = copy_expression(i->ireturn.e);
      return add_node(c, newid(), n);
    }
    break;
  case IPRINT:
    {
      node_t* n = new_cfg_node();
      n->type = NODE_PRINT;
      n->print.e = copy_expression(i->iprint.e);
      n->print.succ = succ;
      return add_node(c, newid(), n);
    }
    break;
  default:
    printf("In add_instr, unexpected instruction type: %d\n", i->type);
    exit(1);
  }
}

list* succs_node(node_t* n){
  switch(n->type){
  case NODE_ASSIGN: return cons_int(n->assign.succ, NULL);
  case NODE_PRINT: return cons_int(n->print.succ, NULL);
  case NODE_RETURN: return NULL;
  case NODE_COND: return cons_int(n->cond.succ1, cons_int(n->cond.succ2, NULL));
  case NODE_GOTO: return cons_int(n->goto_succ, NULL);
  }
  return NULL;
}

list* used_succs(cfg *c){
  if(!c) return NULL;
  return concat(succs_node(c->node), used_succs(c->next));
}

cfg* do_clean_cfg(cfg* c, list* donotremove){
  if(!c) return NULL;
  if(list_in_int(donotremove, c->id)){
    c->next = do_clean_cfg(c->next, donotremove);
    return c;
  } else {
    cfg* nc = do_clean_cfg(c->next, donotremove);
    free(c->node);
    free(c);
    return nc;
  }
}

cfg* clean_cfg(cfg* c, int entry){
  list* l = cons_int(entry,used_succs(c));
  c = do_clean_cfg(c, l);
  free_list(l);
  return c;
}

struct cfg_prog* cfg_of_e_prog(struct eprog* e){
  struct cfg_prog* cfg = (struct cfg_prog*)malloc(sizeof(struct cfg_prog));
  cfg->fname = strdup(e->fname);
  cfg->args = string_list_copy(e->args);
  cfg->graph = NULL;
  node_t* exit_node = new_cfg_node();
  exit_node->type = NODE_RETURN;
  exit_node->ret.e = make_expr_int(0);
  int succ = add_node(&cfg->graph, newid(), exit_node);
  cfg->entry = add_instr(&cfg->graph, e->body, succ);
  cfg->graph = clean_cfg(cfg->graph, cfg->entry);
  return cfg;
}
