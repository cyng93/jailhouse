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

#ifndef _JAILHOUSE_ASM_CELL_H
#define _JAILHOUSE_ASM_CELL_H

#include <jailhouse/types.h>
#include <asm/spinlock.h>

#ifndef __ASSEMBLY__

#include <jailhouse/cell-config.h>
#include <jailhouse/hypercall.h>
#include <jailhouse/paging.h>

struct arch_cell {
	struct paging_structures mm;
	spinlock_t caches_lock;
	bool needs_flush;

	u64 spis;
};

extern struct cell root_cell;

#endif /* !__ASSEMBLY__ */
#endif /* !_JAILHOUSE_ASM_CELL_H */
