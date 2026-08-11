#ifndef TASK_H
#define TASK_H
#include <stdint.h>

// A task is mostly its saved stack pointer: switching tasks = swapping stacks.
struct task {
    uint32_t esp;          // saved stack pointer when this task is not running
    uint32_t stack_base;   // bottom of this task's stack
};

void sched_init(void);
void sched_add(void (*entry)(void));  // create a task that runs entry()
void yield(void);                     // voluntarily give up the CPU

#endif
