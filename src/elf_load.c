/* elf_load.c
   In this tiny kernel we do not implement a full ELF loader.
   The supported workflow is:
     - compile user programs as position-independent objects (-fPIC -nostdlib -c).
     - link them into the kernel binary (see Makefile) so we have symbol addresses at
       link time: e.g. _user_hello_start.
     - proc_spawn takes an entry pointer and creates a process whose sepc points to that entry.
   Future work: implement an ELF parser + copy text/data into a separate memory region and setup page tables.
*/
#include <stdint.h>
#include "console.h"

void load_user_program(void *entry){
    (void)entry;
    kprintf("Loading user program at %p\n", entry);
}
