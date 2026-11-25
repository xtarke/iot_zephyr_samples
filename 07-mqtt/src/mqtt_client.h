/**
 * @file mqtt_client.h
 * @brief MQTT client interface for initializing, connecting, subscribing, and publishing.
 *
 * @copyright Copyright (c) 2024 Analog Devices, Inc.
 * @author Modified by Renan Augusto Starke, 2026.
 * @license SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

/** MQTT connection timeouts */
#define MSECS_NET_POLL_TIMEOUT	5000
#define MSECS_WAIT_RECONNECT	1000


/**
 * @brief Global MQTT connection status flag.
 *
 * Indicates whether the client is currently connected to the MQTT broker (`true`)
 * or disconnected (`false`).
 */
extern bool mqtt_connected;

/**
 *  @brief Initialise the MQTT client & broker configuration
 */
int app_mqtt_init(struct mqtt_client *client);

/**
 *  @brief Blocking function that establishes connectivity to the MQTT broker
 */
void app_mqtt_connect(struct mqtt_client *client);

/**
 *  @brief  Subscribes to user-defined MQTT topics and continuously
 *          processes incoming data while the MQTT connection is active
 */
void app_mqtt_run(struct mqtt_client *client);

/**
 *  @brief   Subscribe to user-defined MQTT topics
 */
int app_mqtt_subscribe(struct mqtt_client *client);

/**
 * @brief Publish an MQTT payload to a specified topic.
 *
 * @param[in,out] client       Pointer to a connected @c mqtt_client instance.
 * @param[in]     topic        Null-terminated string specifying the target MQTT topic.
 * @param[in]     payload_data Pointer to the data buffer to be published.
 *
 * @return 0 on success, or a negative error code on failure.
 */
int app_mqtt_publish(struct mqtt_client *client, const char *topic, uint8_t *payload_data);

/**
 * @brief Callback function pointer type for received MQTT messages.
 *
 * @param[in] topic        Null-terminated string containing the topic name on which
 *                         the message was received.
 * @param[in] payload_data Pointer to the received message payload buffer.
 */
typedef void (*MQTT_msg_received_callback)(const char *topic, uint8_t *payload_data);

/**
 * @brief Register a callback function for incoming MQTT messages.
 *
 * Sets the user-defined handler that will be invoked whenever a message is received
 * on a subscribed topic.
 *
 * @param[in] cb Function pointer matching the @ref MQTT_msg_received_callback signature.
 */
void register_MQTT_sub_callback(MQTT_msg_received_callback cb);


#endif /* __MQTT_CLIENT_H__ */
