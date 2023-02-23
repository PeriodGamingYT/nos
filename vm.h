#ifndef __VM_H
#define __VM_H
#define VM_DATA_MAX 4096
#define VM_MAX 256
struct vm {
	unsigned char data[VM_DATA_MAX];
	int mode;
};

void vm_remove(int);
int vm_add(struct vm *);
void vm_feed(unsigned char *, int, int);
#endif
