#include <vga.h>

static inline unsigned char vga_entry_color(
	enum vga_color foreground,
	enum vga_color background
) {
	return foreground | background << 4;
}

static inline unsigned short vga_entry(
	unsigned char uc,
	unsigned char color
) {
	return (unsigned short)(uc) | (unsigned short)(color << 8);
}

static const int VGA_WIDTH = 80;
static const int VGA_HEIGHT = 25;
static int terminal_x = 0;
static int terminal_y = 0;
static unsigned char terminal_color = VGA_DEFAULT_COLOR;
unsigned short *terminal_buffer = (unsigned short *) 0xb8000;
void terminal_clear() {
	for(int y = 0; y < VGA_HEIGHT; y++) {
		for(int x = 0; x < VGA_WIDTH; x++) {
			const int index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_set_color(unsigned char color) {
	terminal_color = color;
}

void terminal_put_entry(
	char print_char,
	unsigned char color,
	int x,
	int y
) {
	const int index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(print_char, color);
}

void terminal_print_char(
	unsigned char print_char
) {
	terminal_put_entry(
		print_char,
		terminal_color,
		terminal_x,
		terminal_y
	);

	if(++terminal_x >= VGA_WIDTH) {
		terminal_x = 0;
		if(++terminal_y >= VGA_HEIGHT) {
			terminal_y = 0;
			terminal_clear();
		}
	}
}

void terminal_print(
	const unsigned char *print_string
) {
	for(int i = 0; print_string[i] != 0; i++) {
		terminal_print_char(print_string[i]);
	}
}
