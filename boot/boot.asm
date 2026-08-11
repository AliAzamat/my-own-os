[bits 16]
[org 0x7C00]

KERNEL_OFFSET equ 0x1000  ; where in memory we load the kernel

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, msg_boot
    call print_string

    ; --- load the kernel from disk while we still have BIOS (real mode) ---
    mov bx, KERNEL_OFFSET ; es:bx = destination buffer (es is 0)
    mov dh, 15            ; read 15 sectors (our kernel, after sector 0)
    mov dl, 0x80          ; drive 0x80 = the boot hard disk
    call disk_load

    lgdt [gdt_descriptor]
    mov eax, cr0
    or  eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:protected_mode

; --- read DH sectors from drive DL (starting at sector 2) into ES:BX ---
disk_load:
    mov ah, 0x02          ; BIOS read-sectors function
    mov al, dh            ; how many sectors
    mov ch, 0x00          ; cylinder 0
    mov dh, 0x00          ; head 0
    mov cl, 0x02          ; start at sector 2 (sector 1 is the boot sector)
    int 0x13              ; BIOS disk service
    jc disk_error         ; carry set => read failed
    ret
disk_error:
    mov si, msg_disk_err
    call print_string
    jmp $

print_string:
    mov ah, 0x0E
.next:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .next
.done:
    ret

[bits 32]
protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov esp, 0x90000
    call KERNEL_OFFSET    ; jump into the C kernel we loaded
    jmp $                 ; if the kernel ever returns, halt

msg_boot     db "Booting OS...", 0
msg_disk_err db "Disk read failed!", 0

%include "boot/gdt.asm"

times 510 - ($ - $$) db 0
dw 0xAA55
