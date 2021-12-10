#include "kernel.h"
#include "ports.h"
#include "bool.h"
#include "interrupt.h"

#include "util.h"

kernel_t kernel;

uint16_t vga_entry(char ch, uint8_t fore_color, uint8_t back_color) {
	uint16_t ax;

	ax = back_color;
	ax <<= 4;
	ax |= fore_color;
	ax <<= 8;

	ax |= ch;

	return ax;
}

void vga_write(char *str, uint8_t fore_color, uint8_t back_color) {
	while (*str)
		kernel.vga_buff[kernel.vga_buff_pos++] = vga_entry(*str++, fore_color, back_color);
}

void vga_write_char(char str, uint8_t fore_color, uint8_t back_color) {
	kernel.vga_buff[kernel.vga_buff_pos++] = vga_entry(str, fore_color, back_color);
}

uint8_t kernel_read(uint16_t port) {
	uint8_t ch;

	asm volatile ("inb %1, %0" : "=a" (ch) : "d" (port));
	return ch;
}

char kernel_get_char(void) {
	char ch;

	while ((ch = kernel_read(KEYBOARD_PORT)) != 0) {
		if (ch > 0)
			return ch;
	}

	return ch;
}

void wait_for_io(uint32_t timer_count) {
	while(TRUE) {
		asm volatile("nop");
		timer_count--;

		if (timer_count <= 0)
			break;
	}
}

void kernel_clear(void) {
	for (int i = 0; i < BUFSIZE; i++)
		kernel.vga_buff[i] = vga_entry(0, WHITE, BLACK);
}

void vga_write_enter(void) {
	kernel.vga_buff_pos = 80 * kernel.vga_line_pos++;
}

void kernel_init(void) {
	// init the kernel

	util_memset(kernel.cli, 0, 256);

	kernel.vga_buff = (uint16_t *)VGA_START;
	kernel.vga_buff_pos = 0;
	kernel.vga_line_pos = 0;
	kernel.cli_pos = 0;
}

void kernel_prompt(void) {
	vga_write_enter();

	vga_write("builtin", BRIGHT_RED, BLACK);
	vga_write(" ", WHITE, BLACK);
	vga_write("/", BRIGHT_MAGENTA, BLACK);
	vga_write(" ", WHITE, BLACK);
	vga_write("#", BRIGHT_RED, BLACK);
}

void vga_delete_char(void) {
	kernel.vga_buff[--kernel.vga_buff_pos] = 0;
	kernel.cli[--kernel.cli_pos] = 0;
}

void irq1_handler(void) {
	vga_write("Got input!", WHITE, BLACK);
}

void kernel_start(void) {
	idt_init();

	kernel_init();
	kernel_clear();

	vga_write("This is a simple kernel designed for learning purposes", WHITE, BLACK);
	vga_write_enter();
	vga_write_enter();

	kernel_prompt();

	while (TRUE) {
		char ch = get_char(kernel_get_char());

		switch (ch) {
			case KEY_ENTER:
				kernel_parse_command();
				kernel_prompt();
				wait_for_io(0x02FFFFFF * 2);
				continue;

			case KEY_BACKSPACE:
				vga_delete_char();
				wait_for_io(0x02FFFFFF * 2);
				continue;
		}

		// add to CLI
		kernel.cli[kernel.cli_pos++] = ch;

		vga_write_char(ch, WHITE, BLACK);

		wait_for_io(0x02FFFFFF * 2);
	}
}
