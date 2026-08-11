#include "vga.h"

// VGA text mode: an 80x25 grid at 0xB8000, two bytes per cell
// (character byte, then attribute/color byte).
#define VGA_MEM   ((volatile unsigned short *)0xB8000)
#define VGA_COLS  80
#define VGA_ROWS  25
#define COLOR     0x0F        // white on black, in the high byte

static int cursor = 0;        // current cell index (0 .. COLS*ROWS-1)

// One cell value = character in the low byte, color in the high byte.
static unsigned short cell(char c) {
    return (unsigned short)c | (COLOR << 8);
}

void vga_clear(void) {
    for (int i = 0; i < VGA_COLS * VGA_ROWS; i++)
        VGA_MEM[i] = cell(' ');
    cursor = 0;
}

// Scroll up one line when we run past the bottom of the screen.
static void scroll(void) {
    for (int i = 0; i < VGA_COLS * (VGA_ROWS - 1); i++)
        VGA_MEM[i] = VGA_MEM[i + VGA_COLS];          // copy each row up one
    for (int i = 0; i < VGA_COLS; i++)
        VGA_MEM[VGA_COLS * (VGA_ROWS - 1) + i] = cell(' ');  // blank last row
    cursor = VGA_COLS * (VGA_ROWS - 1);
}

void vga_putc(char c) {
    if (c == '\n') {
        cursor += VGA_COLS - (cursor % VGA_COLS);     // jump to start of next row
    } else {
        VGA_MEM[cursor++] = cell(c);
    }
    if (cursor >= VGA_COLS * VGA_ROWS) scroll();       // off the bottom? scroll
}

void vga_print(const char *s) {
    while (*s) vga_putc(*s++);
}
