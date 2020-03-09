#include "datatypes.h"
#include "elang.h"
#include "elang_print.h"
#include "cfg.h"
#include "cfg_liveness.h"
#include "cfg_gen.h"
#include "globals.h"

int num_changed = 0;
int roundnr = 0;
int dead_code_detected;

//void print_mapping_2(list* map){
//  while(map){
//    pair* elt = map->elt;
//    fprintf("n%d: ", (int)(unsigned long)elt->fst);
////    print_string_list(f(list*)elt->snd);
//    fprintf("\n");
//    map = map->next;
//  }
//}

void dead_assign_elimination_graph(list* live, cfg* c){
//  print_mapping_2(live);

  dead_code_detected = 0;
  struct list* assign_var = (struct list*)malloc(sizeof(struct list));
  while(c){
    if(c->node->type == NODE_ASSIGN){
      assign_var->elt = c->node->assign.var;
      assign_var->next = NULL;
      if(!list_string_incl(assign_var, live_after(c->node, live))){
        // if the assigned variable is dead after the assign node
        dead_code_detected = 1;
        c->node->type = NODE_GOTO;
        c->node->goto_succ = c->node->assign.succ;
      }
    }
    c = c->next;
  }
}

cfg_prog* dead_assign_elimination_prog(cfg_prog* c){
  dead_code_detected = 1;

  struct list* live = (struct list*)malloc(sizeof(struct list));
  live = liveness_prog(c);
  while(dead_code_detected){
    dead_assign_elimination_graph(live, c->graph);
  }
  return c;
}
