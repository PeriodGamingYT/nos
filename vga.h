#ifndef __VGA_H
#define __VGA_H
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GRAY = 7,
	VGA_COLOR_DARK_GRAY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

void terminal_clear();
void terminal_set_color(unsigned char);
void terminal_put_entry(
	char,
	unsigned char,
	int,
	int
);

void terminal_print_char(unsigned char);
void terminal_print(const unsigned char *);
#define print(x) terminal_print( x )
#define println(x) terminal_print( x ); terminal_print("\n")

// Default color is light gray on black.
#define VGA_DEFAULT_COLOR 7 | 0 << 4
#endif
