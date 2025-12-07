#include <stdint.h>
#include <string.h>
#include "console.h"
#include "proc.h"
#include "spinlock.h"
#include "fs.h"

/* Declarations from other files */
extern void kernel_trap_entry(void);
extern void user_trap_entry(void);
extern void timer_init(void);
extern void handle_syscall(uint64_t,uint64_t,uint64_t,uint64_t);

/* Embedded user program entry symbols - provided by link step */
extern char _user_hello_start, _user_hello_end;
extern char _user_cat_start, _user_cat_end;

/* Example file data embedded by providing symbols (if any) */
extern char _file_hello_txt_start[]; /* you can create embedded data if you wish */

proc_t proc_table[MAX_PROCS];

void trap_handler(uint64_t *sptr){
    uint64_t scause;
    asm volatile("csrr %0, scause" : "=r"(scause));
    if(scause == 0x8000000000000001ULL){ /* supervisor timer irq */
        scheduler_tick();
    } else if (scause == 8){ /* ecall from U-mode */
        uint64_t a7,a0,a1,a2;
        asm volatile("mv %0, a7" : "=r"(a7));
        asm volatile("mv %0, a0" : "=r"(a0));
        asm volatile("mv %0, a1" : "=r"(a1));
        asm volatile("mv %0, a2" : "=r"(a2));
        extern void handle_syscall(uint64_t,uint64_t,uint64_t,uint64_t);
        handle_syscall(a7,a0,a1,a2);
    } else {
        kprintf("trap scause=%llx\n", (unsigned long long)scause);
    }
    uint64_t sepc;
    asm volatile("csrr %0, sepc" : "=r"(sepc));
    sepc += 4; /* advance past ecall */
    asm volatile("mv a0, %0" :: "r"(sepc));
}

void kernel_main(void){
    kprintf("mini-rvOS booting...\n");
    proc_init();

    /* register embedded user objects as files (optional) */
    /* Example: if you created an embedded file region you can register it:
       fs_register(_file_hello_txt_start, (int)(&_file_hello_txt_end - _file_hello_txt_start));
       For now register nothing; user programs can still be linked in and spawned.
    */

    /* spawn embedded user programs by symbol addresses (simple) */
    void *hello_entry = (void*)&_user_hello_start;
    void *cat_entry = (void*)&_user_cat_start;
    proc_spawn("hello", hello_entry, NULL);
    proc_spawn("cat", cat_entry, NULL);

    /* start scheduler loop */
    while(1){
        schedule();
    }
}
