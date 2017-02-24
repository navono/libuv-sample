uv_async_t async;
uv_timer_t async_timer;
unsigned int count = 0;


void on_timer_cb(uv_timer_t* handle)
{
	uv_async_send(&async);
}

void on_async_cb(uv_async_t* handle)
{
	printf("processing in async cb\n");

	if (count == 3)
	{
		printf("exit\n");

		auto timer = (uv_timer_t*)handle->data;
		uv_timer_stop(timer);

		uv_close((uv_handle_t*)handle, NULL);
		uv_close((uv_handle_t*)&async_timer, NULL);
	}

	count++;
}

int run_async() {
	uv_loop_init(&loop);

	uv_timer_init(&loop, &async_timer);

	async.data = &async_timer;
	uv_async_init(&loop, &async, on_async_cb);

	uv_timer_start(&async_timer, on_timer_cb, 1000, 5000);

	return uv_run(&loop, UV_RUN_DEFAULT);
}