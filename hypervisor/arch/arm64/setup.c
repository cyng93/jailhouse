/*
 * Jailhouse AArch64 support
 *
 * Copyright (C) 2015 Huawei Technologies Duesseldorf GmbH
 *
 * Authors:
 *  Antonios Motakis <antonios.motakis@huawei.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <jailhouse/entry.h>
#include <jailhouse/printk.h>
#include <asm/control.h>
#include <asm/setup.h>

int arch_init_early(void)
{
	return arch_mmu_cell_init(&root_cell);
}

int arch_cpu_init(struct per_cpu *cpu_data)
{
	/* switch to the permanent page tables */
	enable_mmu_el2(hv_paging_structs.root_table);

	return arch_mmu_cpu_cell_init(cpu_data);
}

int arch_init_late(void)
{
	return map_root_memory_regions();
}

void __attribute__((noreturn)) arch_cpu_activate_vmm(struct per_cpu *cpu_data)
{
	trace_error(-EINVAL);
	while (1);
}

int arch_map_device(void *paddr, void *vaddr, unsigned long size)
{
	return paging_create(&hv_paging_structs, (unsigned long)paddr, size,
			(unsigned long)vaddr,
			PAGE_DEFAULT_FLAGS | S1_PTE_FLAG_DEVICE,
			PAGING_NON_COHERENT);
}

int arch_unmap_device(void *vaddr, unsigned long size)
{
	return paging_destroy(&hv_paging_structs, (unsigned long)vaddr, size,
			PAGING_NON_COHERENT);
}

void arch_cpu_restore(struct per_cpu *cpu_data, int return_code)
{
	trace_error(-EINVAL);
	while (1);
}
