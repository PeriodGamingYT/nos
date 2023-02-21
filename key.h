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
#define KEY_ALT 1000
#define KEY_CAPS 1001
#define KEY_NUM_LOCK 1002
#define KEY_SCROLL_LOCK 1003
#define KEY_HOME 1004
#define KEY_UP 1005
#define KEY_PAGE_UP 1006
#define KEY_LEFT 1007
#define KEY_RIGHT 1008
#define KEY_END 1009
#define KEY_DOWN 1010
#define KEY_PAGE_DOWN 1011
#define KEY_INSERT 1012
#define KEY_DELETE 1013
#define KEY_F11 1014
#define KEY_F12 1015
#define KEY_UNDEFINED 1016
void keyboard_install();
#endif
