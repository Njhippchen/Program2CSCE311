/* trap/interrupt entry + simple dispatch */
    .section .text
    .globl user_trap_entry
    .globl kernel_trap_entry

/* Kernel trap entry: save minimal registers, call trap_handler */
kernel_trap_entry:
    /* Save caller-saved registers and sstatus, sepc on stack */
    addi sp, sp, -128
    sd ra, 0(sp)
    sd t0, 8(sp)
    sd t1, 16(sp)
    sd t2, 24(sp)
    sd a0, 32(sp)
    sd a1, 40(sp)
    sd a2, 48(sp)
    sd a3, 56(sp)
    sd a4, 64(sp)
    sd a5, 72(sp)
    /* save sstatus/sepc cause from CSR later in C */
    mv a0, sp
    call trap_handler
    ld ra, 0(sp)
    ld t0, 8(sp)
    ld t1, 16(sp)
    ld t2, 24(sp)
    ld a0, 32(sp)
    ld a1, 40(sp)
    ld a2, 48(sp)
    ld a3, 56(sp)
    ld a4, 64(sp)
    ld a5, 72(sp)
    addi sp, sp, 128
    /* return from trap */
    csrw sepc, a0   /* trap_handler returns new sepc in a0 */
    csrr a1, sstatus
    csrw sstatus, a1
    sret

/* User trap entry intentionally reuses kernel_trap_entry in this simple kernel */
user_trap_entry:
    j kernel_trap_entry
