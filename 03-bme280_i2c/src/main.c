/*
 * Copyright (c) 2018 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/sys/printk.h>
#include <stdio.h>

int main(void)
{
	const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(ambient0));
	struct sensor_value temp, press, humidity;

	printf("Device %p name is %s\n", dev, dev->name);

	if (!device_is_ready(dev)) {
		printk("\nError: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.\n",
		       dev->name);
		//return NULL;
	}

	while (1) {
		k_sleep(K_MSEC(5000));

		sensor_sample_fetch(dev);
		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
		sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &humidity);
		
		printf("T: %d.%06d; P: %d.%06d; H: %d.%06d;\n",
				temp.val1, temp.val2, press.val1, press.val2,
				humidity.val1, humidity.val2);
	}
}
