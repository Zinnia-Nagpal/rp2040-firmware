MEMORY{
    rom    (rx)  : ORIGIN =  0x10000000, LENGTH = 0x00040000
    sram   (rwx) : ORIGIN =  0x20000000, LENGTH = 264k
}
STACK_SIZE = 0x2000;
SECTIONS {

     .vector_table : {
        . = ALIGN(4);
        KEEP(*(.vector_table))
        . = ALIGN(4);
    } > rom
    .text :    { 
       *(.text*)
        *(.rodata.*)
    }>rom 
     
     _sidata = LOADADDR(.data); 
      .data : {
        _sdata = . ;
        *(.data*)
        _edata = . ;
    } > sram AT> rom

    /* uninitialized globals: just zeroed out in SRAM at startup */
    .bss : {
        _sbss = .;
        *(.bss*)
         *(COMMON)  /*  int foo; goes there, while static int foo; does not.*/
         _ebss = . ;
    } > sram

    .STACK (NOLOAD) :
    {
        
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
       _stack_top = .;
    } > sram
}