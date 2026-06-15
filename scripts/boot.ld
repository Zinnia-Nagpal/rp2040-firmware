
MEMORY {
    rom  (rx)  : ORIGIN = 0x10000100, LENGTH = 0x0003FF00
    sram (rwx) : ORIGIN = 0x20000000, LENGTH = 264k
}
STACK_SIZE = 0x2000;
SECTIONS {
    .text : {
        KEEP(*(.text*))
        *(.rodata*)
    } > rom

    _sidata = LOADADDR(.data);
    .data : {
        . = ALIGN(4);
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > sram AT > rom

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > sram

    .stack (NOLOAD) : {
        . = ALIGN(8);
        . = . + STACK_SIZE;
        . = ALIGN(8);
        _stack_top = .;
    } > sram
}
