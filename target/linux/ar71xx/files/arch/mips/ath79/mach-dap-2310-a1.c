/*
 *  D-Link DAP-2310
 *
 *  Copyright (C) 2014-2015 Chris Blake <chrisrblake93@gmail.com>
 *
 *  Based on Atheros AP96 board support configuration
 *
 *  Copyright (C) 2009 Marco Porsch
 *  Copyright (C) 2009-2012 Gabor Juhos <juhosg@openwrt.org>
 *  Copyright (C) 2010 Atheros Communications
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/platform_device.h>
#include <linux/delay.h>

#include <asm/mach-ath79/ath79.h>

#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "machtypes.h"

#define DAP2310_GPIO_LED_WLAN_GREEN		14
#define DAP2310_GPIO_LED_LAN_GREEN		15
#define DAP2310_GPIO_LED_PWR_GREEN		16

#define DAP2310_GPIO_BTN_RESET		12
#define DAP2310_KEYS_POLL_INTERVAL		20	/* msecs */
#define DAP2310_KEYS_DEBOUNCE_INTERVAL	(3 * DAP2310_KEYS_POLL_INTERVAL)

#define DAP2310_CALDATA0_OFFSET            0x21000

static struct gpio_led dap2310_leds_gpio[] __initdata = {
	{
		.name		= "d-link:green:wlan",
		.gpio		= DAP2310_GPIO_LED_WLAN_GREEN,
		.active_low	= 1,
	},
	{
		.name		= "d-link:green:lan",
		.gpio		= DAP2310_GPIO_LED_LAN_GREEN,
		.active_low	= 1,
	},
	{
		.name		= "d-link:green:power",
		.gpio		= DAP2310_GPIO_LED_PWR_GREEN,
		.active_low	= 1,
	}
};

static struct gpio_keys_button dap2310_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = DAP2310_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= DAP2310_GPIO_BTN_RESET,
		.active_low	= 1,
	}
};

static void __init dap2310_setup(void)
{
	u8 *mac = (u8 *) KSEG1ADDR(0xbffd0000);
	u8 wlan_mac[ETH_ALEN];

	ath79_register_mdio(0,0x0);

	ath79_init_mac(ath79_eth0_data.mac_addr, mac, 0);
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RGMII;
	ath79_eth0_data.phy_mask = BIT(0);
	ath79_register_eth(0);

	ath79_register_m25p80(NULL);

	ath79_register_leds_gpio(-1, ARRAY_SIZE(dap2310_leds_gpio),
					dap2310_leds_gpio);
	ath79_register_gpio_keys_polled(-1, DAP2310_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(dap2310_gpio_keys),
					dap2310_gpio_keys);

	ath79_init_mac(wlan_mac, mac, 1);
	ap91_pci_init(mac + DAP2310_CALDATA0_OFFSET, wlan_mac);
}

MIPS_MACHINE(ATH79_MACH_DAP_2310_A1, "DAP-2310-A1", "D-Link DAP-2310 rev. A1", dap2310_setup);
