/**
 * @file wifi.txt
 * @brief Zephyr RTOS WiFi scanning and LED blinking demonstration.
 *
 * Copyright (c) 2025 John Lin
 * SPDX-License-Identifier: Apache-2.0
 * 
 * 
 * Added logs and doxgyen by Renan Augusto Starke
 * 
 */

/* Zephyr includes */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/**
 * @def LED_NODE
 * @brief Devicetree alias for the LED node (led0).
 */
#define LED_NODE DT_ALIAS(led0)

/** 
 * @brief GPIO specification for the LED, derived from the devicetree. 
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

/** 
 * @brief Network management event callback structure for WiFi events. 
 */
static struct net_mgmt_event_callback wifi_cb;

/** 
 * @brief Semaphore used to signal the completion of a WiFi scan. 
 */
static K_SEM_DEFINE(scan_done, 0, 1);

/**
 * @brief Callback function to handle WiFi network management events.
 *
 * This function processes both individual scan results and the scan 
 * completion event, logging the details to the console.
 *
 * @param cb Pointer to the network management event callback structure.
 * @param mgmt_event The network management event type being handled.
 * @param iface Pointer to the network interface triggering the event.
 */
static void wifi_scan_result(struct net_mgmt_event_callback *cb, uint64_t mgmt_event,
			     struct net_if *iface)
{
	if (mgmt_event == NET_EVENT_WIFI_SCAN_DONE) {
		LOG_INF("WiFi scan complete\n");
		k_sem_give(&scan_done);
	} else if (mgmt_event == NET_EVENT_WIFI_SCAN_RESULT) {
		const struct wifi_scan_result *entry =
			(const struct wifi_scan_result *)cb->info;
		LOG_INF("  SSID: %-32s CH: %2d RSSI: %d",
		       entry->ssid, entry->channel, entry->rssi);
	}
}

/**
 * @brief Thread function to continuously blink the LED.
 *
 * Toggles the configured LED pin every 500 milliseconds.
 *
 * @param p1 Unused thread parameter 1.
 * @param p2 Unused thread parameter 2.
 * @param p3 Unused thread parameter 3.
 */
static void led_thread_fn(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	while (1) {
		gpio_pin_toggle_dt(&led);
		k_sleep(K_MSEC(500));
	}
}

/**
 * @brief Define and initialize the LED blinking thread.
 */
K_THREAD_DEFINE(led_thread, 512, led_thread_fn, NULL, NULL, NULL, 7, 0, 0);

/**
 * @brief Application entry point.
 *
 * Initializes the LED GPIO, waits for the WiFi interface to be ready, 
 * registers network event callbacks, and initiates a WiFi scan.
 *
 * @return int Returns 0 on success or error.
 */
int main(void)
{
	int ret;

	LOG_INF("Pico W WiFi + LED Demo\n");

	if (!gpio_is_ready_dt(&led)) {
		LOG_INF("LED device not ready\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		LOG_INF("Failed to configure LED: %d\n", ret);
		return 0;
	}

	LOG_INF("LED blinking started\n");

	/* Wait for WiFi to be ready */
	k_sleep(K_SECONDS(3));

	struct net_if *iface = net_if_get_default();

	if (!iface) {
		LOG_ERR("No network interface\n");
		return 0;
	}

	net_mgmt_init_event_callback(&wifi_cb, wifi_scan_result,
				     NET_EVENT_WIFI_SCAN_RESULT |
				     NET_EVENT_WIFI_SCAN_DONE);
	net_mgmt_add_event_callback(&wifi_cb);

	printk("Starting WiFi scan...\n");
	ret = net_mgmt(NET_REQUEST_WIFI_SCAN, iface, NULL, 0);
	if (ret) {
		LOG_ERR("WiFi scan request failed: %d\n", ret);
		return 0;
	}

	k_sem_take(&scan_done, K_SECONDS(15));
	LOG_INF("Demo complete - LED continues blinking\n");
	return 0;
}