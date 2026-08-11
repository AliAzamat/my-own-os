#include "idt.h"
#include "vga.h"

// Syscall numbers — the stable contract between programs and the kernel.
#define SYS_WRITE 1

extern void syscall_stub(void);   // isr.asm: the int 0x80 entry stub

void syscall_install(void) {
    idt_set_gate(0x80, (uint32_t)syscall_stub);  // int 0x80 -> our gate
}

// Called by the stub. eax = syscall number, ebx = argument.
void syscall_dispatch(uint32_t num, uint32_t arg) {
    switch (num) {
    case SYS_WRITE:
        vga_print((const char *)arg);   // arg is a pointer to a string
        break;
    default:
        vga_print("[syscall] unknown call\n");
    }
}
