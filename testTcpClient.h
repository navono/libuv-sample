#include <stdlib.h>


uv_tcp_t *client;
uv_connect_t con_req;

void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void after_write(uv_write_t* req, int status)
{
	//assert(status == 0);
	free(req);
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	char *data = new char[nread + 1];
	memset(data, 0, nread + 1);

	memcpy_s(data, nread + 1, buf->base, nread);
	printf("recv: %s\n", data);
	delete []data;

	uv_write_t *w_req = (uv_write_t*)malloc(sizeof(uv_write_t));
	
	char *send = "response from client";
	auto send_buf = uv_buf_init(send, strlen(send));

	uv_write(w_req, (uv_stream_t*)&stream->data, &send_buf, 1, after_write);
}

void on_connect(uv_connect_t* req, int status)
{
	//assert(status == 0);
	
	auto client_handle = (uv_tcp_t *)req->data;
	uv_read_start((uv_stream_t*)client_handle, on_alloc, on_read);
}

int run_tcp_client()
{
	uv_loop_init(&loop);

	client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
	uv_tcp_init(&loop, client);
	
	sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", 8888, &addr);
	
	con_req.data = client;	// private data
	uv_tcp_connect(&con_req, client, (const sockaddr*)&addr, on_connect);

	return uv_run(&loop, UV_RUN_DEFAULT);
}