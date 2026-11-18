/**
 * @file main.c
 * @brief RP Pico W static Wi-Fi sample application using Zephyr OS.
 *
 * @details This file contains the implementation for connecting a board
 * (like the Raspberry Pi Pico W) to a Wi-Fi network using static credentials
 * defined in the project configuration. It utilizes Zephyr's network
 * management API and synchronization primitives to manage connection state.
 * After that, the applications sends data to Tago.IO using http post.
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

/* Http includes */
#include <zephyr/net/http/client.h>

/* Local includes */
#include "tago_post.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/**
 * @brief Macro defining the relevant network events to listen for.
 *
 * Mask includes Layer 4 (L4) connected and disconnected events.
 */
#define EVENT_MASK (NET_EVENT_L4_CONNECTED | NET_EVENT_L4_DISCONNECTED)

/**
 * @brief Callback structure for network management events.
 */
static struct net_mgmt_event_callback mgmt_cb;

/* Declare the variable for the network configuration parameters */
static struct wifi_connect_req_params cnx_params;

/**
 * @brief Flag indicating the current Wi-Fi connection status.
 *
 * True if connected to the network, false otherwise.
 */
static bool connected;

/**
 * @brief Semaphore used to block the application until a network connection is established.
 */
static K_SEM_DEFINE(run_app, 0, 1);

/**
 * @brief Callback handler for network management events.
 *
 * @details Handles Layer 4 (L4) connection and disconnection events. It updates
 * the `connected` flag and signals the `run_app` semaphore upon a successful
 * connection, or resets the semaphore upon disconnection.
 *
 * @param cb Pointer to the network management event callback structure.
 * @param mgmt_event The specific network management event that triggered the callback.
 * @param iface Pointer to the network interface where the event occurred.
 */
static void net_mgmt_event_handler(struct net_mgmt_event_callback *cb,
			  uint64_t mgmt_event, struct net_if *iface)
{	
	if ((mgmt_event & EVENT_MASK) != mgmt_event) {
		return;
	}

	if (mgmt_event == NET_EVENT_L4_CONNECTED) {
		LOG_INF("Network connected");
		connected = true;
		k_sem_give(&run_app);
		return;
	}
	if (mgmt_event == NET_EVENT_L4_DISCONNECTED) {
		if (connected == false) {
			LOG_INF("Waiting for network to be connected");
		} else {
			LOG_INF("Network disconnected");
			connected = false;
		}
		k_sem_reset(&run_app);
		return;
	}
}


/**
 * @brief Populates the Wi-Fi connection request parameters.
 *
 * @details Extracts the SSID and password from the Zephyr Kconfig definitions
 * (`CONFIG_WIFI_CREDENTIALS_STATIC_SSID` and `CONFIG_WIFI_CREDENTIALS_STATIC_PASSWORD`)
 * and populates the remaining connection parameters with generic defaults.
 *
 * @param params Pointer to the Wi-Fi connection request parameters structure to populate.
 * @return int Returns 0 on successful population.
 */
static int write_wifi_parameters(struct wifi_connect_req_params *params)
{

	/* Populate the SSID and password from prj.conf */
	params->ssid = CONFIG_WIFI_CREDENTIALS_STATIC_SSID;
	params->ssid_length = strlen(params->ssid);

	params->psk = CONFIG_WIFI_CREDENTIALS_STATIC_PASSWORD;
	params->psk_length = strlen(params->psk);

	/* Populate the rest of the relevant members */
	params->channel = WIFI_CHANNEL_ANY;
	params->security = WIFI_SECURITY_TYPE_PSK;
	params->mfp = WIFI_MFP_OPTIONAL;
	params->timeout = SYS_FOREVER_MS;
	params->band = WIFI_FREQ_BAND_UNKNOWN;
	memset(params->bssid, 0, sizeof(params->bssid));

	return 0;
}

/**
 * @brief Initializes the Wi-Fi driver and attempts connection.
 *
 * @details Sets up the necessary callbacks for monitoring Wi-Fi status, retrieves
 * the default Wi-Fi network interface, provisions it with static credentials, and
 * dispatches connection requests via `net_mgmt()`. The function polls every 5 seconds
 * if the connection fails and will ultimately block indefinitely until `run_app` is given.
 *
 * @return int Returns -1 if no Wi-Fi interface is found. Otherwise, blocks until connection.
 */
static int setup_wifi(){

	LOG_INF("Initializing Wi-Fi driver");
	/* Sleep to allow initialization of Wi-Fi driver */
	k_sleep(K_SECONDS(1));

	/* Initialize and add the callback function for network events */
	net_mgmt_init_event_callback(&mgmt_cb, net_mgmt_event_handler, EVENT_MASK);
	net_mgmt_add_event_callback(&mgmt_cb);

	/* Get the network interface */
	struct net_if *iface = net_if_get_first_wifi();
	if (iface == NULL) {
		LOG_ERR("Returned network interface is NULL");
		return -1;
	}

	/* Populate cnx_params with the network configuration */
	write_wifi_parameters(&cnx_params);

	/* Call net_mgmt() to request the Wi-Fi connection */
	int err;

	/* Try to connect */
	do {
		LOG_INF("Connecting to WiFi...");
		err = net_mgmt(NET_REQUEST_WIFI_CONNECT, iface, &cnx_params, sizeof(struct wifi_connect_req_params));

		if (err) {
			LOG_ERR("Connecting to Wi-Fi failed, err: %d", err);
			LOG_ERR("New try in 5s.");
			k_sleep(K_SECONDS(5));
		}
	} while (err != 0);

	LOG_INF("Waiting to connect!");

	/* Wait forever for a wifi connection.
	 * Unblocked by the callback funcion */
	k_sem_take(&run_app, K_FOREVER);

	return 0;
}


static int response_cb(struct http_response *rsp,
		       enum http_final_call final_data,
		       void *user_data)
{
	if (final_data == HTTP_DATA_MORE) {
		LOG_DBG("Partial data received (%zd bytes)", rsp->data_len);
	} else if (final_data == HTTP_DATA_FINAL) {
		LOG_DBG("All the data received (%zd bytes)", rsp->data_len);
	}

	LOG_DBG("Response status %s", rsp->http_status);

	return 0;
}



/**
 * @brief Main application entry point.
 *
 * @details Calls the Wi-Fi setup routine and then enters an infinite loop,
 * outputting a heartbeat message periodically to standard output.
 *
 * @return int Main exit status (does not normally return).
 */
int main(void)
{
	int temperature = 0;
	
	static struct tagoio_context ctx;

	LOG_INF("TagoIO post sample.");

	/* Try to connect to Wifi. Ths function does not return
	 * until a wifi connection is stablished.
	*/
	setup_wifi();


	int post = 3;
	while (post) {
		k_msleep(1000*10);
		LOG_INF("Posting data.");

		snprintf(ctx.payload, sizeof(ctx.payload),
		       "{\"variable\": \"temperature\","
		       "\"unit\": \"c\",\"value\": %d}",
		       temperature);

		if (tagoio_connect(&ctx) < 0) {
			LOG_ERR("No connection available.");
			continue;
		}
		
		if (tagoio_http_push(&ctx, response_cb) < 0) {
			LOG_ERR("Error pushing data.");
			continue;
		}

		LOG_INF("Post successful...");

		temperature += 2;	
		post--;
	}


	return 0;
}
