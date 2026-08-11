; The bootloader's `call KERNEL_OFFSET` lands here, at the very start of the
; kernel image. This stub immediately calls the C entry point. Linking this
; FIRST guarantees C code isn't sitting at offset 0 by accident.
[bits 32]
[extern kernel_main]      ; defined in kernel.c

global _start
_start:
    call kernel_main      ; hand off to C
    jmp $                 ; if it returns, halt
