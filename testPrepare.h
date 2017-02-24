uv_prepare_t prepare;

// check和idle与此类似


void on_prepared(uv_prepare_t* handle) {
	// 一般做线程启动，pipe连接等之类的工作
}

int run_prepare() {
	uv_loop_init(&loop);

	uv_prepare_init(&loop, &prepare);
	uv_prepare_start(&prepare, on_prepared);


	return uv_run(&loop, UV_RUN_DEFAULT);
}