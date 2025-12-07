/* src/syscall.c */
#include <stdint.h>
#include <string.h>    /* memcpy */
#include "console.h"
#include "proc.h"
#include "fs.h"
#include "semaphore.h"

/* Very small syscall interface:
   a7 = syscall number
   a0,a1,a2 = args
   numbers:
     0 = yield
     1 = write (fd=1) -> a0 = ptr string
     2 = spawn (a0 = entry, a1 = arg)
     3 = read_file (a0=fileid, a1=buf, a2=len)
     4 = exit (a0=code)
     5 = sem_wait (a0 = pointer)
     6 = sem_post (a0 = pointer)
*/

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
                int n = fs_get_size(id);
                if(arg2 < (uint64_t)n) n = arg2;
                const void *p = fs_get_ptr(id);
                if(p && arg1) memcpy((void*)arg1, p, n);
            }
            break;
        case 4:
            {
                proc_t *p = current_proc();
                if(p) { p->state = ZOMBIE; p->exit_code = arg0; }
                schedule();
            }
            break;
        case 5:
            if(arg0) sem_wait((sem_t*)arg0);
            break;
        case 6:
            if(arg0) sem_post((sem_t*)arg0);
            break;
        default:
            kprintf("unknown syscall %d\n", (int)scno);
    }
}
