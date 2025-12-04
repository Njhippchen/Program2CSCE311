#include "proc.h"
#include <string.h>
#include <stdlib.h>
#include "spinlock.h"
#include "console.h"

/* Simple global proc table and runqueue index */
proc_t proc_table[MAX_PROCS];
static int next_pid = 1;
static int cur = -1;
static spinlock_t proc_lock;

/* forward to context switch defined in context.S */
extern void context_switch(context_t *oldp, context_t *newp);

/* We'll implement a very small proc init/spawn/scheduler */
void proc_init(void){
    for(int i=0;i<MAX_PROCS;i++){
        proc_table[i].pid = 0;
        proc_table[i].state = UNUSED;
        proc_table[i].kstack = NULL;
    }
    spinlock_init(&proc_lock);
}

proc_t *current_proc(void){
    if(cur < 0) return NULL;
    return &proc_table[cur];
}

/* spawn: find slot, create kernel stack, setup context to start at user entry */
int proc_spawn(const char *name, void *entry, void *arg){
    spinlock_acquire(&proc_lock);
    int i;
    for(i=0;i<MAX_PROCS;i++){
        if(proc_table[i].state == UNUSED) break;
    }
    if(i==MAX_PROCS){ spinlock_release(&proc_lock); return -1; }
    proc_t *p = &proc_table[i];
    p->pid = next_pid++;
    p->state = READY;
    strncpy(p->name, name, 15);
    p->name[15]=0;
    p->kstack = malloc(PROC_STACK_SIZE);
    /* set kernel sp in context */
    memset(&p->ctx, 0, sizeof(context_t));
    p->ctx.sp = (uint64_t)p->kstack + PROC_STACK_SIZE - 16; /* kernel mode stack*/
    /* user sepc = entry point */
    p->sepc = (uint64_t)entry;
    p->sstatus = 0; /* simple */
    p->user_sp = (void *)((uint64_t)p->kstack + PROC_STACK_SIZE/2); /* arbitrary */
    spinlock_release(&proc_lock);
    return p->pid;
}

/* round-robin scheduler tick: called by timer interrupt to pick next process */
void scheduler_tick(void){
    spinlock_acquire(&proc_lock);
    int start = (cur+1) % MAX_PROCS;
    int i;
    for(i=0;i<MAX_PROCS;i++){
        int idx = (start + i) % MAX_PROCS;
        if(proc_table[idx].state == READY){
            /* switch from current to idx */
            int old = cur;
            cur = idx;
            proc_t *oldp = (old>=0) ? &proc_table[old] : NULL;
            proc_t *newp = &proc_table[cur];
            newp->state = RUNNING;
            if(oldp){
                oldp->state = READY;
                context_switch(&oldp->ctx, &newp->ctx);
            } else {
                /* first time: just restore newp context (simulate) */
                context_switch(NULL, &newp->ctx);
            }
            break;
        }
    }
    spinlock_release(&proc_lock);
}

void schedule(void){
    scheduler_tick();
}
