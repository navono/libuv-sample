#include <stdlib.h>

uv_tcp_t server;


void on_client_close(uv_handle_t* peer) {
	free(peer);
}

void after_shutdown(uv_shutdown_t* req, int status) {
	uv_close((uv_handle_t*) req->handle, on_client_close);
	free(req);
}

void on_read_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void svr_after_write(uv_write_t* req, int status)
{
	//assert(status == 0);
	free(req);
}

void after_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
	uv_shutdown_t* sreq;

	if (nread < 0) {
		//assert(nread == UV_EOF);

		free(buf->base);
		sreq = (uv_shutdown_t*)malloc(sizeof* sreq);
		uv_shutdown(sreq, handle, after_shutdown);
		return;
	}

	if (nread == 0) {
		/* Everything OK, but nothing read. */
		free(buf->base);
		return;
	}

	char *recv = new char[nread + 1];
	memset(recv, 0, nread + 1);
	memcpy_s(recv, nread + 1, buf->base, nread);

	printf("recv: %s\n", recv);
	delete []recv;

	uv_write_t* wr = (uv_write_t*) malloc(sizeof *wr);

	char *response = "This is response";
	uv_buf_t resp_buf = uv_buf_init(response, strlen(response));

	uv_write(wr, handle, &resp_buf, 1, svr_after_write);
}

void on_connection(uv_stream_t* server, int status) {
	uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(&loop, client);

	uv_accept(server, (uv_stream_t*)client);
	uv_read_start((uv_stream_t*)client, on_read_alloc, after_read);
}

int run_tcp_server() {
	uv_loop_init(&loop);

	uv_tcp_init(&loop, &server);

	sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", 9999, &addr);

	uv_tcp_bind(&server, (const sockaddr*)&addr, 0);
	auto r = uv_listen((uv_stream_t*)&server, SOMAXCONN, on_connection);

	return uv_run(&loop, UV_RUN_DEFAULT);
}