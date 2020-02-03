#include <stdlib.h>
#include <stdio.h>
#include "state.h"
#include "rtl.h"
#include "datatypes.h"
#include "elang_run.h"

int* run_rtl_at(int_int_state_t** s, rtl* r, int n);

int* run_rtl_op(int_int_state_t** s, rtl* r, rtl_op* n){
  switch(n->type){
  case RLABEL:
    break;
  case RMOV:
    *s = int_int_set_val(*s, n->mov.rd, int_int_get_val(*s, n->mov.rs));
    break;
  case RIMM:
    *s = int_int_set_val(*s, n->imm.rd, n->imm.imm);
    break;
  case RRET:
    return some((void*)(unsigned long)int_int_get_val(*s, n->ret.rs));
    break;
  case RPRINT:
    printf("%d\n", int_int_get_val(*s, n->print.rs));
    break;
  case RBRANCH:
    if(int_int_get_val(*s, n->branch.rs))
      return run_rtl_at(s, r, n->branch.succtrue);
    return run_rtl_at(s, r, n->branch.succfalse);
    break;
  case RGOTO:
    return run_rtl_at(s, r, n->rgoto.succ);
  case RBINOP:
    {
      int e1 = int_int_get_val(*s, n->binop.rs1);
      int e2 = int_int_get_val(*s, n->binop.rs2);
      *s = int_int_set_val(*s, n->binop.rd, run_binop(n->binop.binop, e1, e2));
    }
    break;
  case RUNOP:
    {
      int e1 = int_int_get_val(*s, n->unop.rs);
      *s = int_int_set_val(*s, n->unop.rd, run_unop(n->unop.unop, e1));
    }
    break;
  }
  return NULL;
}

int* run_rtl_ops(int_int_state_t** s, rtl* r, list* ns){
  if (ns){
    int* res = run_rtl_op(s, r, ns->elt);
    if(res) return res;
    return run_rtl_ops(s, r, ns->next);
  }
  return NULL;
}

int* run_rtl_at(int_int_state_t** s, rtl* r, int id){
  rtl* rsave = r;
  while(r){
    if(r->id == id) return run_rtl_ops(s, rsave, r->ops);
    r = r->next;
  }
  return NULL;
}


int run_rtl_prog(rtl_prog* p, struct list* args){
  if(!p){
    printf("run_rtl_prog: NULL rtlprog\n");
    exit(1);
  }
  int_int_state_t * s = NULL;
  list* lparams = p->args;
  list* largs = args;
  while(lparams){
    if(!largs){
      printf("Not enough arguments provided (%d required, %d provided)\n", list_length(p->args), list_length(args));
      exit(1);
    }
    int v = (unsigned long)largs->elt;
    s = int_int_set_val(s, (unsigned long)lparams->elt, v);
    largs = largs->next;
    lparams = lparams->next;
  }

  int* res = run_rtl_at(&s, p->graph, p->entry);
  terminate_int_int_state(s);
  int vres = *res;
  free(res);
  return vres;
}
