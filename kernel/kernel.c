#include "vga.h"
#include "idt.h"
#include "irq.h"
#include "pmm.h"
#include "paging.h"
#include "task.h"

extern void syscall_install(void);
extern void shell_main(void);

void kernel_main(void) {
    vga_clear();
    vga_print("MiniOS booting...\n");

    idt_install();        // CPU exception handlers
    irq_install();        // PIC remap + timer + keyboard
    pmm_init();           // physical frame allocator
    paging_init();        // virtual memory on
    syscall_install();    // int 0x80 gate
    sched_init();         // scheduler (kernel is task 0)

    __asm__ volatile ("sti");   // enable interrupts: timer + keyboard live

    vga_print("MiniOS ready.\n");
    shell_main();         // drop into the shell — never returns

    for (;;) { }
}
