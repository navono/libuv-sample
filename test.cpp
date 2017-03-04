// test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include "uv.h"


#pragma comment(lib, "libuv.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")

//advapi32.lib
//shell32.lib
//user32.lib

uv_loop_t loop;

#include "testTimer.h"
#include "testTcpClient.h"
#include "testTcpServer.h"
#include "testUDP.h"
#include "testPipeServer.h"
#include "testPipeClient.h"
#include "testTTY.h"
#include "testPoll.h"
#include "testPrepare.h"
#include "testAsync.h"
#include "testProcess.h"
#include "testThreadPool.h"
#include "testFilePoll.h"
#include "testFileEvent.h"



int _tmain(int argc, _TCHAR* argv[])
{
	// dummy
	//printf("child starting\n");
	//fflush(stdout);
	//Sleep(1);

	//printf("child running\n");
	//fflush(stdout);
	//Sleep(1);

	//printf("child ending\n");
	//fflush(stdout);
	////_getch();

	//char buf[1024];
	//char* pbuf;
	//HANDLE h_stdin = GetStdHandle(STD_INPUT_HANDLE);
	//HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//for (;;) {
	//	DWORD n_read;
	//	DWORD n_written;
	//	DWORD to_write;
	//	if (!ReadFile(h_stdin, buf, sizeof buf, &n_read, NULL)) {
	//		//ASSERT(GetLastError() == ERROR_BROKEN_PIPE);
	//		return 1;
	//	}
	//	else
	//	{
	//		printf("recv: %s\n", buf);
	//	}
	//	to_write = n_read;
	//	pbuf = buf;
	//	while (to_write) {
	//		WriteFile(h_stdout, pbuf, to_write, &n_written, NULL);
	//		to_write -= n_written;
	//		pbuf += n_written;
	//	}
	//}



	//////////////////////////////////////////////////////////////////////////
	//return run_timer();

	return run_tcp_client();
	//return run_tcp_server();

	//return run_udp();

	//return run_pipe_client();
	//return run_pipe_server();

	//return run_TTY();
	//return run_poll();
	//return run_prepare();
	//return run_async();

	//return run_process();
	//return run_threadpool();
	//return run_file_poll();
	//return run_file_event();
}

