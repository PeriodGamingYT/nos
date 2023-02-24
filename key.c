#include "key.h"
#include "isr.h"
#include "common.h"
#include "vga.h"

// http://www.osdever.net/bkerndev/Docs/keyboard.htm
unsigned short key_layout[128] = {
    KEY_UNDEFINED,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    KEY_ALT,	/* Alt */
  ' ',	/* Space bar */
    KEY_CAPS,	/* Caps lock */
    KEY_UNDEFINED,	/* 59 - F1 key ... > */
    KEY_UNDEFINED,   KEY_UNDEFINED,   KEY_UNDEFINED,   KEY_UNDEFINED,   
    KEY_UNDEFINED,   KEY_UNDEFINED,   KEY_UNDEFINED,   KEY_UNDEFINED,
    KEY_UNDEFINED,	/* < ... F10 */
    KEY_NUM_LOCK,	/* 69 - Num lock*/
    KEY_SCROLL_LOCK,	/* Scroll Lock */
    KEY_HOME,	/* Home key */
    KEY_UP,	/* Up Arrow */
    KEY_PAGE_UP,	/* Page Up */
  '-',
    KEY_LEFT,	/* Left Arrow */
    0,
    KEY_RIGHT,	/* Right Arrow */
  '+',
    KEY_END,	/* 79 - End key*/
    KEY_DOWN,	/* Down Arrow */
    KEY_PAGE_DOWN,	/* Page Down */
    KEY_INSERT,	/* Insert Key */
    KEY_DELETE,	/* Delete Key */
    KEY_UNDEFINED,   KEY_UNDEFINED,   KEY_UNDEFINED,
    KEY_F11,	/* F11 Key */
    KEY_F12,	/* F12 Key */
    KEY_UNDEFINED,	/* All other keys are undefined */
};

unsigned char keys[33] = { 0 };
static unsigned char turn_off_bit(unsigned char n, int k) {
	return (n & ~(1 << (k - 1)));
}

int is_shift = 0;
int caps_lock = 0;
char key_shift[129] = {
	// 0 - 22.
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// 23 - 43.
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '\"', 0, 0, 0, 0,

	// 44 - 56.
	'<', '_', '>', '?', ')', '!', '@', '#', '$', '%', '^', '&', '*',

	// 57 - 75.
	'(', 0, ':', 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	// 76 - 96.
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '{', '|', '}', 0,0, '~',

	// 97 - 109.
	'A', 'B', 'C', 'D', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',

	// 110 - 122.
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'Q', 'R', 'S', 'T',

	// 123 - 127.
	'U', 'V', 'W', 'X', 'Y', 'Z'
};

static char remap_keys(char key) {
	int is_caps = 
		(is_shift || caps_lock) && (is_shift != caps_lock);

	if(is_caps && key >= 'a' && key <= 'z') {
		key -= 'a' - 'A';
		return key;
	}

	if(is_caps && key >= 'A' && key <= 'Z') {
		key += 'a' - 'A';
		return key;
	}

	if(!is_shift) {
		return key;
	}

	return key_shift[(int)key];
}

static void keyboard_callback(struct regs *arg_regs) {
	unsigned char orig_scancode = in_8(0x60);
	if(orig_scancode == KEY_CAPS_LOCK) {
		caps_lock = !caps_lock;
	}
	
	if(
		orig_scancode == KEY_LEFT_SHIFT ||
		orig_scancode == KEY_RIGHT_SHIFT ||
		orig_scancode == KEY_LEFT_SHIFT_RELEASE ||
		orig_scancode == KEY_RIGHT_SHIFT_RELEASE
	) {
		is_shift = !(orig_scancode & 0x80);
	}
	
	unsigned char scancode = 
		remap_keys(key_layout[orig_scancode & ~(0x80)]);
	
	#ifdef COMMON_DEBUG_KEY
		print("key char ");
		print_char(scancode);
		print(" int (orig) ");
		print_num(orig_scancode);
		print(" release ");
		print_num((orig_scancode & 0x80) != 0);
		println("");
	#endif
	
	if(orig_scancode & 0x80) {
		unsigned char n_index = (int)(scancode / 8);
		int k = scancode % 8;
		keys[n_index] = turn_off_bit(keys[n_index], k);
		return;
	}
	
	keys[(int)(scancode / 8)] |= 1 << (scancode % 8);
}

unsigned short key_get_first() {
	for(int i = 0; i < KEYS_MAX; i++) {
		if(keys[(int)(i / 8)]) {
			continue;
		}
		
		if(keys[(int)(i / 8)] & (1 << (i % 8))) {
			return i;
		}
	}

	return KEY_UNDEFINED;
}

void keyboard_install() {
	register_int_handler(IRQ_NUM(1), (isr_t) keyboard_callback);
}
