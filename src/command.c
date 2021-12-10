#include "bool.h"
#include "kernel.h"

#include "util.h"

void kernel_parse_command(void) {

	if (util_strcmp(kernel.cli, "ping")) {
		vga_write_enter();
		vga_write("Pong!", WHITE, BLACK);
	}

	util_memset(kernel.cli, 0, kernel.cli_pos);
	kernel.cli_pos = 0;
}
