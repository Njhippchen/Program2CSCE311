#include <stdint.h>
#include <string.h>
#include "console.h"
#include "proc.h"
#include "spinlock.h"

/* Declarations from other files */
extern void kernel_trap_entry(void);
extern void user_trap_entry(void);
extern void timer_init(void);
extern void handle_syscall(uint64_t,uint64_t,uint64_t,uint64_t);

/* Embedded user program entry symbols - provided by link step */
extern char _user_hello_start, _user_hello_end;
extern char _user_cat_start, _user_cat_end;

/* File table: tiny embedded files */
extern char _file_hello_txt_start[];
void *file_table[] = { (void*)0, /* placeholder id 0 */ };
int file_sizes[] = { 0 };

proc_t proc_table[MAX_PROCS];

void trap_handler(uint64_t *sptr){
    /* naive trap handler called from assembly; read scause/sepc, etc. */
    uint64_t scause;
    asm volatile("csrr %0, scause" : "=r"(scause));
    if(scause == 0x8000000000000001ULL){ /* supervisor timer irq ? */
        /* call scheduler tick */
        scheduler_tick();
        /* set next timer - use SBI set_timer via ecall? for simplicity just return */
    } else if (scause == 8){ /* environment call from U-mode (ecall) */
        uint64_t a7,a0,a1,a2;
        asm volatile("mv %0, a7" : "=r"(a7));
        asm volatile("mv %0, a0" : "=r"(a0));
        asm volatile("mv %0, a1" : "=r"(a1));
        asm volatile("mv %0, a2" : "=r"(a2));
        handle_syscall(a7,a0,a1,a2);
    } else {
        kprintf("trap scause=%llx\n", (unsigned long long)scause);
    }
    /* return value for sepc: we simply return current sepc to continue */
    uint64_t sepc;
    asm volatile("csrr %0, sepc" : "=r"(sepc));
    /* advance for ecall if needed */
    sepc += 4;
    /* return sepc in a0 (trap.S expects it) */
    asm volatile("mv a0, %0" :: "r"(sepc));
}

void kernel_main(void){
    /* initialize console and proc table */
    kprintf("mini-rvOS booting...\n");
    proc_init();
    /* spawn embedded user programs by symbol addresses (simple) */
    void *hello_entry = (void*)&_user_hello_start;
    void *cat_entry = (void*)&_user_cat_start;
    proc_spawn("hello", hello_entry, NULL);
    proc_spawn("cat", cat_entry, NULL);
    /* Setup timer interrupts using SBI set_timer: not implemented here, QEMU triggers not necessary */
    /* start scheduler loop */
    while(1){
        schedule();
    }
}
