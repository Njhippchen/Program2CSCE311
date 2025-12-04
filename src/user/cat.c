#include <stdint.h>
char buf[128];
void main(void){
    /* read file id 0 (not implemented) */
    register uint64_t a0 asm("a0") = 0; /* file id */
    register uint64_t a1 asm("a1") = (uint64_t)buf;
    register uint64_t a2 asm("a2") = sizeof(buf);
    register uint64_t a7 asm("a7") = 3; /* read file */
    asm volatile("ecall" :: "r"(a0), "r"(a1), "r"(a2), "r"(a7) : "memory");
    /* print */
    register uint64_t a0p asm("a0") = (uint64_t)buf;
    register uint64_t a7p asm("a7") = 1;
    asm volatile("ecall" :: "r"(a0p), "r"(a7p) : "memory");
    while(1) asm volatile("li a7,0; ecall");
}
