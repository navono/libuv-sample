//const char *pipeName = "\\\\.\\pipe\\piping";
uv_pipe_t pipeClient;


void on_pipe_client_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void after_pipe_client_write(uv_write_t *req, int status) {
	free(req);
}

void after_pipe_client_read(uv_stream_t* pipeServer, ssize_t nread, const uv_buf_t* buf) {
	if (nread > 0) {
		char *recv_data = new char[nread + 1];
		memset(recv_data, 0, nread + 1);

		memcpy_s(recv_data, nread + 1, buf->base, nread);
		printf("pipe client: recv --> %s\n", recv_data);
		delete []recv_data;

		uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));

		char *data = "test from pipe client";
		auto buf = uv_buf_init(data, strlen(data));
		auto send_buf = uv_buf_init(data, strlen(data));

		uv_write((uv_write_t*)req, pipeServer, &send_buf, 1, after_pipe_client_write);
		return;
	}

	if (nread < 0) {
		if (nread != UV_EOF)
			fprintf(stderr, "Read error %s\n", uv_err_name(nread));
		uv_close((uv_handle_t*) client, NULL);
	}

	free(buf->base);
}

void on_pipe_client_connection(uv_connect_t* req, int status) {
	uv_pipe_t *pipeClient = (uv_pipe_t*)req->data;

	//char buf[1024];
	//size_t len;
	//int r;

	//len = sizeof buf;
	//r = uv_pipe_getpeername(p, buf, &len);

	//auto s = memcmp(buf, pipeName, len) == 0;

	//len = sizeof buf;
	//r = uv_pipe_getsockname(p, buf, &len);

	uv_read_start((uv_stream_t*)pipeClient, on_pipe_client_alloc, after_pipe_client_read);
}

int run_pipe_client() {
	uv_loop_init(&loop);

	uv_pipe_init(&loop, &pipeClient, 1);

	uv_connect_t *connect_req = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	connect_req->data = &pipeClient;
	uv_pipe_connect(connect_req, &pipeClient, pipeName, on_pipe_client_connection);

	return uv_run(&loop, UV_RUN_DEFAULT);
}