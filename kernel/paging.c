#include "paging.h"
#include "pmm.h"

// x86 32-bit paging is two levels: a page directory of 1024 entries, each
// pointing at a page table of 1024 entries, each mapping one 4 KB page.
// 1024 * 1024 * 4 KB = the full 4 GB virtual address space.
#define ENTRIES 1024
#define PRESENT_RW 0x3               // page present (bit 0) + writable (bit 1)

static uint32_t *page_directory;     // 1024 entries, 4 KB aligned

void map_page(uint32_t virt, uint32_t phys) {
    uint32_t dir_i = virt >> 22;            // top 10 bits select the directory entry
    uint32_t tbl_i = (virt >> 12) & 0x3FF;  // next 10 bits select the table entry

    uint32_t *table;
    if (!(page_directory[dir_i] & 0x1)) {           // no table yet for this range?
        table = (uint32_t *)pmm_alloc_frame();      // allocate one
        for (int i = 0; i < ENTRIES; i++) table[i] = 0;
        page_directory[dir_i] = (uint32_t)table | PRESENT_RW;
    } else {
        table = (uint32_t *)(page_directory[dir_i] & ~0xFFF);
    }
    table[tbl_i] = (phys & ~0xFFF) | PRESENT_RW;     // map the virtual page -> phys
}

void paging_init(void) {
    page_directory = (uint32_t *)pmm_alloc_frame();
    for (int i = 0; i < ENTRIES; i++) page_directory[i] = 0;

    // Identity-map the first 16 MB so kernel addresses don't move when we
    // switch paging on (virtual address == physical address there).
    for (uint32_t addr = 0; addr < 0x1000000; addr += 0x1000)
        map_page(addr, addr);

    // Point CR3 at the directory, then set CR0.PG to turn the MMU on.
    __asm__ volatile ("mov %0, %%cr3" : : "r"(page_directory));
    uint32_t cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;                                // PG = paging enable
    __asm__ volatile ("mov %0, %%cr0" : : "r"(cr0));
}
