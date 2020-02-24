#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "ast.h"
#include "tree_dump.h"
#include "elang.h"
#include "state.h"

int run_unop(enum unop_t u, int e){
  if(u == ENEG){
    return -e;
  }
  assert(0);
}

int run_binop(enum binop_t b, int e1, int e2){
  switch(b){
    case EADD:
      return e1+e2;
    case ESUB:
      return e1-e2;
    case EMUL:
      return e1*e2;
    case EDIV:
      return e1/e2;
    case EMOD:
      return e1%e2;
    case CEQ:
      return e1 == e2;
    case CNEQ:
      return e1 != e2;
    case CLT:
      return e1 < e2;
    case CLE:
      return e1 <= e2;
    case CGT:
      return e1 > e2;
    case CGE:
      return e1 >= e2;
  }
  assert(0);
}

int run_expression(string_int_state_t* s, struct expression* e){
//  printf("run_expression: unexpected expression type: %d", e->etype);
  switch(e->etype){
    case EINT: return e->eint.i;
    case EVAR: return string_int_get_val(s, e->var.s);
    case EUNOP: return run_unop(e->unop.unop, run_expression(s, e->unop.e));
    case EBINOP: return run_binop(e->binop.binop, run_expression(s, e->binop.e1), run_expression(s, e->binop.e2));
  }
  exit(1);
}

int* run_instruction(string_int_state_t** s, struct instruction* i){
//  printf("run_instruction: unexpected instruction type: %d", i->type);
  switch(i->type){
    case IIFTHENELSE:
      if(run_expression(*s, i->iif.cmp)){
        return run_instruction(s, i->iif.ithen);
      } else {
        return run_instruction(s, i->iif.ielse);
      }
    case IWHILE:
      while(run_expression(*s, i->iwhile.cmp)){
        void* res =  run_instruction(s, i->iwhile.i);
        if(res){
          return res;
        };
      }
      return NULL;
    case IASSIGN:
      *s = string_int_set_val(*s, i->iassign.var, run_expression(*s, i->iassign.e));
      return NULL;
    case IRETURN:
      return some((void*)(unsigned long)run_expression(*s, i->ireturn.e));
    case IPRINT:
      printf("%d\n", run_expression(*s, i->iprint.e));
      return NULL;
    case IBLOCK:
      {
        list* ll = i->iblock.l;
        while(ll){
          void* res = run_instruction(s, ll->elt);
          if(res){
            return res;
          };
          ll = ll->next;
        }
        return NULL;
      }
  }
  exit(1);
}

int run_eprog(struct eprog* p, struct list* args){
  if(!p){
    printf("run_eprog: NULL eprog\n");
    exit(1);
  }
  string_int_state_t* s = NULL;

  list* varNames = p->args;
  list* varValues = args;
  while(varNames){
    if(varValues == NULL ){
      printf("Not enough given arguments\n");
      exit(1);
    }
    s = string_int_set_val(s, varNames->elt, (int)(unsigned long) varValues->elt);
    varNames = varNames->next;
    varValues = varValues->next;
  }
  // Appel de run_instruction et retour de la valeur de retour
//  print_string_int_state(s);
  void* res = run_instruction(&s, p->body);
  terminate_string_int_state(s);
  if(res){
    return *((int *)res);
  } else {
    exit(-1);
  }
  return 0;
}