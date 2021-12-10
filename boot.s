.set MAGIC, 0x1BADB002 # identified by bootloader

# cpu flag
.set FLAGS, 0

.set CHECKSUM, -(MAGIC + FLAGS)

# enable multiboot
.section .multiboot

# for grub to read
.long MAGIC
.long FLAGS
.long CHECKSUM

# 512 bytes big
stack_bottom:
	.skip 1024

stack_top:
	.section .text
	.global irq1
	.global load_idt
	.global _start

	.type irq1, @function
	.type load_idt, @function
	.type _start, @function

_start:
	mov $stack_top, %esp

	call kernel_start
	cli

# interrupt handler

irq1:
	pushal # pushes all registers on stack
	cld # clears direction flag (influence direction of how some instructions work)
	call irq1_handler # keyboard handler
	popal # pop all registers off stack
	#iretd # interrupt return
	ret

load_idt:
	#mov %edx, 4(%esp)
	mov 4(%esp), %edx
	lidt (%edx) # load IDT
	sti # set interrupt flag
	ret

# just halt
hlt_loop:
	hlt
	jmp hlt_loop

.size _start, . - _start
