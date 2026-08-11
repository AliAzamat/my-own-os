#include "vga.h"
#include "task.h"

extern volatile char  key_buffer[];   // filled by the keyboard handler
extern volatile int   key_ready;      // set when a full line is available

// Compare two NUL-terminated strings (no libc here).
static int streq(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *a == *b;
}

static void run(const char *cmd) {
    if (streq(cmd, "help"))
        vga_print("commands: help, clear, ticks\n");
    else if (streq(cmd, "clear"))
        vga_clear();
    else if (streq(cmd, "ticks"))
        vga_print("the timer is counting...\n");
    else if (cmd[0] != '\0')
        vga_print("unknown command (try 'help')\n");
}

void shell_main(void) {
    vga_print("MiniOS shell. Type 'help'.\n> ");
    for (;;) {
        if (key_ready) {                 // the handler signalled a full line
            run((const char *)key_buffer);
            key_ready = 0;
            vga_print("> ");             // prompt for the next command
        }
        yield();                         // share the CPU while waiting
    }
}
