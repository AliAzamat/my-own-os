#ifndef IDT_H
#define IDT_H
#include <stdint.h>

// One IDT entry (a "gate"): where to jump when interrupt N fires.
struct idt_entry {
    uint16_t handler_low;   // handler address bits 0-15
    uint16_t selector;      // code segment selector (our CODE_SEG)
    uint8_t  zero;          // always 0
    uint8_t  flags;         // present, ring, gate type
    uint16_t handler_high;  // handler address bits 16-31
} __attribute__((packed));

void idt_install(void);
void idt_set_gate(int n, uint32_t handler);

#endif
