#include "vga.h"
#include "common.h"

static int cursor_x = 0;
static int cursor_y = 0;
static unsigned short *video_memory = (unsigned short *) 0xb8000;
static void move_cursor() {
	unsigned short location = cursor_y * 80 + cursor_x;
	out_8(0x3d4, 14);
	out_8(0x3d5, location >> 8);
	out_8(0x3d4, 15);
	out_8(0x3d5, location);
}

void print_char(char arg_char) {
	unsigned char background = 0;
	unsigned char foreground = 15;
	unsigned char attribute_8 = 
		(background << 4) | (foreground & 0x0f);

	unsigned short attribute = attribute_8 << 8;
	if(arg_char == 0x08 && cursor_x) {
		cursor_x--;
	}

	switch(arg_char) {
		case 0x09:
			cursor_x = (cursor_x + 8) & ~(8 - 1);
			break;

		case '\r':
			cursor_x = 0;
			break;

		case '\n':
			cursor_x = 0;
			cursor_y++;
			break;

		default:
			unsigned short *location = 
				video_memory + (cursor_y * 80 + cursor_x);
			
			*location = arg_char | attribute;
			cursor_x++;
			break;
	}

	if(cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
		if(cursor_y >= 25) {
			cursor_y = 0;
		}
	}

	move_cursor();
}

void print_clear() {
	unsigned char attribute_8 = (0 << 4) | (15 & 0x0f);
	unsigned short blank = 0x20 | (attribute_8 << 8);
	for(int i = 0; i < 80 * 25; i++) {
		video_memory[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

void print(char *string) {
	for(int i = 0; string[i] != 0; i++) {
		print_char(string[i]);
	}
}
