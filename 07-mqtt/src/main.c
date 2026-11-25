/**
 * @file main.c
 * @brief Main application entry point for the Zephyr Wi-Fi and MQTT sample.
 *
 * Initializes Wi-Fi connectivity, configures the MQTT client, schedules periodic
 * message publishing via the system work queue, and processes incoming MQTT events.
 */

/* C includes */
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* Zephyr includes */
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#include <zephyr/logging/log.h>

/* Wifi header files */
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>
#include <zephyr/net/wifi_credentials.h>

/* MQTT includes */
#include <zephyr/net/mqtt.h>

/* Local includes */
#include "wifi.h"
#include "mqtt_client.h"

/* MQTT client struct */
static struct mqtt_client client_ctx;

/* MQTT publish work item */
struct k_work_delayable mqtt_publish_work;


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/**
 * @brief Work handler for periodic MQTT publishing.
 *
 * Executed by the Zephyr system work queue. If the MQTT client is connected
 * (@ref mqtt_connected is `true`), publishes a `"Hello!"` message to the topic
 * defined by `CONFIG_NET_SAMPLE_MQTT_PUB_TOPIC` and reschedules itself according
 * to `CONFIG_NET_SAMPLE_MQTT_PUBLISH_INTERVAL`. Otherwise, cancels the delayable work.
 *
 * @param[in] work Pointer to the @c k_work structure associated with @ref mqtt_publish_work.
 */
static void publish_work_handler(struct k_work *work)
{
	int rc;

	if (mqtt_connected) {
		rc = app_mqtt_publish(&client_ctx, CONFIG_NET_SAMPLE_MQTT_PUB_TOPIC, "Hello!");
		if (rc != 0) {
			LOG_INF("MQTT Publish failed [%d]", rc);
		}
		k_work_reschedule(&mqtt_publish_work,
					K_SECONDS(CONFIG_NET_SAMPLE_MQTT_PUBLISH_INTERVAL));
	} else {
		k_work_cancel_delayable(&mqtt_publish_work);
	}
}

/**
 * @brief Callback function invoked when an MQTT message is received.
 *
 * Logs the topic and payload data of incoming messages on subscribed topics.
 * Registered via @ref register_MQTT_sub_callback.
 *
 * @param[in] topic        Null-terminated string containing the topic name.
 * @param[in] payload_data Pointer to the received payload buffer.
 */
void mqtt_event(const char *topic, uint8_t *payload_data) {
    
	LOG_INF("On MQTT msg callback!");
	LOG_INF("Topic: %s.", topic);
	LOG_INF("Paylod: %s.", payload_data);
}


/**
 * @brief Main application entry point.
 *
 * Establishes Wi-Fi connectivity via @ref setup_wifi, initializes the MQTT client
 * context, initializes the periodic publish work item, and registers the incoming
 * message callback. Enters a loop that connects to the MQTT
 * broker, starts periodic publishing, and runs the MQTT event loop.
 *
 * @return 0 on normal completion, or a negative error code if MQTT initialization fails.
 */
int main(void)
{
	int rc;
	
	LOG_INF("MQTT sample.");

	/* Try to connect to Wifi. Ths function does not return
	 * until a wifi connection is stablished.
	*/
	setup_wifi();

	rc = app_mqtt_init(&client_ctx);
	if (rc != 0) {
		LOG_ERR("MQTT Init failed [%d]", rc);
		return rc;
	}

	k_work_init_delayable(&mqtt_publish_work, publish_work_handler);


	register_MQTT_sub_callback(mqtt_event);

	while (1) {
		/* Block until MQTT connection is up */
		app_mqtt_connect(&client_ctx);
		

		/* We are now connected, begin queueing periodic MQTT publishes */
		k_work_reschedule(&mqtt_publish_work,
					K_SECONDS(CONFIG_NET_SAMPLE_MQTT_PUBLISH_INTERVAL));

		/* Handle MQTT inputs and connection */
		app_mqtt_run(&client_ctx);
	}


	return 0;
}
