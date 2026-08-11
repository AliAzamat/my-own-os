#ifndef VGA_H
#define VGA_H

// A tiny screen API the rest of the kernel will use to talk to the user.
void vga_clear(void);
void vga_putc(char c);
void vga_print(const char *s);

#endif
