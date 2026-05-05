
.text
.syntax unified
.thumb

/* vector table - goes in .vector_table section */
.section .vector_table
.word _stack_top
.word reset_handler
/* reset handler - goes in .text section */
.section .text
.global reset_handler
.thumb_func
reset_handler:

LDR r0, =_sdata   
LDR r1, =_edata
LDR r2, =_sidata

    /* loop: load word from source, store to destination */
    /* increment both pointers by 4 */
    /* compare destination to end, loop if not done */
loop:
    LDR r3, [r2]  /* read one word FROM flash */
    STR r3, [r0] /* write that word TO sram */
    ADDS r0 , #4
    ADDS r2, #4
    CMP r0 , r1
    BNE loop

    LDR r0, =_sbss
    LDR r1, =_ebss
    MOV r2, #0
    bss_loop:
     STR r2, [r0]
     ADDS r0, #4
     CMP r0, r1
     BNE bss_loop

     BL main
     deadloop:
      B deadloop