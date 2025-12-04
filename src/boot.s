/* Minimal boot: set stack, call kernel_main */
    .section .text
    .globl _start
_start:
    /* Clear registers (not strictly required) */
    la sp, _stack_top
    /* call kernel_main */
    la  a0, 0
    jal ra, kernel_main

/* reserve stack */
    .section .bss
    .align 12
_stack_top:
    .skip 4096*16
