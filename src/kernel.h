#pragma once

#include <stdint.h>

#define VGA_START 0xB8000
#define BUFSIZE 2200

typedef uint8_t color_t;

enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};

struct kernel_t {
	uint16_t *vga_buff;
	uint16_t vga_buff_pos;

	uint8_t vga_line_pos;

	char cli[256];
	uint8_t cli_pos;

} typedef kernel_t;

extern kernel_t kernel;

void vga_write_char(char, uint8_t, uint8_t);
void vga_write(char *, uint8_t, uint8_t);
void vga_write_enter(void);

char get_char(uint8_t);

void kernel_parse_command(void);
