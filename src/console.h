#ifndef CONSOLE_H
#define CONSOLE_H

/* Basic console output functions for our RISC-V OS */

void kputchar(char c);
void kputs(const char *s);
void kprintf(const char *fmt, ...);

#endif
