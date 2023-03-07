#include "vm.h"
#include "kheap.h"
#include "common.h"
#include "vga.h"
#include "key.h"
#include "timer.h"

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

int vm_max = 0;
int vm_add(struct vm *arg_vm, int step) {
	int i = 0;
	while(vms[i] != 0 && i < VM_MAX) {
		i++;
	}

	if(i >= VM_MAX) {
	
		// expansion of macro is:
		// print("vm  "); println("can't have any more vms"); return
		// the -1 finishes the return. VM_PANIC is usually used in
		// instruction_t callbacks.
		VM_PANIC("can't have any more vms") - 1;
	}

	arg_vm->step = step;
	vms[i] = arg_vm;
	vm_max = i;
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
	#ifdef COMMON_DEBUG_VM
		print("get_bytes index");
		print_dec(*index);
		print(" result ");
		print_dec(result);
		println("");
	#endif
	
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

static void ffi_print(struct vm *arg_vm) {
	for(
		int i = 0; 
		arg_vm->data[i] != 0 &&
		i < arg_vm->code_length;
		i++
	) {
		print_char(arg_vm->data[i]);
	}
}

static void ffi_print_num(struct vm *arg_vm) {
	int zero = 0;
	int num = get_bytes(arg_vm->data, &zero, arg_vm->mode);
	print_num(num);
}

static void ffi_move_cursor(struct vm *arg_vm) {
	int zero = 0;
	int x = get_bytes(arg_vm->data, &zero, arg_vm->mode);
	int y = get_bytes(arg_vm->data, &zero, arg_vm->mode);
	move_cursor(x, y);
}

static void ffi_print_clear(struct vm *arg_vm) {
	print_clear();
}

// Insecure!
static void ffi_allocate(struct vm *arg_vm) {
	int zero = 0;
	int num = get_bytes(arg_vm->data, &zero, arg_vm->mode);
	unsigned char *ptr = (unsigned char *) kmalloc(num);
	set_vm_reg(0, (unsigned int) &ptr, arg_vm);
}

static void ffi_free(struct vm *arg_vm) {
	int zero = 0;
	int num = get_bytes(arg_vm->data, &zero, arg_vm->mode);
	kfree((int *) num);
}

static void ffi_get_key(struct vm *arg_vm) {
	set_vm_reg(0, key_get_first(), arg_vm);
}

instruction_t vm_ffi[VM_FFI_MAX] = {
	ffi_print,
	ffi_print_num,
	ffi_move_cursor,
	ffi_print_clear,
	ffi_allocate,
	ffi_free,
	ffi_get_key
};
 
static void ffi(struct vm *arg_vm) {
	int ffi_num = VM_GET_BYTES(1);
	vm_ffi[ffi_num](arg_vm);
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

static void push(struct vm *arg_vm) {
	int value = get_bytes(
		arg_vm->data,
		&arg_vm->stack_index,
		arg_vm->mode
	);

	set_vm_reg(
		arg_vm->stack_index,
		value,
		arg_vm
	);

	arg_vm->stack_index++;
}

static void push_reg(struct vm *arg_vm) {
	int inst_reg = VM_GET_BYTES(1);
	set_vm_reg(
		arg_vm->stack_index,
		get_bytes(
			arg_vm->data,
			&inst_reg,
			arg_vm->mode
		),

		arg_vm
	);

	arg_vm->stack_index++;
}

static void pop(struct vm *arg_vm) {
	int inst_reg = VM_GET_BYTES(1);
	arg_vm->stack_index--;
	set_vm_reg(
		inst_reg,
		get_bytes(
			arg_vm->data,
			&arg_vm->stack_index,
			arg_vm->mode
		),

		arg_vm
	);
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
	set_reg,
	push,
	push_reg,
	pop
};

static void vm_parse(
	struct vm *arg_vm
) {
	if(arg_vm->code_index + 1 >= arg_vm->code_length) {
		#ifdef COMMON_DEBUG_VM
			println("vm code index too big");
		#endif
		
		arg_vm->stop = 1;
	}
	
	if(arg_vm->stop != 0) {
		#ifdef COMMON_DEBUG_VM
			println("vm stopped");
		#endif
		
		return;
	}

	int index = arg_vm->code[arg_vm->code_index];
	#ifdef COMMON_DEBUG_VM
		print("vm code index ");
		print_num(index);
		println("");
	#endif
	
	arg_vm->code_index++;
	instructions[index](arg_vm);
}

#ifdef COMMON_DEBUG_VM
	#define VM_PRINT(x) \
		print( #x ); \
		print(" "); \
		print_num(arg_vm. x ); \
		print(" "); \
		print_dec(arg_vm. x ); \
		println("")
	
	void vm_print(struct vm arg_vm) {
		println("vm_print");
		VM_PRINT(mode);
		VM_PRINT(code_length);
		VM_PRINT(code_index);
		VM_PRINT(flags);
		VM_PRINT(stop);
		VM_PRINT(step);
	}
#endif

void vm_step(struct vm *arg_vm, int steps) {
	#ifdef COMMON_DEBUG_VM
		vm_print(*arg_vm);
	#endif
	
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
	result.step = 0;
	result.stack_index = 0;
	return result;
}

int vm_index = 0;
void vm_round_robin() {
	#ifdef COMMON_DEBUG_VM
		print_clear();
		println("vm_round_robin");
		print("index ");
		print_num(vm_index);
		println("");
		print("max ");
		print_num(vm_max);
		println("");
	#endif
	
	if(
		vms[vm_index] != 0 && 
		vms[vm_index]->step > 0
	) {
		vm_step(vms[vm_index], vms[vm_index]->step);
	}
	
	vm_index++;
	if(vm_index >= vm_max) {
		vm_index = 0;
	}
}

int vm_installed = 0;
void vm_install() {
	vm_installed = 1;
}

int is_vm_installed() {
	return vm_installed;
}
