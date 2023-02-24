#ifndef __KEY_H
#define __KEY_H
#define KEY_CAPS_LOCK 0x3a
#define KEY_LEFT_SHIFT 0x2a
#define KEY_RIGHT_SHIFT 0x36
#define KEY_LEFT_SHIFT_RELEASE 0xaa
#define KEY_RIGHT_SHIFT_RELEASE 0xb6

// Q: Why are these here?
// A: Intentional design choice, instead of having "special" keys only
// accessible through escape codes in a correct termois config. I just
// instead make every key a short, with the "special" keys starting at 1000.
// It might cause trouble for you if you are implementing something like 
// UTF-8 (Maybe in a fork). But here I am only targeting ASCII. 
// So this is why it is this way.
#define KEY_ALT 129
#define KEY_CAPS 130
#define KEY_NUM_LOCK 131
#define KEY_SCROLL_LOCK 132
#define KEY_HOME 133
#define KEY_UP 134
#define KEY_PAGE_UP 135
#define KEY_LEFT 136
#define KEY_RIGHT 137
#define KEY_END 138
#define KEY_DOWN 139
#define KEY_PAGE_DOWN 140
#define KEY_INSERT 141
#define KEY_DELETE 142
#define KEY_F11 143
#define KEY_F12 144
#define KEY_UNDEFINED 145
#define KEYS_MAX 146
void keyboard_install();
unsigned short key_get_first();
#endif
