#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "lexer.h"
#include "parser.h"
#include "globals.h"
#include "datatypes.h"
#include "ast.h"
#include "tree_dump.h"
#include "elang_gen.h"
#include "elang_print.h"
#include "elang_run.h"
#include "cfg_gen.h"
#include "cfg_print.h"
#include "cfg_run.h"
#include "cfg_dead_assign_elimination.h"
#include "cfg_constant_propagation.h"
#include "rtl.h"
#include "rtl_gen.h"
#include "rtl_print.h"
#include "rtl_run.h"
#include "linear.h"
#include "linear_gen.h"
#include "linear_print.h"
#include "linear_run.h"
#include "linear_liveness.h"
#include "regalloc.h"
#include "x86gen.h"
#include "riscv_gen.h"

// -----------------------------------------------------------------
// ----------------------------- MAIN ------------------------------
// -----------------------------------------------------------------

// ------------------------- INITIALIZATION ------------------------


void print_usage() {
  printf("%s: usage: %s %s\n",
         myname, myname,
         "file.e\n\
\t[-ast file.dot]: dump AST in .dot file\n\
\t[-elang file]: dump E representation in file\n\
\t[-elang-run [param1 ... paramn]]: run E program on passed parameters\n\
\t[-cfg file.dot]: dump CFG representation in .dot file\n\
\t[-cfg-run [param1 ... paramn]]: run CFG representation on passed parameters\n\
\t[-rtl file.rtl]: dump RTL representation in text file, or on stdout if file.rtl is -\n\
\t[-rtl-run [param1 ... paramn]]: run RTL representation on passed parameters\n\
\t[-lin file.lin]: dump Linear representation in text file, or on stdout if file.lin is -\n\
\t[-lin-run [param1 ... paramn]]: run Linear representation on passed parameters\n\
\t[-target x86-64 | x86-32 | riscv]: specify processor target among x86-64 or x86-32 or RISCV-64 V\n\
\t[-naive-regalloc | -clever-regalloc n]: specify register allocation algorithm (naive is default)\n\
\t[-show-regalloc]: display the result of register allocation\n\
\t[-libdir path/]: specify the path to runtime library\n\
\t\t path is one of '<path>/runtime_riscv', '<path>/runtime_x86-32', '<path>/runtime_x86-64'\n\
\t[-s file.s]: dump textual representation of assembly produced in file.s, or on stdout if file.s is -\n\
\t[-o file]: specify the name of the ELF file to be produced.\n\
");
}


struct list* list_params(int argc, char** argv, unsigned int* curarg){
  if((*curarg) >= argc){
    return NULL;
  }
  char* arg = argv[*curarg];
  if(arg[0] == '-' && isalpha(arg[1])){
    return NULL;
  }
  (*curarg)++;
  return cons_int(atoi(arg), list_params(argc, argv, curarg));
}

void produce_x86asm(int sfile, linear_prog* rtl, int m64){
  FILE* sf = fdopen(sfile, "w");
  x86_of_lin_prog(sf, rtl, m64);
  fclose(sf);
  close(sfile);
}

void produce_riscv_asm(int sfile, linear_prog* rtl){
  FILE* sf = fdopen(sfile, "w");
  riscv_of_lin_prog(sf, rtl);
  fclose(sf);
  close(sfile);
}

FILE* get_file(char* filename){
  if(!strcmp(filename,"-")) return stdout;
  FILE* f = fopen(filename, "w");
  if(f) return f;
  printf("Error while opening file %s. Exiting.\n", filename);
  exit(1);
}

void close_file(FILE* f){
  if(f == stdout) return;
  fclose(f);
}

void compile_x86(linear_prog* lin, char* libdir, char* exe_file, int m64){
  char template[] = "trucXXXXXX.s";
  int sfile = mkstemps(template, 2);
  produce_x86asm(sfile, lin, m64);
  char *lib = NULL;
  int len = snprintf(lib, 0, "%s/main.c", libdir)+1;
  lib = malloc(len*sizeof(char));
  snprintf(lib, len, "%s/main.c", libdir);

  int status;
  int pid;
  if((pid = fork())){
    while(wait(&status) != pid);
    unlink(template);
    free(lib);
  } else {
    int res;
    char* argv[10];
    int i = 0;
    argv[i++] = "gcc";
    argv[i++] = template;
    argv[i++] = lib;
    argv[i++] = m64 ? "-m64" : "-m32";
    argv[i++] = m64 ? "-no-pie" : "-fno-pie";
    argv[i++] = "-fno-stack-protector";
    argv[i++] = "-o";
    argv[i++] = exe_file;
    argv[i++] = "-g";
    argv[i++] = NULL;
    res = execvp(*argv, argv);
    if( res == -1 && errno == ENOENT){
      fprintf(stderr, "Le compilateur gcc n'est pas installé\n");
      exit(-1);
    }
    else{
      perror("Erreur lors du lancement du compilateur gcc x86");
      exit(-1);
    }
  }
}

void compile_riscv(linear_prog* lin, char* libdir, char* exe_file){
  char template[] = "trucXXXXXX.s";
  int sfile = mkstemps(template, 2);
  produce_riscv_asm(sfile, lin);
  char *lib = NULL;
  int len = snprintf(lib, 0, "%s/main.c", libdir)+1;
  lib = malloc(len*sizeof(char));
  snprintf(lib, len, "%s/main.c", libdir);

  int status;
  int pid;
  if((pid = fork())){
    while(wait(&status) != pid);
    unlink(template);
    free(lib);
  } else {
    int res;
    char* argv[7];
    argv[0] = "riscv64-unknown-elf-gcc";
    argv[1] = template;
    argv[2] = lib;
    argv[3] = "-o";
    argv[4] = exe_file;
    argv[5] = "-g";
    argv[6] = NULL;
    res = execvp(*argv, argv);
    if( res == -1 && errno == ENOENT){
      fprintf(stderr, "Le cross-compilateur riscv64-unknown-elf-gcc n'est pas installé\n");
      exit(-1);
    }
    else{
      perror("Erreur lors du lancement du compilateur gcc x86");
      exit(-1);
    }
  }

}


int main(int argc, char** argv) {
  init_ast_tags();
  struct ast_node* ast = NULL;
  struct eprog* ep = NULL;
  struct cfg_prog* cfg = NULL;
  struct rtl_prog* rtl = NULL;
  struct linear_prog* lin = NULL;
  char* option;
  char* libdir = NULL;
  char* ast_file = NULL;
  char* elang_file = NULL;
  char* cfg_file = NULL;
  char* rtl_file = NULL;
  char* lin_file = NULL;
  char* s_file = NULL;
  char* exe_file = NULL;
  int m64 = 1;
  int elang_run = 0;
  list* elang_params = NULL;
  int cfg_run = 0;
  list* cfg_params = NULL;
  int rtl_run = 0;
  list* rtl_params = NULL;
  int lin_run = 0;
  list* lin_params = NULL;
  unsigned int curarg = 1;
  int
    do_ast = 0,
    do_e = 0,
    do_cfg = 0,
    do_rtl = 0,
    do_lin = 0,
    do_asm = 0;
  myname = argv[0];
  if (argc - curarg == 0){
    print_usage();
    exit(-1);
  }
  else {
    while (argc - curarg > 0) {
      option = argv[curarg++];

      if (!strcmp(option,  "-ast")){
        ast_file = argv[curarg++];
        do_ast = 1;
      }
      else if (!strcmp(option,  "-elang")){
        elang_file = argv[curarg++];
        do_e = 1;
      }
      else if (!strcmp(option,  "-cfg")){
        cfg_file = argv[curarg++];
        do_cfg = 1;
      }
      else if (!strcmp(option,  "-rtl")){
        rtl_file = argv[curarg++];
        do_rtl = 1;
      }
      else if (!strcmp(option,  "-lin")){
        lin_file = argv[curarg++];
        do_lin = 1;
      }
      else if (!strcmp(option,  "-libdir")){
        libdir = argv[curarg++];
      }
      else if (!strcmp(option, "-s")){
        s_file = argv[curarg++];
        do_asm = 1;
      }
      else if (!strcmp(option,  "-o")){
        if(!libdir){
          printf("No libdir specified, don't know where to find main.c!\n");
          exit(1);
        }
        do_asm = 1;
        exe_file = argv[curarg++];
      }
      else if (!strcmp(option, "-naive-regalloc")){
        clever_allocation = 0;
      }
      else if (!strcmp(option, "-clever-regalloc")){
        clever_allocation = 1;
        ncolors = atoi(argv[curarg++]);
      }
      else if (!strcmp(option, "-show-regalloc")){
        print_allocation_flag = 1;
      }
      else if (!strcmp(option,  "-elang-run")){
        elang_run = 1;
        elang_params = list_params(argc, argv, &curarg);
        do_e = 1;
      }
      else if (!strcmp(option,  "-cfg-run")){
        cfg_run = 1;
        cfg_params = list_params(argc, argv, &curarg);
        do_cfg = 1;
      }
      else if (!strcmp(option,  "-rtl-run")){
        rtl_run = 1;
        rtl_params = list_params(argc, argv, &curarg);
        do_rtl = 1;
      }
      else if (!strcmp(option,  "-lin-run")){
        lin_run = 1;
        lin_params = list_params(argc, argv, &curarg);
        do_lin = 1;
      }

      else if(!strcmp(option, "-target")){
        char* target = argv[curarg++];
        if(!strcmp(target, "riscv")){
          target_riscv = 1;
        } else if (!strcmp(target, "x86-32")){
          target_riscv = 0;
          m64 = 0;
        } else if (!strcmp(target, "x86-64")){
          target_riscv = 0;
          m64 = 1;
        } else {
          fprintf(stderr, "Unkown target '%s'. Aborting.\n", target);
          exit(1);
        }
      }

      else if(source_name == NULL){
        source_name = option;
      }

      else if (argc <= curarg) {
        // remaining options have at least one argument
        print_usage();

        printf("Unrecognized option '%s'.\n", option);
        return EXITCODE_BADARGUMENTS;
      }
      else {
        print_usage();

        printf("Unrecognized option '%s'.\n", option);
        return EXITCODE_BADARGUMENTS;
      }
    }

    if(do_asm) do_lin = 1;
    if(do_lin) do_rtl = 1;
    if(do_rtl) do_cfg = 1;
    if(do_cfg) do_e = 1;
    if(do_e) do_ast = 1;


    if (source_name){
      FILE* source_fd = fopen(source_name, "r");
      if (!source_fd) {
        printf("%s: could not open input file %s\n", myname, source_name);
        exit(EXITCODE_IOERROR);
      }
      if(do_ast) {
        ast = parse_file(source_fd);
      } else {
        show_lexemes(source_fd);
      }
      if(fclose(source_fd) < 0){
        printf("%s: could not close file descriptor %p\n", myname, source_fd);
        exit(EXITCODE_IOERROR);
      }
      if(do_e) ep = make_eprog(ast);
      if(do_cfg){
        cfg = cfg_of_e_prog(ep);
        cfg = dead_assign_elimination_prog(cfg);
        cfg = constant_propagation_cfg_prog(cfg);
      }
      if(do_rtl) rtl = rtl_of_cfg_prog(cfg);
      if(do_lin) lin = linearize_prog(rtl);
    }

    if(ast_file != NULL){
      FILE* fd = get_file(ast_file);
      draw_ast(fd, ast);
      fclose(fd);
    }

    if(elang_file != NULL){
      FILE* f = get_file(elang_file);
      print_eprog(f, ep);
      close_file(f);
    }
    if(cfg_file != NULL){
      FILE* f = get_file(cfg_file);
      print_cfg_prog(f, cfg);
      close_file(f);
    }
    if(rtl_file != NULL){
      FILE* f = get_file(rtl_file);
      print_rtl_prog(f, rtl);
      close_file(f);
    }
    if(lin_file != NULL){
      FILE* f = get_file(lin_file);
      print_linear_prog(f, lin);
      close_file(f);
    }
    if(s_file != NULL){
      FILE* f = get_file(s_file);
      if(target_riscv)
        produce_riscv_asm(fileno(f), lin);
      else
        produce_x86asm(fileno(f), lin, m64);
      close_file(f);
    }
    if(do_asm && exe_file != NULL && lin != NULL){
      if(target_riscv)
        compile_riscv(lin, libdir, exe_file);
      else
        compile_x86(lin, libdir, exe_file, m64);
    }

    if(elang_run){
      int res = run_eprog(ep, elang_params);
      printf("Result = %d\n", res);
      free_list(elang_params);
    }

    if(cfg_run){
      int res = run_cfg_prog(cfg, cfg_params);
      printf("Result = %d\n", res);
      free_list(cfg_params);
    }

    if(rtl_run){
      int res = run_rtl_prog(rtl, rtl_params);
      printf("Result = %d\n", res);
      free_list(rtl_params);
    }

    if(lin_run){
      int res = run_linear_prog(lin, lin_params);
      printf("Result = %d\n", res);
      free_list(lin_params);
    }

  }

  free_ast(ast);
  free_ast_tags();
  free_eprog(ep);
  free_cfg(cfg);
  free_rtl(rtl);
  free_lin(lin);
  return EXITCODE_NOERROR;
}
