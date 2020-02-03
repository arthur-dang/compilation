#include <stdlib.h>
#include <assert.h>
#include "rtl.h"
#include "linear.h"
#include "regalloc.h"
#include "elang.h"

typedef enum x86reg
  {EAX = 0, EBX, ECX, EDX, ESI, EDI, EBP, ESP, R8, R9, R10, R11, R12, R13, R14, R15 }
  x86reg;

char* regs64[] = { "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp",
                 "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15" };

x86reg x86_64_allocable[] = { ESI, R8, R9, R10, R11, R12, R13, R14, R15 };

char* regs32[] = { "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp" };

x86reg x86_32_allocable[] = { ESI, EDI };

char** regs;
x86reg *x86_allocable;
int target_m64;
int wordsize;

void x86_fix_allocation(allocation* alloc){
  if(alloc){
    switch(alloc->loc.type){
    case REG:
      alloc->loc.reg = x86_allocable[alloc->loc.reg];
      break;
    case STACK:
    case PARAM:
      break;
    }
    x86_fix_allocation(alloc->next);
  }
}

void x86_loc(FILE* f, loc l){
  switch (l.type){
  case STACK:
    if(target_m64)
      fprintf(f, "QWORD");
    else
      fprintf(f, "DWORD");
    fprintf(f, " PTR [%s+%d]", regs[EBP], -wordsize*l.stkoffset);
    break;
  case PARAM:
    if(target_m64)
      fprintf(f, "QWORD");
    else
      fprintf(f, "DWORD");
    fprintf(f, " PTR [%s+%d]", regs[EBP], wordsize*(l.paramnum+2));
    break;
  case REG:
    fprintf(f, "%s", regs[l.reg]);
    break;
  default:
    printf("Unexpected loc type: %d\n", l.type);
    exit(1);
  }
}

void x86_alloc(allocation* alloc, FILE* f, int pseudoreg){
  loc* l = get_reg_loc(alloc, pseudoreg);
  if(!l){
    printf("Unable to allocate pseudo-register %d. Exiting.\n", pseudoreg);
    exit(1);
  }
  x86_loc(f, *l);
}


char* x86unop(unop_t u){
  switch(u){
  case ENEG: return "neg";
  }
  assert(0);
}

char* x86binop(enum binop_t b){
  switch(b){
  case EADD: return "add";
  case ESUB: return "sub";
  case EMUL: return "mul";
  case EDIV: return "div";
  case EMOD: return "mod";
  case CEQ: return "==";
  case CNEQ: return "!=";
  case CLT: return "<";
  case CGT: return ">";
  case CLE: return "<=";
  case CGE: return ">=";
  }
  assert(0);
}

void make_move(allocation* alloc, FILE* f, int pseudoregsrc, int pseudoregdst){
  fprintf(f, "mov %s,", regs[EBX]);
  x86_alloc(alloc, f, pseudoregsrc);
  fprintf(f, "\nmov ");
  x86_alloc(alloc, f, pseudoregdst);
  fprintf(f, ",%s\n", regs[EBX]);
}

void make_move_to_reg(allocation* alloc, FILE* f, int pseudoregsrc, int dst){
  fprintf(f, "mov %s,", regs[dst]);
  x86_alloc(alloc, f, pseudoregsrc);
  fprintf(f,"\n");
}


void x86_of_rtl_op(allocation* alloc, FILE* f, rtl_op* n){
  switch(n->type){
  case RLABEL:
    fprintf(f, ".n%d:\n", n->label.lab);
    break;
  case RMOV:
    make_move(alloc, f, n->mov.rs, n->mov.rd);
    break;
  case RRET:
    make_move_to_reg(alloc, f, n->ret.rs, EAX);
    fprintf(f, "jmp .ret\n");
    break;
  case RPRINT:
    if(target_m64){
      make_move_to_reg(alloc, f, n->print.rs, EDI);
      fprintf(f, "call print_long\n");
    } else {
      make_move_to_reg(alloc, f, n->print.rs, EAX);
      fprintf(f, "sub %s, 0xc\n", regs[ESP]);
      fprintf(f, "push %s\n", regs[EAX]);
      fprintf(f, "call print_int\n");
      fprintf(f, "add %s,0xc\n", regs[ESP]);
    }
    break;
  case RGOTO:
    fprintf(f, "jmp .n%d\n", n->rgoto.succ);
    break;
  case RBRANCH:
    make_move_to_reg(alloc, f, n->branch.rs, EAX);
    fprintf(f, "test %s, %s\n", regs[EAX], regs[EAX]);
    fprintf(f, "jnz .n%d\n", n->branch.succtrue);
    fprintf(f, "jmp .n%d\n", n->branch.succfalse);
    break;
  case RBINOP:
    if(n->binop.binop >= CEQ){
      fprintf(f, "xor %s, %s\n", regs[EBX], regs[EBX]);
      make_move_to_reg(alloc, f, n->binop.rs1, ECX);
      make_move_to_reg(alloc, f, n->binop.rs2, EDX);
      fprintf(f, "cmp %s, %s\n", regs[ECX], regs[EDX]);
      fprintf(f, "\n");
      switch(n->binop.binop){
      case CEQ: fprintf(f, "sete "); break;
      case CNEQ: fprintf(f, "setne "); break;
      case CLT: fprintf(f, "setl "); break;
      case CLE: fprintf(f, "setle "); break;
      case CGT: fprintf(f, "setg "); break;
      case CGE: fprintf(f, "setge "); break;
      default: break;
      }
      fprintf(f, "bl\nmov ");
      x86_alloc(alloc, f, n->binop.rd);
      fprintf(f, ",%s\n", regs[EBX]);
    } else if (n->binop.binop == EDIV){
      make_move_to_reg(alloc, f, n->binop.rs1, EAX);
      make_move_to_reg(alloc, f, n->binop.rs2, EBX);
      fprintf(f, "mov %s, 0\n", regs[EDX]);
      fprintf(f, "div %s\nmov ", regs[EBX]);
      x86_alloc(alloc, f, n->binop.rd);
      fprintf(f, ", %s\n", regs[EAX]);
    } else if (n->binop.binop == EMOD){
      make_move_to_reg(alloc, f, n->binop.rs1, EAX);
      make_move_to_reg(alloc, f, n->binop.rs2, EBX);
      fprintf(f, "mov %s, 0\n", regs[EDX]);
      fprintf(f, "div %s\nmov ", regs[EBX]);
      x86_alloc(alloc, f, n->binop.rd);
      fprintf(f, ", %s\n", regs[EDX]);
    } else if (n->binop.binop == EMUL){
      make_move_to_reg(alloc, f, n->binop.rs1, EAX);
      make_move_to_reg(alloc, f, n->binop.rs2, EBX);
      fprintf(f, "mul %s\n", regs[EBX]);
      fprintf(f, "mov ");
      x86_alloc(alloc, f, n->binop.rd);
      fprintf(f, ",%s\n", regs[EAX]);
    } else {
      make_move_to_reg(alloc, f, n->binop.rs1, EAX);
      make_move_to_reg(alloc, f, n->binop.rs2, EBX);
      fprintf(f, "%s %s, %s\n",x86binop(n->binop.binop), regs[EAX], regs[EBX]);
      fprintf(f, "mov ");
      x86_alloc(alloc, f, n->binop.rd);
      fprintf(f, ",%s\n", regs[EAX]);
    }
    break;
  case RUNOP:
    make_move_to_reg(alloc, f, n->unop.rs, EAX);
    fprintf(f, "%s %s\n",x86unop(n->unop.unop), regs[EAX]);
    fprintf(f, "mov ");
    x86_alloc(alloc, f, n->unop.rd);
    fprintf(f, ",%s\n", regs[EAX]);
    break;
  case RIMM:
    fprintf(f, "mov ");
    x86_alloc(alloc, f, n->imm.rd);
    fprintf(f, ",%d\n", n->imm.imm);
    break;
  }
}

void x86_of_rtl_ops(allocation* alloc, FILE* f, list* ns){
  if(!ns) return;
  x86_of_rtl_op(alloc, f, ns->elt);
  x86_of_rtl_ops(alloc, f, ns->next);
}

void x86_of_lin_prog(FILE* f, linear_prog* lin, int m64){
  if(!lin){
    fprintf(stderr, "x86gen: lin prog is NULL.\n");
    exit(-1);
  }
  target_m64 = m64;
  if(m64){
    regs = regs64;
    x86_allocable = x86_64_allocable;
    wordsize = 8;
  } else {
    regs = regs32;
    x86_allocable = x86_32_allocable;
    wordsize = 4;
  }
  int num;
  allocation* alloc = regalloc_prog(lin, &num);
  x86_fix_allocation(alloc);
  fprintf(f, ".intel_syntax noprefix\n");
  fprintf(f, ".global supecomp_main\n");
  fprintf(f, "supecomp_main:\n");
  fprintf(f, "push %s\n", regs[EBP]);
  fprintf(f, "mov %s, %s\n", regs[EBP], regs[ESP]);
  fprintf(f, "sub %s, %d\n", regs[ESP], num*wordsize);
  x86_of_rtl_ops(alloc, f, lin->ops);
  fprintf(f, ".ret: leave\n");
  fprintf(f, "ret\n");
  free_allocation(alloc);
}
