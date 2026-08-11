#ifndef PAGING_H
#define PAGING_H
#include <stdint.h>

void paging_init(void);                       // build tables + enable the MMU
void map_page(uint32_t virt, uint32_t phys);  // map one virtual page -> physical

#endif
