#include "idt.h"
#include "vga.h"

#define IDT_SIZE 256
static struct idt_entry idt[IDT_SIZE];

// The pointer we load into the CPU's IDTR register.
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_set_gate(int n, uint32_t handler) {
    idt[n].handler_low  = handler & 0xFFFF;
    idt[n].handler_high = (handler >> 16) & 0xFFFF;
    idt[n].selector     = 0x08;       // CODE_SEG: our 32-bit code segment
    idt[n].zero         = 0;
    idt[n].flags        = 0x8E;       // present, ring 0, 32-bit interrupt gate
}

// Implemented in isr.asm: the low-level stub for interrupt vector 0.
extern void isr0(void);

void idt_install(void) {
    idt_set_gate(0, (uint32_t)isr0);  // vector 0 = divide-by-zero exception

    struct idt_ptr ptr;
    ptr.limit = sizeof(idt) - 1;
    ptr.base  = (uint32_t)&idt;
    __asm__ volatile ("lidt (%0)" : : "r"(&ptr));   // load the IDTR
}

// C-level handler the assembly stub calls. For now it just reports and halts.
void isr_handler(void) {
    vga_print("\n[EXCEPTION] CPU fault — halting.\n");
    for (;;) { }
}
