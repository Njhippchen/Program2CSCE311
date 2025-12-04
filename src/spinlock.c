#include "spinlock.h"
static inline int xchg(volatile int *ptr, int val){
    int old;
    asm volatile("amoswap.w.aq %0, %1, %2"
        : "=r"(old) : "r"(val), "m"(*ptr) : "memory");
    return old;
}
void spinlock_init(spinlock_t *lk){ lk->locked = 0; }
void spinlock_acquire(spinlock_t *lk){
    while(xchg(&lk->locked, 1) != 0) { asm volatile("wfi"); }
}
void spinlock_release(spinlock_t *lk){ lk->locked = 0; }
