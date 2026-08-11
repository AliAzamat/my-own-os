#ifndef PMM_H
#define PMM_H
#include <stdint.h>

#define PAGE_SIZE 4096        // x86 pages are 4 KB

void  pmm_init(void);
void *pmm_alloc_frame(void);  // returns a 4 KB physical frame, or 0 if none
void  pmm_free_frame(void *frame);

#endif
