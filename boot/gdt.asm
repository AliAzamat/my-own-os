; The Global Descriptor Table (GDT) describes memory segments to the CPU in
; protected mode. We use a "flat" model: code and data each span all 4 GB,
; so segmentation effectively gets out of the way and we program in flat 32-bit.
gdt_start:
gdt_null:                 ; entry 0 must be all zeros (required by the CPU)
    dd 0x0
    dd 0x0
gdt_code:                 ; code segment: base=0, limit=4GB, executable, readable
    dw 0xFFFF             ; limit (bits 0-15)
    dw 0x0                ; base  (bits 0-15)
    db 0x0                ; base  (bits 16-23)
    db 10011010b          ; present, ring 0, code, executable, readable
    db 11001111b          ; granularity (4KB), 32-bit, limit bits 16-19
    db 0x0                ; base  (bits 24-31)
gdt_data:                 ; data segment: same span, writable, not executable
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b          ; present, ring 0, data, writable
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:           ; what we load into the GDTR register
    dw gdt_end - gdt_start - 1   ; size of the table minus 1
    dd gdt_start                 ; 32-bit address of the table

; Selectors: byte offset of each entry into the GDT (used to load segment regs)
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
