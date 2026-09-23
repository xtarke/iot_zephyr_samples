/**
 * @file main.c
 * @brief Main application for reading BMP280 sensor data using Zephyr RTOS.
 *
 * This file contains the main application that initializes the BMP280 sensor 
 * and continuously fetches ambient temperature and atmospheric pressure data 
 * at a fixed interval.
 */

/* C includes */
#include <stdio.h>

/* Zephyr includes */
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

/** 
 * @brief Register the logging module for the main application. 
 */
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/**
 * @brief Pointer to the BMP280 device structure.
 * 
 * This device instance is retrieved at build time using the Device Tree 
 * node label 'bmp280'.
 */
static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(bmp280));

/**
 * @brief Main application entry point.
 *
 * This function verifies if the BMP280 sensor is ready for operation. 
 * Once verified, it enters an infinite loop where it periodically fetches 
 * sensor samples (temperature and pressure) every 5 seconds and logs 
 * them to the console.
 *
 * @return Returns standard error codes (e.g., -EIO) if device initialization fails.
 */
int main(void)
{
	int ret;
	struct sensor_value temp, press;

	if (!device_is_ready(dev)) {
		LOG_ERR("Error: Device \"%s\" is not ready; "
		       "check the driver initialization logs for errors.",
		       dev->name);
		return -EIO;
	}

	LOG_INF("Sample using device %s starting.", dev->name);

	while (1) {
		ret = sensor_sample_fetch(dev);

		/* Check if fetch is okay */
		if (ret < 0){
			LOG_ERR("Error fetching %s data.", dev->name);
			continue;
		}

		sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
		sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
		
		/* Zephyr sensor API: *
	 	* int32_t val1 Integer part of the value.
	 	* int32_t val2 Fractional part of the value (in one-millionth parts). */

		LOG_INF("T: %d.%06d; P: %d.%06d",
				temp.val1, temp.val2, press.val1, press.val2);

		k_sleep(K_MSEC(5000));
	}
}