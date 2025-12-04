/* src/syscall.c */
#include <stdint.h>
#include <string.h>    /* <- added for memcpy */
#include "console.h"
#include "proc.h"

/* Very small syscall interface:
   a7 = syscall number
   a0,a1,a2 = args
   numbers:
     0 = yield
     1 = write (fd=1) -> a0 = ptr string
     2 = spawn (a0 = entry)
     3 = read_file (a0=fileid, a1=buf, a2=len)
     4 = exit (a0=code)
*/

/* Simple file table: declared in kernel.c as extern */
extern void* file_table[];
extern int file_sizes[];

void handle_syscall(uint64_t scno, uint64_t arg0, uint64_t arg1, uint64_t arg2){
    switch(scno){
        case 0: /* yield */
            schedule();
            break;
        case 1: /* write */
            kputs((const char*)arg0);
            break;
        case 2: /* spawn */
            proc_spawn("userapp", (void*)arg0, (void*)arg1);
            break;
        case 3: /* read file id */
            {
                int id = (int)arg0;
                if(id < 0) break;
                int n = file_sizes[id];
                if(arg2 < (uint64_t)n) n = arg2;
                memcpy((void*)arg1, file_table[id], n);
            }
            break;
        case 4:
            /* mark current proc as ZOMBIE */
            {
                proc_t *p = current_proc();
                if(p) { p->state = ZOMBIE; p->exit_code = arg0; }
                schedule();
            }
            break;
        default:
            kprintf("unknown syscall %d\n", (int)scno);
    }
}
