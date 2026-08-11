#include "ports.h"
#include "idt.h"
#include "vga.h"

// The PIC (Programmable Interrupt Controller) delivers hardware IRQs. By
// default IRQ0-7 arrive as vectors 8-15, which collide with CPU exceptions,
// so we remap the PIC to deliver them as vectors 32-47 instead.
void pic_remap(void) {
    outb(0x20, 0x11); outb(0xA0, 0x11);   // start init sequence
    outb(0x21, 0x20); outb(0xA1, 0x28);   // master -> vec 32, slave -> vec 40
    outb(0x21, 0x04); outb(0xA1, 0x02);   // wire master/slave together
    outb(0x21, 0x01); outb(0xA1, 0x01);   // 8086 mode
    outb(0x21, 0x00); outb(0xA1, 0x00);   // unmask all IRQs
}

extern void irq0(void);   // timer  stub (vector 32)
extern void irq1(void);   // keyboard stub (vector 33)

void irq_install(void) {
    pic_remap();
    idt_set_gate(32, (uint32_t)irq0);
    idt_set_gate(33, (uint32_t)irq1);
}

// Minimal US scancode set: index = scancode, value = ASCII (0 = ignore).
static const char SCANCODES[] = {
    0,0,'1','2','3','4','5','6','7','8','9','0','-','=',0,0,
    'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',
};

volatile uint32_t ticks = 0;   // incremented every timer interrupt

void timer_handler(void) {
    ticks++;
    outb(0x20, 0x20);          // End-Of-Interrupt to the master PIC
}

void keyboard_handler(void) {
    uint8_t sc = inb(0x60);    // read the scancode from the keyboard port
    if (sc < sizeof(SCANCODES) && SCANCODES[sc]) {
        char buf[2] = { SCANCODES[sc], 0 };
        vga_print(buf);        // echo the typed character
    }
    outb(0x20, 0x20);          // End-Of-Interrupt
}
