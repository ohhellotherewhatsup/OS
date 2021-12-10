#pragma once

#define IDT_SIZE 256

extern int load_idt();
extern int irq1();

struct idt_entry {
	unsigned short int offset_lowerbits;
	unsigned short int selector;     // Kernel segment goes here
	unsigned char zero;              // Always zero
	unsigned char type_attr;         // Flags set using the table
	unsigned short int offset_higherbits;
} __attribute__ ((packed));

struct idt_ptr_t {
	unsigned short limit;
	unsigned int base;
} __attribute__ ((packed));


void idt_init(void);
