[bits 32]
[extern isr_handler]
[extern timer_handler]
[extern keyboard_handler]
global isr0
global irq0
global irq1

isr0:
    pusha
    call isr_handler
    popa
    iret

irq0:                     ; timer (vector 32)
    pusha
    call timer_handler
    popa
    iret

irq1:                     ; keyboard (vector 33)
    pusha
    call keyboard_handler
    popa
    iret
