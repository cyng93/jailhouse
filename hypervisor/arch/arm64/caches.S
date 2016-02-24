/*
 * Jailhouse AArch64 support
 *
 * Copyright (C) 2016 Huawei Technologies Duesseldorf GmbH
 *
 * Authors:
 *  Antonios Motakis <antonios.motakis@huawei.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 * Implementation copied from Linux source files:
 *   - arch/arm64/mm/cache.S
 *   - arch/arm64/mm/proc-macros.S
 */

/*
 * dcache_line_size - get the minimum D-cache line size from the CTR register.
 */
	.macro	dcache_line_size, reg, tmp
	mrs	\tmp, ctr_el0			// read CTR
	ubfm	\tmp, \tmp, #16, #19		// cache line size encoding
	mov	\reg, #4			// bytes per word
	lsl	\reg, \reg, \tmp		// actual cache line size
	.endm

/*
 *	arch_paging_flush_cpu_caches(addr, size)
 *
 *	Ensure that the data held in the page addr is written back to the
 *	page in question.
 *
 *	- addr    - address
 *	- size    - size in question
 */
	.global arch_paging_flush_cpu_caches
arch_paging_flush_cpu_caches:
	dcache_line_size x2, x3
	add	x1, x0, x1
	sub	x3, x2, #1
	bic	x0, x0, x3
1:	dc	civac, x0			// clean & invalidate D line
						// unified line
	add	x0, x0, x2
	cmp	x0, x1
	b.lo	1b
	dsb	sy
	ret
