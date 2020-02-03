#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "parser.h"
#include "lexer.h"
#include "ast.h"
#include "datatypes.h"

int graph_numnode = 0;

// tag_names is an association list used to remember the user-friendly names of
// nodes in the AST.

struct list* tag_names;

void register_tag_name(enum ast_tag i, char* s){
  char** ss = assoc_get(tag_names, (void*)i);
  if(ss){
    printf("Already registered string '%s' for tag number %d. (when trying %s)\n", *ss, i, s);
  }
  tag_names = assoc_set(tag_names, (void*)i, (void*)s);
}

char* get_tag_name(enum ast_tag i){
  char** s = (char**)assoc_get(tag_names, (void*)i);
  if(!s){
    printf("Couldn't find name associated with tag number %d.\n", i);
    exit(-1);
  }
  char* tn = *s;
  free(s);
  return tn;
}

/* dump_tree(tree,parent,arrowtext) :
   - tree : the tree to be dumped
   - parent : "option int" : the identifier of the parent node if any (some(n) or none)
   - arrowtext : the text to be printed on the arrow from the parent to the current node
 */

void dump_tree(FILE* fd, struct ast_node* tree, int* parent, char* arrowtext){
  int this_node = graph_numnode;
  graph_numnode = graph_numnode + 1;

  if(!tree){
    fprintf(fd, "n%d [label=\"null node\"]\n", this_node);
  }
  else{
    int type = tree->tag;
    if (type == AST_INT){
      fprintf(fd, "n%d [label=\"%d\"]\n", this_node, (int)(uintptr_t)tree->integer);
    } else if (type == AST_STRING){
      fprintf(fd, "n%d [label=\"%s\"]\n", this_node, (char*)tree->string);
    } else {
      fprintf(fd, "n%d [label=\"%s\"]\n", this_node, get_tag_name(tree->tag));
      list* children = tree->children;
      while(children){
        struct ast_node* child = (struct ast_node*)children->elt;
        int* p = some((void*)(unsigned long)this_node);
        dump_tree(fd, child, p, "");
        free(p);
        children = children->next;
      }
    } 
  }
  if(parent){
    fprintf(fd, "n%d -> n%d [label=\"%s\"]\n", *parent, this_node, arrowtext);
  }
  
}

void init_ast_tags(){
  tag_names = NULL;
  register_tag_name(AST_NODE_FUNDEF, "AST_NODE_FUNDEF");
  register_tag_name(AST_NODE_FUNARGS, "AST_NODE_FUNARGS");
  register_tag_name(AST_NODE_FUNBODY, "AST_NODE_FUNBODY");
  register_tag_name(AST_EQS, "AST_EQS");
  register_tag_name(AST_CMPS, "AST_CMPS");
  register_tag_name(AST_FACTORS, "AST_FACTORS");
  register_tag_name(AST_TERMS, "AST_TERMS");

  register_tag_name(AST_STRING, "AST_STRING");

  register_tag_name(AST_INT, "AST_INT");
  register_tag_name(AST_EADD, "AST_EADD");
  register_tag_name(AST_ESUB, "AST_ESUB");
  register_tag_name(AST_EMUL, "AST_EMUL");
  register_tag_name(AST_EDIV, "AST_EDIV");
  register_tag_name(AST_EMOD, "AST_EMOD");
  register_tag_name(AST_ENEG, "AST_ENEG");
  register_tag_name(AST_CLT, "AST_CLT");
  register_tag_name(AST_CLE, "AST_CLE");
  register_tag_name(AST_CGT, "AST_CGT");
  register_tag_name(AST_CGE, "AST_CGE");
  register_tag_name(AST_CEQ, "AST_CEQ");
  register_tag_name(AST_CNEQ, "AST_CNE");

  register_tag_name(AST_IASSIGN, "AST_IASSIGN");
  register_tag_name(AST_IIFTHENELSE, "AST_IIFTHENELSE");
  register_tag_name(AST_IWHILE, "AST_IWHILE");
  register_tag_name(AST_IRETURN, "AST_IRETURN");
  register_tag_name(AST_IPRINT, "AST_IPRINT");
  register_tag_name(AST_IBLOCK, "AST_IBLOCK");
}

void free_ast_tags(){
  list* n = tag_names;
  while(n){
    free(n->elt);
    n = n->next;
  }
  free_list(tag_names);
}

void draw_ast(FILE* fd, struct ast_node* ast){
  fprintf(fd, "digraph G {\n");
  dump_tree(fd,ast, NULL, "");
  fprintf(fd, "}\n");
}
