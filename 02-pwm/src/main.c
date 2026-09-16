/**
 * @file main.c
 * @brief Zephyr application demonstrating basic PWM output.
 *
 * This file contains the main application logic for initializing a PWM device
 * and setting it to a 50% duty cycle using a predefined period.
 */

#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/**
 * @def PWM_CHANNEL
 * @brief The PWM channel number to use for the output.
 */
#define PWM_CHANNEL   0

/**
 * @def PWM_PERIOD_NS
 * @brief The total period of the PWM signal in nanoseconds (1 millisecond).
 */
#define PWM_PERIOD_NS (1 * NSEC_PER_MSEC)

/**
 * @brief PWM devicetree specification.
 *
 * Obtains the PWM device, channel, and period specifications from the 
 * devicetree node labeled `pwm_led0`.
 */
static const struct pwm_dt_spec pwm_channel = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_led0));

/**
 * @brief Main application entry point.
 *
 * This function verifies that the PWM device is ready for use. If it is ready,
 * it configures the specified PWM channel with a 50% duty cycle (half of the 
 * total period) and normal polarity. Upon successful configuration, it logs 
 * a status message and suspends the main thread indefinitely.
 *
 * @return 0 on success, or a negative error code if the PWM configuration fails.
 */
int main(void)
{
	int ret;

	if (!device_is_ready(pwm_channel.dev)) {
		LOG_ERR("PWM device is not ready\n");
		return 0;
	}

	ret = pwm_set(pwm_channel.dev, PWM_CHANNEL, PWM_PERIOD_NS, PWM_PERIOD_NS / 2, PWM_POLARITY_NORMAL);
	if (ret < 0) {
		LOG_ERR("Failed to set pwm (%d)\n", ret);
		return ret;
	}
	
	LOG_INF("PWM sample started, T = %d ns", PWM_PERIOD_NS);

	k_sleep(K_FOREVER);

	return 0;
}