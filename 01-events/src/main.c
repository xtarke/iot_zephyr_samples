/**
 * @file main_2.c
 * @brief Zephyr application demonstrating button input, events, and LED toggling.
 *
 * This file contains the main application logic for reading a button press
 * via the Zephyr input subsystem, posting an event to the main thread, and 
 * toggling an LED associated with the led0 devicetree alias.
 */

#include <inttypes.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/**
 * @def LED0_NODE
 * @brief Devicetree alias for the LED.
 */
#define LED0_NODE DT_ALIAS(led0)

/**
 * @brief GPIO specification for the LED.
 * 
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/**
 * @brief Zephyr event object for button actions.
 *
 * Used to signal the main thread that a specific button action has occurred.
 */
K_EVENT_DEFINE(my_bytton_event);

/**
 * @brief Callback function triggered by input events.
 *
 * This callback listens for button events. Upon a synchronized event, it logs
 * the button code, its state (pressed or released), and the cycle count.
 * If the button is released (event value is 0), it posts the button code to 
 * `my_bytton_event` and toggles the state of the LED.
 *
 * @param evt Pointer to the input event structure.
 * @param user_data Optional user data passed to the callback (unused).
 */
static void button_input_cb(struct input_event *evt, void *user_data)
{
	if (evt->sync == 0) {
		return;
	}

	LOG_INF("Button %d %s at %u",
	       evt->code,
	       evt->value ? "pressed" : "released",
	       k_cycle_get_32());

	if (evt->value == 0){
		k_event_post(&my_bytton_event, evt->code);
		gpio_pin_toggle_dt(&led);
	}
}

/* Register the input callback */
INPUT_CALLBACK_DEFINE(NULL, button_input_cb, NULL);

/**
 * @brief Main application entry point.
 *
 * Initializes the LED GPIO pin as an active output. If initialization succeeds, 
 * it prompts the user to trigger an event. The main thread then enters an 
 * infinite loop, safely waiting for events to be posted to `my_bytton_event` 
 * and logging them as they arrive.
 *
 * @return 0 on success or initialization failure.
 */
int main(void)
{
	int ret;
	int events;
	
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	LOG_INF("Press the button to trigger an event");

	while(1){
		events = k_event_wait_safe(&my_bytton_event, 0xFFF, false, K_FOREVER);

		LOG_INF("An event has been posted: %d", events);
	}
	
	k_sleep(K_FOREVER);

	return 0;
}