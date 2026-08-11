#include "task.h"
#include "pmm.h"

#define MAX_TASKS 8

static struct task tasks[MAX_TASKS];
static int n_tasks = 0;
static int current = 0;

// Defined in switch.asm: save current esp into *old, load esp from *next.
extern void context_switch(uint32_t *old_esp, uint32_t new_esp);

void sched_init(void) {
    n_tasks = 1;           // task 0 is the kernel we are already running as
    current = 0;
}

void sched_add(void (*entry)(void)) {
    struct task *t = &tasks[n_tasks];
    uint32_t stack = (uint32_t)pmm_alloc_frame() + PAGE_SIZE;  // top of a fresh page

    // Build a fake stack frame so the first switch "returns" into entry().
    uint32_t *sp = (uint32_t *)stack;
    *(--sp) = (uint32_t)entry;     // address ret will jump to
    t->esp = (uint32_t)sp;
    t->stack_base = stack;
    n_tasks++;
}

void yield(void) {
    int prev = current;
    current = (current + 1) % n_tasks;      // round-robin: next task
    if (current == prev) return;            // only one task — nothing to do
    context_switch(&tasks[prev].esp, tasks[current].esp);
}
