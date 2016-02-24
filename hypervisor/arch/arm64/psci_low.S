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

#include <asm/head.h>
#include <asm/psci.h>

	.globl smc
	/*
	 * Since we trap all SMC instructions, it may be useful to forward them
	 * when it isn't a PSCI call. The shutdown code will also have to issue
	 * a real PSCI_OFF call on secondary CPUs.
	 */
smc:
	smc	#0
	ret

	.global _psci_cpu_off
	/* x0: struct psci_mbox* */
_psci_cpu_off:
	ldr	x2, =PSCI_INVALID_ADDRESS
	/* Clear mbox */
	str	x2, [x0]

	/* Wait for a CPU_ON call that updates the mbox */
1:	wfe
	ldr	x3, [x0]
	cmp	x3, #PSCI_INVALID_ADDRESS
	b.eq	1b

	/* Jump to the requested entry, with a parameter */
	ldr	x0, [x0, #8]
	br	x3
	ret

	.global _psci_cpu_on
	/* x0: struct psci_mbox*, x1: entry, x2: context */
_psci_cpu_on:
1:	ldxp	x4, x5, [x0]
	cmp	x4, #PSCI_INVALID_ADDRESS
	b.ne	store_failed
	stxp	w7, x1, x2, [x0]
	cbnz	w7, 1b

	dsb	ishst
	sev

	mov	x0, #0
	ret

store_failed:
	mov	x0, #PSCI_ALREADY_ON
	ret

	.global _psci_suspend_return
_psci_suspend_return:
	ret
