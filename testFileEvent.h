uv_fs_event_t fileEvent;



void on_file_event(uv_fs_event_t* handle, const char* filename, int events, int status) {
	char path[1024];
	size_t size = 1023;
	uv_fs_event_getpath(handle, path, &size);
	path[size] = '\0';

	fprintf(stderr, "Change detected in %s: ", path);
	if (events & UV_RENAME)
		fprintf(stderr, "renamed");
	if (events & UV_CHANGE)
		fprintf(stderr, "changed");

	fprintf(stderr, " %s\n", filename ? filename : "");
}

int run_file_event() {
	uv_loop_init(&loop);

	uv_fs_event_init(&loop, &fileEvent);
	uv_fs_event_start(&fileEvent, on_file_event, "E:\\vector_string.txt", UV_FS_EVENT_WATCH_ENTRY);

	return uv_run(&loop, UV_RUN_DEFAULT);
}