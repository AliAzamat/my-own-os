# Build Your Own Operating System (From Scratch)

A from-scratch intermediate project that demystifies the operating system by building a minimal x86 OS from the ground up. You start in 16-bit real mode with a 512-byte bootloader, switch the CPU into 32-bit protected mode, load and jump into a C kernel, print to the screen by writing VGA text memory directly, install a Global Descriptor Table and an Interrupt Descriptor Table, take keyboard interrupts, turn on paging for virtual memory, run a tiny cooperative scheduler, and finish with a minimal shell reached through a software-interrupt syscall. Every step adds one or two files so the final tree is a real, bootable mini-OS you can run in QEMU.

Built step-by-step with [KhwajaLabs Build](https://khwajalabs.com).

## Stack
- C
- x86 Assembly
- NASM
- QEMU
- Make
- GDB
