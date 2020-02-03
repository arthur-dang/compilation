#include <stdlib.h>
#include <string.h>
#include "datatypes.h"
#include "rtl.h"
#include "rtl_gen.h"
#include "linear.h"

list* jmps(list* ops){
  list* res = NULL;
  while(ops){
    rtl_op* op = ops->elt;
    switch(op->type){
    case RBRANCH:
      res = cons_int(op->branch.succtrue, cons_int(op->branch.succfalse, res));
      break;
    case RGOTO:
      res = cons_int(op->rgoto.succ, res);
      break;
    default:
      break;
    }
    ops = ops->next;
  }
  return res;
}


list* find_rtl_node(rtl* r, int id){
  while(r){
    if(r->id == id) return r->ops;
    r = r->next;
  }
  return NULL;
}


linear_prog* linearize_prog(rtl_prog* rtl){
  linear_prog* p = malloc(sizeof(linear_prog));
  p->fname = strdup(rtl->fname);
  p->args = int_list_copy(rtl->args);
  p->ops = NULL;
  list* visited = NULL;
  list* worklist = cons_int(rtl->entry, NULL);

  while(worklist){
    int rtl_node = (int)(unsigned long)worklist->elt;
    worklist = worklist->next;
    visited = cons_int(rtl_node, visited);
    list* ops = find_rtl_node(rtl->graph, rtl_node);
    p->ops = concat(p->ops, cons(new_rtl_op_label(rtl_node), int_list_copy(ops)));
    list* succs = jmps(ops);
    while(succs){
      if(!list_in_int(visited, (unsigned long)succs->elt)){
        worklist = cons(succs->elt, worklist);
      }
      succs = succs->next;
    }
  }

  return p;
}

void free_lin(linear_prog* lin){
  if(lin){
    free(lin->fname);
    free_list(lin->args);
    free_rtl_ops(lin->ops);
    free_list(lin->ops);
    free(lin);
  }
}
