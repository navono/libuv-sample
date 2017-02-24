#include <stdlib.h>

uv_udp_t udp_handle;


void close_cb(uv_handle_t* handle) {
	uv_is_closing(handle);
}
void sv_send_cb(uv_udp_send_t* req, int status) {
	//uv_close((uv_handle_t*) req->handle, close_cb);
	free(req);
}

void on_udp_alloc_cb(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void udp_recv_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf
	, const struct sockaddr* addr, unsigned flags) {
	sockaddr_in *sin = (sockaddr_in*)addr;

	char string_address[INET_ADDRSTRLEN];
	uv_inet_ntop(AF_INET, &sin->sin_addr, string_address, sizeof(string_address));
	//auto ip = inet_ntoa(sin->sin_addr);
	auto port = ntohs(sin->sin_port);

	uv_udp_send_t* req = (uv_udp_send_t*)malloc(sizeof *req);
	uv_buf_t sndbuf = uv_buf_init("PONG", 4);

	uv_udp_send(req, handle, &sndbuf, 1, addr, sv_send_cb);
}

int run_udp() {
	uv_loop_init(&loop);
	uv_udp_init(&loop, &udp_handle);

	sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", 9988, &addr);

	uv_udp_bind(&udp_handle, (const sockaddr*)&addr, 0);
	uv_udp_recv_start(&udp_handle, on_udp_alloc_cb, udp_recv_cb);

	return uv_run(&loop, UV_RUN_DEFAULT);
}