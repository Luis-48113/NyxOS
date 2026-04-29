/* Multiboot kernel linker script */
ENTRY(_entry)

SECTIONS {
    . = 0x00100000;

    .multiboot ALIGN(4) : {
        *(.multiboot)
    }

    .text ALIGN(4K) : {
        *(.text)
        *(.rodata)
    }

    .data ALIGN(4K) : {
        *(.data)
    }

    .bss ALIGN(4K) : {
        *(.bss)
        *(COMMON)
        . = . + 32K;
        stack_top = .;
    }
}
