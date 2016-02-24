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

#include <jailhouse/control.h>
#include <jailhouse/printk.h>
#include <asm/control.h>
#include <asm/irqchip.h>
#include <asm/platform.h>
#include <asm/traps.h>

int arch_cell_create(struct cell *cell)
{
	return trace_error(-EINVAL);
}

void arch_flush_cell_vcpu_caches(struct cell *cell)
{
	/* AARCH64_TODO */
	trace_error(-EINVAL);
}

void arch_cell_destroy(struct cell *cell)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_config_commit(struct cell *cell_added_removed)
{
}

void arch_shutdown(void)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_suspend_cpu(unsigned int cpu_id)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_resume_cpu(unsigned int cpu_id)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_reset_cpu(unsigned int cpu_id)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_park_cpu(unsigned int cpu_id)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_shutdown_cpu(unsigned int cpu_id)
{
	trace_error(-EINVAL);
	while (1);
}

void __attribute__((noreturn)) arch_panic_stop(void)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_panic_park(void)
{
	trace_error(-EINVAL);
	while (1);
}

void arch_handle_sgi(struct per_cpu *cpu_data, u32 irqn)
{
	cpu_data->stats[JAILHOUSE_CPU_STAT_VMEXITS_MANAGEMENT]++;

	switch (irqn) {
	case SGI_INJECT:
		irqchip_inject_pending(cpu_data);
		break;
	default:
		printk("WARN: unknown SGI received %d\n", irqn);
	}
}

/*
 * Handle the maintenance interrupt, the rest is injected into the cell.
 * Return true when the IRQ has been handled by the hyp.
 */
bool arch_handle_phys_irq(struct per_cpu *cpu_data, u32 irqn)
{
	if (irqn == MAINTENANCE_IRQ) {
		cpu_data->stats[JAILHOUSE_CPU_STAT_VMEXITS_MAINTENANCE]++;

		irqchip_inject_pending(cpu_data);
		return true;
	}

	cpu_data->stats[JAILHOUSE_CPU_STAT_VMEXITS_VIRQ]++;

	irqchip_set_pending(cpu_data, irqn, true);

	return false;
}
