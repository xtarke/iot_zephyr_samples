/**
 * @file tago_post.c
 * @brief HTTP POST implementation for transmitting telemetry data to the TagoIO cloud platform on Zephyr RTOS[cite: 1].
 * @author Gerson Fernando Budke <nandojve@gmail.com>[cite: 1]
 * @copyright Copyright (c) 2020 Gerson Fernando Budke[cite: 1]
 * @license Apache-2.0[cite: 1]
 *
 * @details This module manages the network lifecycle for sending JSON payloads to the 
 *          TagoIO HTTP data endpoint[cite: 1]. It handles DNS resolution with retry mechanisms,
 *          POSIX socket creation for IPv4/IPv6 configurations, and HTTP POST transmission[cite: 1].
 */

#include <zephyr/logging/log.h>
#include <zephyr/posix/netinet/in.h>
#include <zephyr/posix/sys/socket.h>
#include <zephyr/posix/arpa/inet.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/posix/poll.h>
#include <zephyr/posix/netdb.h>

#include <zephyr/net/net_ip.h>
#include <zephyr/net/socket.h>
#include <zephyr/net/socketutils.h>
#include <zephyr/net/dns_resolve.h>
#include <zephyr/net/tls_credentials.h>
#include <zephyr/net/http/client.h>

#include "tago_post.h"

LOG_MODULE_DECLARE(tagoio_http_post, LOG_LEVEL_INF);

/** 
 * @def CONFIG_TAGOIO_DEVICE_TOKEN
 * @brief Unique device authorization token assigned by TagoIO for API authentication[cite: 1].
 */
#define CONFIG_TAGOIO_DEVICE_TOKEN "99ea1253-734f-4721-8e59-6a199b8ca70d"

/** 
 * @def TAGOIO_SERVER
 * @brief Fully qualified domain name (FQDN) for the TagoIO API server[cite: 1].
 */
#define TAGOIO_SERVER "api.tago.io"

/** 
 * @def TAGOIO_URL
 * @brief Target API HTTP endpoint path for posting data payloads[cite: 1].
 */
#define TAGOIO_URL          "/data"

/** 
 * @def HTTP_PORT
 * @brief Standard HTTP TCP port number string[cite: 1].
 */
#define HTTP_PORT           "80"

/** 
 * @def CONFIG_TAGOIO_HTTP_CONN_TIMEOUT
 * @brief Timeout duration in seconds for HTTP client request connection[cite: 1].
 */
#define CONFIG_TAGOIO_HTTP_CONN_TIMEOUT 10

/**
 * @brief HTTP header fields required for TagoIO API POST requests[cite: 1].
 *
 * Contains authorization token, content type specification, and SSL flag headers[cite: 1].
 * Terminated by a NULL pointer as expected by the HTTP client library[cite: 1].
 */
static const char *tagoio_http_headers[] = {
	"Device-Token: " CONFIG_TAGOIO_DEVICE_TOKEN "\r\n",
	"Content-Type: application/json\r\n",
	"_ssl: false\r\n",
	NULL
};

/**
 * @brief Resolves TagoIO server IP address and establishes a TCP socket connection[cite: 1].
 *
 * Performs DNS resolution for @ref TAGOIO_SERVER with up to 3 retries (5 seconds delay between attempts)[cite: 1].
 * Automatically configures IPv4 or IPv6 address family based on active network configuration[cite: 1].
 * Creates a stream socket and connects to the target server[cite: 1].
 *
 * @param[in,out] ctx Pointer to the @ref tagoio_context structure. Upon success, the 
 *                    established socket descriptor is stored in `ctx->sock`[cite: 1].
 *
 * @return 0 on success[cite: 1].
 * @return Negative standard errno code (`-errno`) on socket creation or connection failure[cite: 1].
 * @return Negative `getaddrinfo` error code on DNS resolution failure[cite: 1].
 */
int tagoio_connect(struct tagoio_context *ctx)
{
	struct addrinfo *addr;
	struct addrinfo hints;
	char hr_addr[INET6_ADDRSTRLEN];
	char *port;
	int dns_attempts = 3;
	int ret = -1;

	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (IS_ENABLED(CONFIG_NET_IPV6)) {
		hints.ai_family = AF_INET6;
	} else if (IS_ENABLED(CONFIG_NET_IPV4)) {
		hints.ai_family = AF_INET;
	}
	port = HTTP_PORT;

	while (dns_attempts--) {
		ret = getaddrinfo(TAGOIO_SERVER, port, &hints, &addr);
				
		if (ret == 0) {
			break;
		}
		k_sleep(K_SECONDS(5));
	}

	if (ret < 0) {
		LOG_ERR("Could not resolve dns, error: %d", ret);
		return ret;
	}

	LOG_DBG("%s address: %s",
		(addr->ai_family == AF_INET ? "IPv4" : "IPv6"),
		net_addr_ntop(addr->ai_family,
			      &net_sin(addr->ai_addr)->sin_addr,
			      hr_addr, sizeof(hr_addr)));

	ctx->sock = socket(hints.ai_family,
			   hints.ai_socktype,
			   hints.ai_protocol);
	if (ctx->sock < 0) {
		LOG_ERR("Failed to create %s HTTP socket (%d)",
			(addr->ai_family == AF_INET ? "IPv4" : "IPv6"),
			-errno);

		freeaddrinfo(addr);
		return -errno;
	}

	if (connect(ctx->sock, addr->ai_addr, addr->ai_addrlen) < 0) {
		LOG_ERR("Cannot connect to %s remote (%d)",
			(addr->ai_family == AF_INET ? "IPv4" : "IPv6"),
			-errno);

		freeaddrinfo(addr);
		return -errno;
	}

	freeaddrinfo(addr);

	return 0;
}

/**
 * @brief Transmits stored JSON payload data via HTTP POST request to TagoIO[cite: 1].
 *
 * Prepares the Zephyr HTTP request structure with host info, headers, and payload from `ctx`[cite: 1].
 * Sends the request over the open socket, invokes the response callback, and closes the socket[cite: 1].
 *
 * @param[in,out] ctx Pointer to the @ref tagoio_context containing valid `sock`, `payload`, 
 *                    and `resp` receive buffer[cite: 1]. Socket is closed and set to `-1` upon return[cite: 1].
 * @param[in] resp_cb Callback function of type `http_response_cb_t` to handle server HTTP responses[cite: 1].
 *
 * @return 0 on successfully sending request and receiving complete response[cite: 1].
 * @return Negative error code returned by `http_client_req()` on transmission failure[cite: 1].
 */
int tagoio_http_push(struct tagoio_context *ctx,
		     http_response_cb_t resp_cb)
{
	struct http_request req;
	int ret;

	memset(&req, 0, sizeof(req));

	req.method		= HTTP_POST;
	req.host		= TAGOIO_SERVER;
	req.port		= HTTP_PORT;
	req.url			= TAGOIO_URL;
	req.header_fields	= tagoio_http_headers;
	req.protocol		= "HTTP/1.1";
	req.response		= resp_cb;
	req.payload		= ctx->payload;
	req.payload_len		= strlen(ctx->payload);
	req.recv_buf		= ctx->resp;
	req.recv_buf_len	= sizeof(ctx->resp);

	ret = http_client_req(ctx->sock, &req,
			      CONFIG_TAGOIO_HTTP_CONN_TIMEOUT * MSEC_PER_SEC,
			      ctx);

	close(ctx->sock);
	ctx->sock = -1;

	return ret;
}