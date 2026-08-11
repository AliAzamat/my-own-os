#ifndef PORTS_H
#define PORTS_H
#include <stdint.h>

// x86 talks to many devices through a separate "I/O port" address space,
// reached with the in/out instructions rather than memory addresses.
static inline uint8_t inb(uint16_t port) {
    uint8_t r;
    __asm__ volatile ("inb %1, %0" : "=a"(r) : "Nd"(port));
    return r;
}
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

#endif
