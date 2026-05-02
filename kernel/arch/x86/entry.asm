.section .multiboot
	.long 0x1BADB002
	.long 0x00000003 | 0x00010000  # Request framebuffer info
	.long -(0x1BADB002 + 0x00000003 | 0x00010000)

.section .text
.global _entry
.extern kernel_main
.extern stack_top

_entry:
	push %ebx  # Multiboot info pointer
	push %eax  # Multiboot magic number
	mov $stack_top, %esp
	call kernel_main
	hlt

.global load_idt
load_idt:
	mov 4(%esp), %eax
	lidt (%eax)
	ret

.global load_tss
load_tss:
	mov 4(%esp), %eax
	ltr %ax
	ret

.global enable_interrupts
enable_interrupts:
	sti
	ret

.global disable_interrupts
disable_interrupts:
	cli
	ret

.macro IRQ_HANDLER num
.global irq_handler_\num
irq_handler_\num:
	push $0
	push $\num
	jmp irq_common
.endm

IRQ_HANDLER 0
IRQ_HANDLER 1
IRQ_HANDLER 2
IRQ_HANDLER 3

.global irq_common
irq_common:
	push %ds
	push %es
	push %fs
	push %gs
	push %eax
	push %ebp
	push %esi
	push %edi
	push %ebx
	push %ecx
	push %edx
	
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	
	mov 48(%esp), %eax
	push %eax
	call irq_handler
	add $4, %esp
	
	pop %edx
	pop %ecx
	pop %ebx
	pop %edi
	pop %esi
	pop %ebp
	popl %eax
	pop %gs
	pop %fs
	pop %es
	pop %ds
	
	add $8, %esp
	iret
