/* For simplicity: our "user programs" are simple position-independent functions compiled
   to objects and we provide their entry addresses at link time. This loader is minimal. */
#include <stdint.h>
#include "console.h"

void load_user_program(void *entry){
    /* This is a stub: in this simple kernel we treat entry as a pointer and create a proc */
    (void)entry;
    kprintf("Loading user program at %p\n", entry);
}
