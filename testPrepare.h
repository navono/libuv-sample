uv_prepare_t prepare;

// check��idle�������


void on_prepared(uv_prepare_t* handle) {
	// һ�����߳�������pipe���ӵ�֮��Ĺ���
}

int run_prepare() {
	uv_loop_init(&loop);

	uv_prepare_init(&loop, &prepare);
	uv_prepare_start(&prepare, on_prepared);


	return uv_run(&loop, UV_RUN_DEFAULT);
}