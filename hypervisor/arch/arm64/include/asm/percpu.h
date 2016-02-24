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

#ifndef _JAILHOUSE_ASM_PERCPU_H
#define _JAILHOUSE_ASM_PERCPU_H

#include <jailhouse/types.h>
#include <asm/paging.h>

#ifndef __ASSEMBLY__

#include <asm/cell.h>
#include <asm/spinlock.h>

union mpidr {
	u64 val;
	struct {
		u8 aff0;
		u8 aff1;
		u8 aff2;
		u8 pad1;
		u8 aff3;
		u8 pad2[3];
	} f;
};

struct per_cpu {
	/* common fields */
	unsigned int cpu_id;
	struct cell *cell;
	u32 stats[JAILHOUSE_NUM_CPU_STATS];
	int shutdown_state;
	bool failed;

	bool flush_vcpu_caches;
	union mpidr mpidr;
} __attribute__((aligned(PAGE_SIZE)));

static inline struct per_cpu *this_cpu_data(void)
{
	while (1);
	return NULL;
}

#define DEFINE_PER_CPU_ACCESSOR(field)					\
static inline typeof(((struct per_cpu *)0)->field) this_##field(void)	\
{									\
	return this_cpu_data()->field;					\
}

DEFINE_PER_CPU_ACCESSOR(cpu_id)
DEFINE_PER_CPU_ACCESSOR(cell)

static inline struct per_cpu *per_cpu(unsigned int cpu)
{
	while (1);
	return NULL;
}

static inline unsigned int arm_cpu_phys2virt(unsigned int cpu_id)
{
	return per_cpu(cpu_id)->virt_id;
}

unsigned int arm_cpu_virt2phys(struct cell *cell, unsigned int virt_id);

/* Validate defines */
#define CHECK_ASSUMPTION(assume)	((void)sizeof(char[1 - 2*!(assume)]))

static inline void __check_assumptions(void)
{
	CHECK_ASSUMPTION(sizeof(unsigned long) == (8));
}
#endif /* !__ASSEMBLY__ */

#endif /* !_JAILHOUSE_ASM_PERCPU_H */
