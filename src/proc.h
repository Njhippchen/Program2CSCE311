#ifndef PROC_H
#define PROC_H
#include <stdint.h>

#define MAX_PROCS 8
#define PROC_STACK_SIZE 8192

typedef enum { UNUSED=0, READY, RUNNING, SLEEPING, ZOMBIE } proc_state;

typedef struct context {
    uint64_t ra, sp, gp, tp;
    uint64_t t0,t1,t2;
    uint64_t s0,s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11;
} context_t;

typedef struct proc {
    int pid;
    proc_state state;
    context_t ctx;
    void *kstack;         /* kernel stack area */
    uint64_t sepc;        /* saved user program counter */
    uint64_t sstatus;
    char name[16];
    /* simple simple-file descriptor: pointer into embedded file table */
    void *user_sp;        /* saved user stack pointer for return */
    /* next pointer for ready queue list */
    int exit_code;
} proc_t;

extern proc_t proc_table[MAX_PROCS];
void proc_init(void);
int proc_spawn(const char *name, void *entry, void *arg);
void scheduler_tick(void);
void schedule(void);
proc_t *current_proc(void);
#endif
