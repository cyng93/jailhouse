#
# Jailhouse AArch64 support
#
# Copyright (C) 2015 Huawei Technologies Duesseldorf GmbH
#
# Authors:
#  Antonios Motakis <antonios.motakis@huawei.com>
#
# This work is licensed under the terms of the GNU GPL, version 2.  See
# the COPYING file in the top-level directory.
#

include $(CONFIG_MK)

KBUILD_AFLAGS := $(subst -include asm/unified.h,,$(KBUILD_AFLAGS))

always := built-in.o

obj-y := entry.o setup.o control.o mmio.o caches.o
obj-y += ../arm/mmu_cell.o ../arm/paging.o ../arm/dbg-write.o ../arm/lib.o

obj-$(CONFIG_SERIAL_AMBA_PL011) += ../arm/dbg-write-pl011.o
