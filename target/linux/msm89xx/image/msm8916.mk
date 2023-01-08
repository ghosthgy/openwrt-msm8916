# SPDX-License-Identifier: GPL-2.0-only

define Device/longcheer-l8150
	DEVICE_VENDOR := Longcheer
	DEVICE_MODEL := L8150
	SOC := msm8916
	CMDLINE := "earlycon console=ttyMSM0,115200 root=/dev/mmcblk0p31 rw rootwait"
endef
TARGET_DEVICES += longcheer-l8150
