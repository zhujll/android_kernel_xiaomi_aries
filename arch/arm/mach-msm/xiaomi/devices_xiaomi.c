/* Copyright (c) 2012, LGE Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/memory.h>
#include <linux/persistent_ram.h>

#include <asm/setup.h>
#include <asm/sizes.h>
#include <asm/system_info.h>
#include <asm/memory.h>

#include <mach/board_xiaomi.h>

#include <ram_console.h>

#ifdef CONFIG_KEXEC_HARDBOOT
#include <linux/memblock.h>
#endif

#ifdef CONFIG_ANDROID_PERSISTENT_RAM
static struct persistent_ram_descriptor pram_descs[] = {
#ifdef CONFIG_ANDROID_RAM_CONSOLE
	{
		.name = "ram_console",
		.size = XIAOMI_RAM_CONSOLE_SIZE,
	},
#endif
};

static struct persistent_ram xiaomi_persistent_ram = {
	.size = XIAOMI_PERSISTENT_RAM_SIZE,
	.num_descs = ARRAY_SIZE(pram_descs),
	.descs = pram_descs,
};

void __init xiaomi_add_persistent_ram(void)
{
	struct persistent_ram *pram = &xiaomi_persistent_ram;
	struct membank* bank = &meminfo.bank[0];

	pram->start = bank->start + bank->size - XIAOMI_PERSISTENT_RAM_SIZE;

	persistent_ram_early_init(pram);
}
#endif

void __init xiaomi_reserve(void)
{
#ifdef CONFIG_KEXEC_HARDBOOT
	// Reserve space for hardboot page, just before the ram_console
	struct membank* bank = &meminfo.bank[0];
	phys_addr_t start = bank->start + bank->size - SZ_1M - XIAOMI_PERSISTENT_RAM_SIZE;
	int ret = memblock_remove(start, SZ_1M);
	if(!ret)
		pr_info("Hardboot page reserved at 0x%X\n", start);
	else
		pr_err("Failed to reserve space for hardboot page at 0x%X!\n", start);
#endif
	xiaomi_add_persistent_ram();
}

#ifdef CONFIG_ANDROID_RAM_CONSOLE
static struct platform_device ram_console_device = {
	.name = "ram_console",
	.id = -1
};

void __init xiaomi_add_ramconsole_devices(void)
{
	platform_device_register(&ram_console_device);
}
#endif /* CONFIG_ANDROID_RAM_CONSOLE */
