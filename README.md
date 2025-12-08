# Program2CSCE311

video link: https://youtu.be/Re-hm_NVapg

program2 report: https://docs.google.com/document/d/1B1_x7Q1T4aBe1K0yqjrT5z89isa068sqWE2u1BpnHtc/edit?usp=sharing

Clear instructions to build and run your OS:
1. Install the required RISC-V toolchain and QEMU emulator.
2. Compile the OS kernel and embedded user programs.
3. Boot the kernel and observe basic process scheduling and system calls.
4. Can change the user code and observe its behavior inside the kernel.

Dependencies:
You need a RISC-V cross toolchain and QEMU (RISC-V target). Example test environment:

riscv64-unknown-elf-gcc, riscv64-unknown-elf-ld (or other riscv64-*-elf toolchain)

qemu-system-riscv64 (QEMU with virt machine support)

make, gcc (host), binutils (host)

QEMU command to run:
make run

Expected behavior description:
You should see the kernel boot message followed by the embedded user programs outputting text:
mini-rvOS booting...
hello from user program
<stuff from cat program or other user apps>

What happens internally:
kernel_main() initializes the kernel and proc_table.
The kernel spawns the embedded user programs (symbols linked into the kernel).
Simple user programs use ecall to perform syscalls (write/yield/read/spawn/exit).
The scheduler switches between ready processes in a round-robin fashion.