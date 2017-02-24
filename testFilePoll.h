uv_fs_poll_t filePoll;
char buffer[1024] = {0};


void on_read(uv_fs_t* readReq) {
	auto open_req = (uv_fs_t*)readReq->data;
	auto buf = (uv_buf_t*)open_req->data;

	if (readReq->result < 0) {
		fprintf(stderr, "Read error: %s\n", uv_strerror(readReq->result));
		return;
	}
	else if (readReq->result == 0) {
		uv_fs_t close_req;
		// synchronous

		uv_fs_close(&loop, &close_req, open_req->result, NULL);
	}
	else if (readReq->result > 0) {
		printf("file content: %s\n", (char*)buf->base);
	}

	uv_fs_req_cleanup(readReq);
	uv_fs_req_cleanup(open_req);

	free(buf);
	free(readReq);
	free(open_req);
}

void on_file_open(uv_fs_t* openReq) {
	auto s = uv_strerror(openReq->result);
	if (openReq->result >= 0) {
		auto buf = (uv_buf_t*)malloc(sizeof uv_buf_t);
		buf->base = buffer;
		buf->len = sizeof buffer;

		uv_fs_t *read_req = (uv_fs_t*)malloc(sizeof uv_fs_t);
		read_req->data = openReq;
		openReq->data = buf;
		uv_fs_read(&loop, read_req, openReq->result, buf, 1, -1, on_read);
	}
}

int run_file_poll() {
	uv_loop_init(&loop);

	uv_fs_t *open_req = (uv_fs_t *)malloc(sizeof uv_fs_t);
	
	// flags: O_RDONLY, O_WRONLY, O_RDWR
	// mode: O_APPEND, 
	auto r = uv_fs_open(&loop, open_req, "E:\\vector_string.txt", O_RDONLY, 0, on_file_open);

	return uv_run(&loop, UV_RUN_DEFAULT);
}