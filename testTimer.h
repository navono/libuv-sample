#include <stdlib.h>
#include <windows.h>

uv_timer_t timer;
unsigned int counter = 0;
unsigned long long tickCount = 0;

void on_timer(uv_timer_t* handle)
{
	auto a = (int*)handle->data;
	*a = 11;
	
	if (counter >= 5)
	{
		printf("uv_timer_stop\n");
		uv_timer_stop(handle);
	}
	else
	{
		++counter;
		auto now = GetTickCount64();
		printf("on_timer: %d	elapsed: %I64d\n", counter, now - tickCount);
		tickCount = now;
	}
}

int run_timer()
{
	uv_loop_init(&loop);
	uv_timer_init(&loop, &timer);

	// private data
	int a = 10;
	timer.data = &a;

	// first starts `timeout` seconds after execution of `uv_timer_start`
	// then repeats every `repeat` seconds.
	uv_timer_start(&timer, on_timer, 5000, 5000);
	tickCount = GetTickCount64();

	return uv_run(&loop, UV_RUN_DEFAULT);
}