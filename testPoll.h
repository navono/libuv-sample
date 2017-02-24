
uv_poll_t poll;

void on_poll_close(uv_handle_t* handle) {

}

void on_poll_cb(uv_poll_t* poll, int status, int events) {
	auto sock = (uv_os_sock_t*)poll->data;
	char buffer[25] = {0};
	auto r = recv(*sock, buffer, sizeof buffer, 0);

	if (_memicmp(buffer, "Q", 1) == 0) {
		uv_poll_stop(poll);
		closesocket(*sock);
		printf("poll exit\n", buffer);
		
		// crash
		//uv_close((uv_handle_t*) &poll, on_poll_close);
		
		return;
	}

	printf("poll data: %s\n", buffer);
}

int run_poll() {
	WSAData wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	uv_os_sock_t sock = socket(AF_INET, SOCK_STREAM, 0);
	
	unsigned long on = 1;
	auto r = ioctlsocket(sock, FIONBIO, &on);

	sockaddr_in addr;
	r = uv_ip4_addr("127.0.0.1", 8899, &addr);
	r = connect(sock, (sockaddr*)&addr, sizeof addr);

	r = uv_loop_init(&loop);
	r = uv_poll_init_socket(&loop, &poll, sock);
	poll.data = &sock;

	r = uv_poll_start(&poll, UV_READABLE, on_poll_cb);

	return uv_run(&loop, UV_RUN_DEFAULT);
}