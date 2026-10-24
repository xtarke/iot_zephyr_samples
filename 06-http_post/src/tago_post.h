#ifndef __TAGO_POST_H__
#define __TAGO_POST_H__


#define TAGOIO_SOCKET_MAX_BUF_LEN 1280

struct tagoio_context {
	int sock;
	uint8_t payload[TAGOIO_SOCKET_MAX_BUF_LEN];
	uint8_t resp[TAGOIO_SOCKET_MAX_BUF_LEN];
};

int tagoio_connect(struct tagoio_context *ctx);
int tagoio_http_push(struct tagoio_context *ctx,
		     http_response_cb_t resp_cb);



#endif //__TAGO_POST_H__
