#ifndef __VM_H
#define __VM_H
#define VM_DATA_MAX 4096
#define VM_MAX 256
#define VM_INSTRUCTION_MAX 15
#define VM_FFI_MAX 256
#define VM_GET_BYTES(x) \
	get_bytes(arg_vm->code, &arg_vm->code_index, x )

#define VM_PANIC(x) \
	print("vm "); \
	println( x ); \
	return

#define VM_FLAGS_EQUAL 1
#define VM_FLAGS_ZERO_A 2
#define VM_FLAGS_ZERO_B 4
#define VM_FLAGS_A_LARGER 8
struct vm {
	unsigned char data[VM_DATA_MAX];
	int mode;
	unsigned char *code;
	int code_length;
	int code_index;
	int flags;
	int stop;
	int step;
};

void vm_remove(int);
int vm_add(struct vm *, int);
struct vm vm_create(unsigned char *, int);
void vm_step(struct vm *, int);
void vm_install();
#endif
