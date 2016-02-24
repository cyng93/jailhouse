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

#ifndef _JAILHOUSE_ASM_PLATFORM_H
#define _JAILHOUSE_ASM_PLATFORM_H

#include <jailhouse/config.h>

#ifdef CONFIG_MACH_FOUNDATION_V8

# ifdef CONFIG_ARM_GIC_V3
#  define GICD_BASE	((void *)0x2f000000)
#  define GICD_SIZE	0x10000
#  define GICR_BASE	((void *)0x2f100000)
#  define GICR_SIZE	0x100000

#  include <asm/gic_v3.h>
# else /* GICv2 */
#  define GICD_BASE	((void *)0x2c001000)
#  define GICD_SIZE	0x1000
#  define GICC_BASE	((void *)0x2c002000)
/*
 * WARN: most device trees are broken and report only one page for the GICC.
 * It will brake the handle_irq code, since the GICC_DIR register is located at
 * offset 0x1000...
 */
#  define GICC_SIZE	0x2000
#  define GICH_BASE	((void *)0x2c004000)
#  define GICH_SIZE	0x2000
#  define GICV_BASE	((void *)0x2c006000)
#  define GICV_SIZE	0x2000

#  include <asm/gic_v2.h>
# endif /* GIC */

# define MAINTENANCE_IRQ 25
# define UART_BASE	0x1c090000

#endif /* CONFIG_MACH_FOUNDATION_V8 */

#ifdef CONFIG_MACH_AMD_SEATTLE

/* the device tree shipped with the kernel is wrong;
 * these are the corrected values */
#  define GICD_BASE	((void *)0xe1110000)
#  define GICD_SIZE	0x1000
#  define GICC_BASE	((void *)0xe112f000)
#  define GICC_SIZE	0x2000
#  define GICH_BASE	((void *)0xe1140000)
#  define GICH_SIZE	0x10000
#  define GICV_BASE	((void *)0xe116f000)
#  define GICV_SIZE	0x2000

#  include <asm/gic_v2.h>
# define MAINTENANCE_IRQ 25
# define UART_BASE	0xe1010000

#endif /* CONFIG_MACH_AMD_SEATTLE */

#endif /* !_JAILHOUSE_ASM_PLATFORM_H */
