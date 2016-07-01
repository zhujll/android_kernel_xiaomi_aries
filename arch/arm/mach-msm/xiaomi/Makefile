subdir-ccflags-$(CONFIG_ARCH_MSM) += -Iarch/arm/mach-msm

-include $(src)/Makefile.board

obj-$(CONFIG_MACH_XIAOMI) += devices_xiaomi.o
obj-$(CONFIG_UPDATE_LCDC_LUT) += xiaomi_qc_lcdc_luts.o
obj-$(CONFIG_LCD_KCAL) += xiaomi_kcal_ctrl.o
obj-$(CONFIG_XIAOMI_KEXEC_HARDBOOT_SYSNODE) += xiaomi_kexec_hardboot_sys_node.o

CFLAGS_devices_xiaomi.o += -Idrivers/staging/android
