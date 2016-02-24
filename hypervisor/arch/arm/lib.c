/*
 * Jailhouse, a Linux-based partitioning hypervisor
 *
 * Copyright (c) ARM Limited, 2014
 *
 * Authors:
 *  Jean-Philippe Brucker <jean-philippe.brucker@arm.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 */

#include <jailhouse/control.h>
#include <jailhouse/processor.h>
#include <jailhouse/string.h>
#include <jailhouse/types.h>
#include <asm/percpu.h>
#include <asm/sysregs.h>

int phys_processor_id(void)
{
	return this_cpu_data()->cpu_id;
}

unsigned int arm_cpu_virt2phys(struct cell *cell, unsigned int virt_id)
{
	unsigned int cpu;

	for_each_cpu(cpu, cell->cpu_set) {
		if (per_cpu(cpu)->virt_id == virt_id)
			return cpu;
	}

	return -1;
}
