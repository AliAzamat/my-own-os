# My Own Operating System (From Scratch)

A from-scratch project that demystifies the operating system by building a minimal x86 OS from the ground up. Started in 16-bit real mode with a 512-byte bootloader, switched the CPU into 32-bit protected mode, loaded and jumped into a C kernel, printed to the screen by writing VGA text memory directly, installed a Global Descriptor Table and an Interrupt Descriptor Table, took keyboard interrupts, turned on paging for virtual memory, ran a tiny cooperative scheduler, and finished with a minimal shell reached through a software-interrupt syscall. Every step adds one or two files so the final tree is a real, bootable mini-OS that can be run in QEMU.

## Stack
- C
- x86 Assembly
- NASM
- QEMU
- Make
- GDB
