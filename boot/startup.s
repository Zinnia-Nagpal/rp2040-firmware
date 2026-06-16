
.syntax unified
.thumb

.section .vectors
.word _stack_top
.word reset_handler

.section .text
.global reset_handler
.thumb_func
reset_handler:
    LDR r0, =_sdata
    LDR r1, =_edata
    LDR r2, =_sidata
    CMP r0, r1
    BEQ skip_data
copy_loop:
    LDR r3, [r2]
    STR r3, [r0]
    ADDS r0, #4
    ADDS r2, #4
    CMP r0, r1
    BNE copy_loop
skip_data:
    LDR r0, =_sbss
    LDR r1, =_ebss
    MOVS r2, #0
    CMP r0, r1
    BEQ skip_bss
bss_loop:
    STR r2, [r0]
    ADDS r0, #4
    CMP r0, r1
    BNE bss_loop
skip_bss:
    BL main
deadloop:
    B deadloop
