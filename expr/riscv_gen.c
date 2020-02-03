#include <stdlib.h>
#include <assert.h>
#include "rtl.h"
#include "linear.h"
#include "regalloc.h"
#include "elang.h"

typedef enum riscvreg { X0, RA, SP, GP, TP, T0, T1, T2, S0, S1, A0, A1, A2, A3, A4, A5, A6, A7,
                        S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, T3, T4, T5, T6 } riscvreg;

char* riscv_regs[] = { "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0", "s1",
                 "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
                 "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9",
                 "s10", "s11", "t3", "t4", "t5", "t6"};

enum riscvreg riscv_allocable[] = { A1, A2, A3, A4, A5, A6, A7, S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11, T3, T4, T5, T6 };

void riscv_fix_allocation(allocation* alloc){
  if(alloc){
    switch(alloc->loc.type){
    case REG:
      alloc->loc.reg = riscv_allocable[alloc->loc.reg];
      break;
    case STACK:
    case PARAM:
      break;
    }
    riscv_fix_allocation(alloc->next);
  }
}

void riscv_loc(FILE* f, loc l){
  switch (l.type){
  case STACK:
    fprintf(f, "%d(s0)", -8*l.stkoffset);
    break;
  case PARAM:
    fprintf(f, "%d(s0)", 8*(l.paramnum+2));
    break;
  case REG:
    fprintf(f, "%s", riscv_regs[l.reg]);
    break;
  default:
    fprintf(stderr, "Unexpected loc type: %d\n", l.type);
    exit(1);
  }
}

loc* get_loc(allocation* alloc, int pseudoreg){
  loc* l = get_reg_loc(alloc, pseudoreg);
  if(!l){
    printf("Unable to allocate pseudo-register %d. Exiting.\n", pseudoreg);
    exit(1);
  }
  return l;
}

void make_ld(FILE* f, riscvreg reg, loc* l){
  fprintf(f, "ld %s, ", riscv_regs[reg]);
  riscv_loc(f, *l);
  fprintf(f, "\n");
}

void make_sd(FILE* f, riscvreg reg, loc* l){
  fprintf(f, "sd %s, ", riscv_regs[reg]);
  riscv_loc(f, *l);
  fprintf(f, "\n");
}

void make_mv(FILE* f, riscvreg rd, riscvreg rs){
  fprintf(f, "mv %s, %s\n", riscv_regs[rd], riscv_regs[rs]);
}

void riscv_make_move(allocation* alloc, FILE* f, int pseudoregsrc, int pseudoregdst){
}

void riscv_make_move_to_reg(allocation* alloc, FILE* f, int pseudoregsrc, riscvreg dst){
}

riscvreg optional_read_reg(allocation* alloc, FILE* f, int pseudoreg, riscvreg temp){
  loc* l = get_loc(alloc, pseudoreg);
  riscvreg r = temp;
  switch(l->type){
  case REG:
    r = l->reg;
    break;
  case STACK:
  case PARAM:
    make_ld(f, temp, l);
    break;
  }
  return r;
}

riscvreg optional_write_reg(allocation* alloc, FILE* f, int pseudoreg, riscvreg temp){
  loc* l = get_loc(alloc, pseudoreg);
  riscvreg r = temp;
  switch(l->type){
  case REG:
    r = l->reg;
    break;
  case STACK:
  case PARAM:
    if(f)
      make_sd(f, temp, l);
    break;
  }
  return r;
}

void make_unop(FILE* f, allocation* alloc, char* op, int pseudord, int pseudors){
  riscvreg rs = optional_read_reg(alloc, f, pseudors, T0);
  riscvreg rd = optional_write_reg(alloc, NULL, pseudord, T0);
  fprintf(f, "%s %s, %s\n", op, riscv_regs[rd], riscv_regs[rs]);
  optional_write_reg(alloc, f, pseudord, T0);
}

void make_binop(FILE* f, allocation* alloc, char* op, int pseudord, int pseudors1, int pseudors2){
  riscvreg rs1 = optional_read_reg(alloc, f, pseudors1, T0);
  riscvreg rs2 = optional_read_reg(alloc, f, pseudors2, T1);
  riscvreg rd = optional_write_reg(alloc, NULL, pseudord, T0);
  fprintf(f, "%s %s, %s, %s\n", op, riscv_regs[rd], riscv_regs[rs1], riscv_regs[rs2]);
  optional_write_reg(alloc, f, pseudord, T0);
}

void riscv_of_rtl_op(allocation* alloc, FILE* f, rtl_op* n){
}

void riscv_of_rtl_ops(allocation* alloc, FILE* f, list* ns){
  if(!ns) return;
  riscv_of_rtl_op(alloc, f, ns->elt);
  riscv_of_rtl_ops(alloc, f, ns->next);
}

void riscv_of_lin_prog(FILE* f, linear_prog* lin){
  if(!lin){
    fprintf(stderr, "riscvgen: lin prog is NULL.\n");
    exit(-1);
  }
  int num;
  allocation* alloc = regalloc_prog(lin, &num);
  riscv_fix_allocation(alloc);
  fprintf(f, ".globl supecomp_main\n");
  fprintf(f, "supecomp_main:\n");
  fprintf(f, "addi sp, sp, -8\n");
  fprintf(f, "sd ra, 0(sp)\n");
  fprintf(f, "addi sp, sp, -8\n");
  fprintf(f, "sd s0, 0(sp)\n");
  fprintf(f, "mv s0, sp\n");
  fprintf(f, "addi sp, sp, -%d\n", num*8);
  riscv_of_rtl_ops(alloc, f, lin->ops);
  fprintf(f, ".ret: \n");
  fprintf(f, "mv sp, s0\n");
  fprintf(f, "ld s0, 0(sp)\n");
  fprintf(f, "addi sp, sp, 8\n");
  fprintf(f, "ld ra, 0(sp)\n");
  fprintf(f, "addi sp, sp, 8\n");
  fprintf(f, "ret\n");
  free_allocation(alloc);
}
