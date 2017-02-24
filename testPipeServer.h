const char *pipeName = "\\\\.\\pipe\\piping";
uv_pipe_t pipeServer;


void on_pipe_server_calloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void after_pipe_server_write(uv_write_t *req, int status) {
	free(req);
}

void on_pipe_server_read(uv_stream_t* pipeClient, ssize_t nread, const uv_buf_t* buf) {
	if (nread > 0) {
		char *revc_data = new char[nread + 1];
		memset(revc_data, 0, nread + 1);

		memcpy_s(revc_data, nread + 1, buf->base, nread);
		printf("pipe server: recv --> %s\n", revc_data);
		delete []revc_data;

		uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
		char *send_data = "pipe server response";
		uv_buf_t send_buf = uv_buf_init(send_data, strlen(send_data));
		uv_write((uv_write_t*)req, pipeClient, &send_buf, 1, after_pipe_server_write);
		printf("pipe server: send --> %s\n", send_data);

		return;
	}

	if (nread < 0) {
		if (nread != UV_EOF)
			printf("pipe server: read error --> %s\n", uv_err_name(nread));

		uv_close((uv_handle_t*) pipeClient, nullptr);
		printf("pipe server: close pipe client\n");
	}

	free(buf->base);
}

void on_pipe_server_connection(uv_stream_t* server, int status) {
	if (server->type == UV_NAMED_PIPE) {
		printf("pipe server: incoming connection.\n");

		uv_pipe_t *pipeClient;
		pipeClient = (uv_pipe_t *)malloc(sizeof(uv_pipe_t));
		uv_pipe_init(server->loop, pipeClient, 1);
		uv_accept(server, (uv_stream_t*)pipeClient);

		server->data = pipeClient;

		uv_write_t *req = (uv_write_t*) malloc(sizeof(uv_write_t));
		char *data = "test from pipe server";
		auto buf = uv_buf_init(data, strlen(data));
		auto send_buf = uv_buf_init(data, strlen(data));
		uv_write((uv_write_t*)req, (uv_stream_t*)pipeClient, &send_buf, 1, after_pipe_server_write);
		printf("pipe server: send %s\n", data);

		uv_read_start((uv_stream_t*)pipeClient, on_pipe_server_calloc, on_pipe_server_read);
		printf("pipe server: start uv_read_start\n");
	}
}

int run_pipe_server() {
	uv_loop_init(&loop);
	
	uv_pipe_init(&loop, &pipeServer, 0);
	uv_pipe_bind(&pipeServer, pipeName);
	uv_listen((uv_stream_t*)&pipeServer, 128, on_pipe_server_connection);
	
	printf("pipe server: listening...\n");

	//uv_read_start((uv_stream_t*)&pipeServer, on_pipe_server_calloc, after_pipe_server_read);
	
	return uv_run(&loop, UV_RUN_DEFAULT);
}