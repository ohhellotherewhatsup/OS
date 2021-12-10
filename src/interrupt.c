#include "interrupt.h"

void idt_init(void) {
    struct idt_entry IDT[IDT_SIZE];

    // The address of the asm method, to populate the idt_entry vector number 33.
    unsigned long irq1_address;

    // Initialize the PIC chips
    // The specific code can be found in my github repo
    // Not necessary for this overview

    // Populating the idt_entry vector for the keyboard interrupt
    irq1_address = (unsigned long)irq1; 
    IDT[33].offset_lowerbits = irq1_address & 0xffff;
    IDT[33].selector = 0x10; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[33].zero = 0;
    IDT[33].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;


    struct idt_ptr_t idt_ptr;

    // fill the IDT descriptor 
    unsigned long idt_address = (unsigned long)IDT ;
    idt_ptr.limit = (sizeof (struct idt_entry) * 256) + ((idt_address & 0xffff) << 16);
    idt_ptr.base = idt_address >> 16 ;

    // Finally, call the asm method to initialize the IDT
    load_idt(idt_ptr);
}
