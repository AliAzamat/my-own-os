#include "pmm.h"

// We manage the first 16 MB of RAM as 4 KB frames: 16MB / 4KB = 4096 frames.
#define NUM_FRAMES 4096
#define BITMAP_LEN (NUM_FRAMES / 32)     // one bit per frame, packed into u32s

static uint32_t frame_bitmap[BITMAP_LEN]; // bit set = frame in use

static void set_used(int i)  { frame_bitmap[i / 32] |=  (1u << (i % 32)); }
static void set_free(int i)  { frame_bitmap[i / 32] &= ~(1u << (i % 32)); }
static int  is_used(int i)   { return frame_bitmap[i / 32] &  (1u << (i % 32)); }

void pmm_init(void) {
    for (int i = 0; i < BITMAP_LEN; i++) frame_bitmap[i] = 0;
    // Reserve frame 0 (low memory / null) so we never hand out address 0.
    set_used(0);
}

// Find the lowest free frame, mark it used, return its physical address.
void *pmm_alloc_frame(void) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (!is_used(i)) {
            set_used(i);
            return (void *)(uintptr_t)(i * PAGE_SIZE);
        }
    }
    return 0;                  // out of physical memory
}

void pmm_free_frame(void *frame) {
    int i = (int)((uintptr_t)frame / PAGE_SIZE);
    set_free(i);
}
