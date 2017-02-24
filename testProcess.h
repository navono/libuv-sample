#include <string>

uv_process_t process;
uv_shutdown_t *sreq;

void exit_cb(uv_process_t* process, int64_t exit_status, int term_signal) {
	uv_close((uv_handle_t*)process, NULL);
}

void pro_pipe_after_shutdown(uv_shutdown_t* req, int status) {
	uv_close((uv_handle_t*) req->handle, NULL);
	free(req);
}

void on_pro_pipe_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void on_pro_pipe_read(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf) {
	if (nread < 0) {
		//assert(nread == UV_EOF);

		free(buf->base);
		sreq = (uv_shutdown_t*)malloc(sizeof* sreq);
		uv_shutdown(sreq, handle, pro_pipe_after_shutdown);
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
}

void write_cb(uv_write_t* req, int status) {
	free(req);
}

int run_process() {
	uv_loop_init(&loop);

	int r;
	uv_pipe_t out, in;
	uv_pipe_init(&loop, &out, 0);
	uv_pipe_init(&loop, &in, 0);

	char exepath[1024];
	size_t exepath_size = 1024;
	uv_exepath(exepath, &exepath_size);
	exepath[exepath_size] = '\0';

	std::string str(exepath), path;
	auto pos = str.find_last_of('\\');
	if (pos != std::string::npos) {
		path = str.substr(0, pos);
	}

	path.append("\\dummy.exe");

	char *args[3];
	args[0] = "dummy.exe"/*(char*)path.c_str()*/;
	args[1] = NULL;
	args[2] = NULL;

	uv_process_options_t opt;
	opt.file = path.c_str()/*"C:\\Windows\\System32\\win32calc.exe"*/;
	opt.args = args;
	opt.exit_cb = exit_cb;
	opt.flags = UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS;
	opt.env = NULL;
	opt.cwd = NULL;

	uv_stdio_container_t stdio[3];
	//stdio[0].flags = UV_IGNORE;
	stdio[0].flags = (uv_stdio_flags)(UV_CREATE_PIPE | UV_READABLE_PIPE);
	stdio[0].data.stream = (uv_stream_t*)&in;

	stdio[1].flags = (uv_stdio_flags)(UV_CREATE_PIPE | UV_WRITABLE_PIPE);
	stdio[1].data.stream = (uv_stream_t*)&out;
	stdio[2].flags = UV_IGNORE;

	opt.stdio_count = 3;
	opt.stdio = stdio;
	
	r = uv_spawn(&loop, &process, &opt);
	auto s = uv_err_name(r);

	r = uv_read_start((uv_stream_t*)&out, on_pro_pipe_alloc, on_pro_pipe_read);
	s = uv_err_name(r);

	// write to child process
	uv_buf_t info;
	char buffer[] = "hello-from-spawn_stdin";
	info.base = buffer;
	info.len = sizeof(buffer);

	uv_write_t *w_req = (uv_write_t *)malloc(sizeof uv_write_t);
	r = uv_write(w_req, (uv_stream_t*)&in, &info, 1, write_cb);

	return uv_run(&loop, UV_RUN_DEFAULT);
}