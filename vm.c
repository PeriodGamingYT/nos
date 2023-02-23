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
		panic("can't have any more vms!");
	}

	vms[i] = arg_vm;
	return i;
}

static void vm_parse(
	unsigned char *data,
	int length,
	struct vm *arg_vm
) {
	for(int i = 0; i < length; i++) {
		// TODO
	}
}

void vm_feed(unsigned char *data, int length, int index) {
	if(
		index < 0 ||
		index > VM_MAX ||
		vms[index] == 0
	) {
		return;
	}

	vm_parse(data, length, vms[index]);
}
