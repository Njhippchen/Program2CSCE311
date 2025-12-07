#ifndef SEMAPHORE_H
#define SEMAPHORE_H
#include <stdint.h>

/* small counting semaphore built on spinlock + simple wait queue */
typedef struct {
    volatile int value;
    /* For simplicity we use a busy-waiting loop with wfi; not a full wait-queue */
    // Future: add real wait-queue list of pids
} sem_t;

void sem_init(sem_t *s, int val);
void sem_wait(sem_t *s);
void sem_post(sem_t *s);

#endif
