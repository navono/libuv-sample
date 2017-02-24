uv_tty_t tty_handle;
uv_timer_t tick;
uv_write_t write_req;
int width, height;
int pos = 0;
char *message = "  Hello TTY  ";


void update(uv_timer_t *req) {
	char data[500];

	uv_buf_t buf;
	buf.base = data;
	buf.len = sprintf_s(data, 500, "\033[2J\033[H\033[%dB\033[%luC\033[42;37m%s",
		pos,
		(unsigned long) (width-strlen(message))/2,
		message);
	uv_write(&write_req, (uv_stream_t*) &tty_handle, &buf, 1, NULL);

	pos++;
	if (pos > height) {
		uv_tty_reset_mode();
		uv_timer_stop(&tick);
	}
}

void test_tty1()
{
	if (uv_guess_handle(1) == UV_TTY) {
		uv_write_t req;
		uv_buf_t buf;
		buf.base = "\033[41;37m";
		buf.len = strlen(buf.base);
		uv_write(&req, (uv_stream_t*) &tty_handle, &buf, 1, NULL);
	}

	uv_write_t req;
	uv_buf_t buf;
	buf.base = "Hello TTY\n";
	buf.len = strlen(buf.base);
	uv_write(&req, (uv_stream_t*) &tty_handle, &buf, 1, NULL);
	uv_tty_reset_mode();
}

int test_tty2()
{
	if (uv_tty_get_winsize(&tty_handle, &width, &height)) {
		fprintf(stderr, "Could not get TTY information\n");
		uv_tty_reset_mode();
		return 1;
	}

	fprintf(stderr, "Width %d, height %d\n", width, height);
	uv_timer_init(&loop, &tick);
	uv_timer_start(&tick, update, 200, 200);

	return 0;
}

int run_TTY()
{
	uv_loop_init(&loop);

	uv_tty_init(&loop, &tty_handle, 1, 0);
	uv_tty_set_mode(&tty_handle, UV_TTY_MODE_NORMAL);

	test_tty2();

	return uv_run(&loop, UV_RUN_DEFAULT);
}