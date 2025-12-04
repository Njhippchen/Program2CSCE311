/* Compile for rv64gc - position-independent simple function */
#include <stdint.h>
void main(void){
    const char *msg = "hello from user program\n";
    register uint64_t a0 asm("a0") = (uint64_t)msg;
    register uint64_t a7 asm("a7") = 1; /* write */
    asm volatile("ecall" :: "r"(a0), "r"(a7) : "memory");
    /* yield forever */
    while(1){
        asm volatile("li a7, 0; ecall");
    }
}
