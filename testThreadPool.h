
uv_work_t work;


void do_some_work(uv_work_t* req)
{
	printf("do some long work!\n");
	Sleep(5000);
}

void after_work(uv_work_t* req, int status)
{
	printf("word done!\n");
}

int run_threadpool()
{
	uv_loop_init(&loop);

	uv_queue_work(&loop,&work, do_some_work, after_work);

	return uv_run(&loop, UV_RUN_DEFAULT);
}