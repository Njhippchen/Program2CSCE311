#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

/* simple QEMU UART output: write char to MTIME? Actually we'll use SBI console_putc using ecall ABI:
   For clarity we will just write to SBI console by using the standard ecall number provided by OpenSBI:
   But for portability in our simple kernel, we'll call a low-level write via memory-mapped UART - but virt machine console is accessible via putchar to stdout if QEMU forwards semihosting...
   To keep things simple we use a small shim that uses SBI call 1 (console_putchar).
*/
static inline void sbi_console_putchar(int ch){
    register uint64_t a0 asm("a0") = ch;
    register uint64_t a7 asm("a7") = 1; /* sbi_console_putc (SBI call 1) */
    asm volatile("ecall" : "+r"(a0) : "r"(a7));
}

void kputchar(char c){ sbi_console_putchar(c); }

void kputs(const char *s){
    while(*s) kputchar(*s++);
}

#include <stdio.h>
void kprintf(const char *fmt, ...){
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    kputs(buf);
}
