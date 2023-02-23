#include "vm.h"
#include "kheap.h"
#include "common.h"

struct vm *vms[VM_MAX];
void vm_remove(int index) {
	if(
		index < 0 ||
		index > VM_MAX ||
		vms[index] == 0
	) {
		return;
	}

	vms[index] = 0;
}

int vm_add(struct vm *arg_vm) {
	int i = 0;
	while(vms[i] != 0 && i < VM_MAX) {
		i++;
	}

	if(i >= VM_MAX) {
		VM_PANIC("can't have any more vms");
	}

	vms[i] = arg_vm;
	return i;
}

static int get_bytes(
	unsigned char *data,
	int *index,
	int length
) {
	int result = 0;
	for(int i = 0; i < length; i++) {
		result |= data[*index + i] << (i * 8);
	}

	*index += length;
	return result;
}

static void set_vm_reg(
	int reg,
	int num,
	struct vm *arg_vm
) {
	for(int i = 0; i < arg_vm->mode; i++) {
		arg_vm->data[reg * arg_vm->mode + i] = 
			(unsigned char)(num >> (i * 8));
	}
}

typedef void (*instruction_t)(struct vm *);
static void add(struct vm *arg_vm) {
	int inst_reg_1 = VM_GET_BYTES(1);
	int inst_reg_2 = VM_GET_BYTES(1);
	int inst_reg_copy = inst_reg_1;
	set_vm_reg(
		inst_reg_copy,
		get_bytes(arg_vm->data, &inst_reg_1, arg_vm->mode) +
		get_bytes(arg_vm->data, &inst_reg_2, arg_vm->mode),
		arg_vm
	);
}

static void sub(struct vm *arg_vm) {
	int inst_reg_1 = VM_GET_BYTES(1);
	int inst_reg_2 = VM_GET_BYTES(1);
	int inst_reg_copy = inst_reg_1;
	set_vm_reg(
		inst_reg_copy,
		get_bytes(arg_vm->data, &inst_reg_1, arg_vm->mode) -
		get_bytes(arg_vm->data, &inst_reg_2, arg_vm->mode),
		arg_vm
	);
}

static void mul(struct vm *arg_vm) {
	int inst_reg_1 = VM_GET_BYTES(1);
	int inst_reg_2 = VM_GET_BYTES(1);
	int inst_reg_copy = inst_reg_1;
	set_vm_reg(
		inst_reg_copy,
		get_bytes(arg_vm->data, &inst_reg_1, arg_vm->mode) *
		get_bytes(arg_vm->data, &inst_reg_2, arg_vm->mode),
		arg_vm
	);
}

static void div(struct vm *arg_vm) {
	int inst_reg_1 = VM_GET_BYTES(1);
	int inst_reg_2 = VM_GET_BYTES(1);
	int inst_reg_copy = inst_reg_1;
	set_vm_reg(
		inst_reg_copy,
		get_bytes(arg_vm->data, &inst_reg_1, arg_vm->mode) /
		get_bytes(arg_vm->data, &inst_reg_2, arg_vm->mode),
		arg_vm
	);	
}

static void jump(struct vm *arg_vm) {
	int address = VM_GET_BYTES(4);
	if(
		address < 0 ||
		address > arg_vm->code_length
	) {
		VM_PANIC("jump out of bounds");
	}
	
	arg_vm->code_index = address;
}

static void compare(struct vm *arg_vm) {
	int inst_reg_1 = VM_GET_BYTES(1);
	int inst_reg_2 = VM_GET_BYTES(1);
	int a = get_bytes(arg_vm->data, &inst_reg_1, arg_vm->mode);
	int b = get_bytes(arg_vm->data, &inst_reg_2, arg_vm->mode);
	arg_vm->flags = 0;
	arg_vm->flags |= (a == b) << VM_FLAGS_EQUAL;
	arg_vm->flags |= (a == 0) << VM_FLAGS_ZERO_A;
	arg_vm->flags |= (b == 0) << VM_FLAGS_ZERO_B;
	arg_vm->flags |= (a > b) << VM_FLAGS_A_LARGER;
}

static void set(struct vm *arg_vm) {
	int inst_reg = VM_GET_BYTES(1);
	int value = VM_GET_BYTES(arg_vm->mode);
	set_vm_reg(inst_reg, value, arg_vm);
}

static void jump_equal(struct vm *arg_vm) {
	int address = VM_GET_BYTES(4);
	if(arg_vm->flags & VM_FLAGS_EQUAL) {
		arg_vm->code_index = address;
	}
}

static void jump_zero_a(struct vm *arg_vm) {
	int address = VM_GET_BYTES(4);
	if(arg_vm->flags & VM_FLAGS_ZERO_A) {
		arg_vm->code_index = address;
	}
}

static void jump_zero_b(struct vm *arg_vm) {
	int address = VM_GET_BYTES(4);
	if(arg_vm->flags & VM_FLAGS_ZERO_B) {
		arg_vm->code_index = address;
	}
}

static void jump_a_larger(struct vm *arg_vm) {
	int address = VM_GET_BYTES(4);
	if(arg_vm->flags & VM_FLAGS_A_LARGER) {
		arg_vm->code_index = address;
	}
}

static void stop(struct vm *arg_vm) {
	arg_vm->stop = 1;
}

static void mode(struct vm *arg_vm) {
	int mode = VM_GET_BYTES(1);
	arg_vm->mode = mode;
}

typedef void (*ffi_t)(unsigned char *);
ffi_t vm_ffi[VM_FFI_MAX] = { 0 };
static void ffi(struct vm *arg_vm) {
	int ffi_num = VM_GET_BYTES(1);
	vm_ffi[ffi_num](arg_vm->data);
}

static void set_reg(struct vm *arg_vm) {
	int inst_reg_1 = VM_GET_BYTES(1);
	int inst_reg_2 = VM_GET_BYTES(1);
	set_vm_reg(
		inst_reg_1,
		get_bytes(
			arg_vm->data,
			&inst_reg_2,
			arg_vm->mode
		),

		arg_vm
	);
}

static void vm_parse(
	struct vm *arg_vm
) {
	if(arg_vm->stop) {
		return;
	}
	
	instruction_t instructions[VM_INSTRUCTION_MAX] = {
		add,
		sub,
		mul,
		div,
		jump,
		compare,
		set,
		jump_equal,
		jump_zero_a,
		jump_zero_b,
		jump_a_larger,
		stop,
		mode,
		ffi,
		set_reg
	};

	int index = arg_vm->code[arg_vm->code_index];
	arg_vm->code_index++;
	instructions[index](arg_vm);
}

void vm_step(struct vm *arg_vm, int steps) {
	for(int i = 0; i < steps; i++) {
		vm_parse(arg_vm);
	}
}

struct vm vm_create(unsigned char *code, int length) {
	struct vm result;
	for(int i = 0; i < VM_DATA_MAX; i++) {
		result.data[i] = 0;
	}
	
	result.mode = 1;
	result.code = code;
	result.code_length = length;
	result.code_index = 0;
	result.flags = 0;
	result.stop = 0;
	return result;
}
