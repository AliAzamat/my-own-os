; The context switch: save the outgoing task's stack pointer, load the
; incoming task's. Because each task's registers live ON its own stack,
; swapping esp swaps everything about which task is running.
[bits 32]
global context_switch

context_switch:               ; void context_switch(uint32_t *old, uint32_t new)
    mov eax, [esp + 4]        ; eax = &old->esp
    mov edx, [esp + 8]        ; edx = new esp value
    push ebx                  ; save callee-saved registers on the OLD stack
    push esi
    push edi
    push ebp
    mov [eax], esp            ; save the old task's esp through the pointer
    mov esp, edx              ; switch to the new task's stack
    pop ebp                   ; restore the NEW task's saved registers
    pop edi
    pop esi
    pop ebx
    ret                       ; "return" into the new task
