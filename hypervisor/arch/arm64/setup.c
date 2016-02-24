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
#include <asm/irqchip.h>
#include <asm/setup.h>
#include <asm/smp.h>

int arch_init_early(void)
{
	int err = 0;

	err = arch_mmu_cell_init(&root_cell);
	if (err)
		return err;

	return irqchip_init();
}

int arch_cpu_init(struct per_cpu *cpu_data)
{
	int err = 0;
	unsigned long hcr = HCR_VM_BIT | HCR_IMO_BIT | HCR_FMO_BIT
				| HCR_TSC_BIT | HCR_TAC_BIT | HCR_RW_BIT;

	/* switch to the permanent page tables */
	enable_mmu_el2(hv_paging_structs.root_table);

	cpu_data->psci_mbox.entry = 0;
	cpu_data->virt_id = cpu_data->cpu_id;
	arm_read_sysreg(MPIDR_EL1, cpu_data->mpidr.val);

	/* Setup guest traps */
	arm_write_sysreg(HCR_EL2, hcr);

	err = arch_mmu_cpu_cell_init(cpu_data);
	if (err)
		return err;

	return irqchip_cpu_init(cpu_data);
}

int arch_init_late(void)
{
	int err;

	/* Setup the SPI bitmap */
	err = irqchip_cell_init(&root_cell);
	if (err)
		return err;

	return map_root_memory_regions();
}

void __attribute__((noreturn)) arch_cpu_activate_vmm(struct per_cpu *cpu_data)
{
	struct registers *regs = guest_regs(cpu_data);

	/* return to the caller in Linux */
	arm_write_sysreg(ELR_EL2, regs->usr[30]);

	vmreturn(regs);
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

/* disable the hypervisor on the current CPU */
void arch_shutdown_self(struct per_cpu *cpu_data)
{
	irqchip_cpu_shutdown(cpu_data);

	/* Free the guest */
	arm_write_sysreg(HCR_EL2, HCR_RW_BIT);
	arm_write_sysreg(VTCR_EL2, VTCR_RES1);

	/* Remove stage-2 mappings */
	arch_cpu_tlb_flush(cpu_data);

	/* TLB flush needs the cell's VMID */
	isb();
	arm_write_sysreg(VTTBR_EL2, 0);

	/* we will restore the root cell state with the MMU turned off,
	 * so we need to make sure it has been commited to memory */
	arch_paging_flush_cpu_caches(guest_regs(cpu_data),
				     sizeof(struct registers));
	dsb(ish);

	/* Return to EL1 */
	arch_shutdown_mmu(cpu_data);
}

void arch_cpu_restore(struct per_cpu *cpu_data, int return_code)
{
	struct registers *regs = guest_regs(cpu_data);

	/* Jailhouse initialization failed; return to the caller in EL1 */
	arm_write_sysreg(ELR_EL2, regs->usr[30]);

	regs->usr[0] = return_code;

	arch_shutdown_self(cpu_data);
}
