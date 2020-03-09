#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elang.h"
#include "cfg.h"


void print_mapping(FILE* f, list* map){
  while(map){
    pair* elt = map->elt;
    fprintf(f, "n%d: ", (int)(unsigned long)elt->fst);
    print_string_list(f, (list*)elt->snd);
    fprintf(f, "\n");
    map = map->next;
  }
}

void print_list(list* list){
  printf("print\n");
  while(list){
    printf("%s\n", (char*)list->elt);
    list = list->next;
  }
}

list* expr_used_vars(struct expression* e){
  struct list* used_var = NULL;
  switch(e->etype){
    case EUNOP: {
      printf("unop\n");
      return expr_used_vars(e->unop.e);

    }
    case EBINOP:
    {
      return concat(expr_used_vars(e->binop.e1), expr_used_vars(e->binop.e2));
      printf("binop\n");
    }
    case EINT:
      printf("int\n");
      return NULL;
    case EVAR:
      printf("var\n");
      return cons_string(e->var.s, NULL);
  }
  return used_var;
}


list* live_after(node_t* n, list* map){
  list* live_aft = NULL;
  struct list* successeurs = (struct list*)malloc(sizeof(struct list));
  successeurs = succs_node(n);
  while(successeurs){
    concat(live_aft, assoc_get(map, successeurs->elt));
    successeurs = successeurs->next;
  }
  return live_aft;
}

list* live_before(list* live_aft, node_t* n){
  list* list_bef = NULL;

  switch(n->type){
    case NODE_ASSIGN:
    {
      struct list* used_var = (struct list*)malloc(sizeof(struct list));
      used_var = expr_used_vars(n->assign.e);
      print_string_list(stdout, used_var);
      concat(list_bef, used_var);
    }
    case NODE_PRINT:
    {
      struct list* used_var = (struct list*)malloc(sizeof(struct list));
      used_var = expr_used_vars(n->print.e);
      concat(list_bef, used_var);
    }
    case NODE_RETURN:
    {
      struct list* used_var = (struct list*)malloc(sizeof(struct list));
      used_var = expr_used_vars(n->ret.e);
      concat(list_bef, used_var);
    }
    case NODE_COND:
    {
      struct list* used_var = (struct list*)malloc(sizeof(struct list));
      used_var = expr_used_vars(n->cond.cond);
      concat(list_bef, used_var);
    }
    case NODE_GOTO:
      return live_aft;
  }
  return list_bef;
}

int new_changes;

list* liveness_graph(list* map, cfg* c){
  new_changes = 0;
  while(c){
    struct list* live_aft = (struct list*)malloc(sizeof(struct list));
    live_aft = live_after(c->node, map);
    struct list* live_bef = (struct list*)malloc(sizeof(struct list));
    live_bef = live_before(live_aft, c->node);
    //print_list(live_bef);
    if(!list_string_incl(live_bef, assoc_get(map, &(c->id)))){
      new_changes = 1;
      assoc_set(map, &(c->id), live_bef);
    }

    c = c->next;
  }
  return map;
}

list* liveness_prog(cfg_prog* p){
  new_changes = 1;
  list* map = NULL;
  while(new_changes){
    map = liveness_graph(map, p->graph);
  }
  return map;
}