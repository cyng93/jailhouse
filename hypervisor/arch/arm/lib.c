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

#include <jailhouse/processor.h>
#include <jailhouse/string.h>
#include <jailhouse/types.h>
#include <asm/percpu.h>
#include <asm/sysregs.h>

int phys_processor_id(void)
{
	return this_cpu_data()->cpu_id;
}
