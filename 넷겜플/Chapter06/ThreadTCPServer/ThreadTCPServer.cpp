#include "..\..\Common.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <format>

#define SERVERPORT 9000
#define BUFSIZE    512

CRITICAL_SECTION cs;
using namespace std;
HANDLE hWriteEvent;
HANDLE hReadEvent;


// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	int fsize = 0; // 파일 크기
	int nsize = 0; // 파일 이름 크기
	int nowsize = 0; // 현재까지 받은 크기
	int receiveSize = 3000; // 한번에 받아오는 크기

	DWORD retval;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr *)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1) {
		// 클라이언트와 데이터 통신
		// 데이터 받기(고정 길이)
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();	// 시작

		retval = recv(client_sock, (char*)&nsize, sizeof(int), MSG_WAITALL);	// 파일 이름 크기 받기

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		retval = recv(client_sock, (char*)&fsize, sizeof(int), MSG_WAITALL);	// 파일 크기 받기

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 데이터 받기(가변 길이)
		char* buf = new char[fsize]; // 가변 길이 데이터
		char* filename = new char[nsize + 1]; // 파일 이름

		retval = recv(client_sock, filename, nsize, MSG_WAITALL);	// 파일 이름
		filename[nsize] = '\0';

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		while (nowsize < fsize) {
			if (nowsize + receiveSize <= fsize) {
				retval = recv(client_sock, (char*)&buf[nowsize], receiveSize, MSG_WAITALL);					// 파일 보내기
			}
			else {
				retval = recv(client_sock, (char*)&buf[nowsize], fsize % receiveSize, MSG_WAITALL);
			}
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}
			nowsize += receiveSize;

			if (nowsize > fsize) nowsize = fsize;

			retval = WaitForSingleObject(hReadEvent, INFINITE);
			if (retval != WAIT_OBJECT_0) break;
			CONSOLE_SCREEN_BUFFER_INFO cur;
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
			COORD pos = { cur.dwCursorPosition.X, cur.dwCursorPosition.Y };
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
			cout << "\n전송률 = " << (int)(((float)nowsize / (float)fsize) * 100) << "[ " << nowsize << "/" << fsize << " ]\n";
			SetEvent(hWriteEvent);
		}
		nowsize = 0;

		std::chrono::system_clock::time_point end = std::chrono::system_clock::now(); // 끝
		std::chrono::nanoseconds nano = end - start;

		cout << " 파일을 받아왔습니다. " << endl;

		cout << "-----------------------------경과 시간 = " << nano.count() << endl << endl;

		ofstream out{ filename, ios::binary };
		out.write(buf, fsize);
	}

	// 소켓 닫기
	closesocket(client_sock);
	cout << "[TCP 서버] 클라이언트 종료: IP 주소 = "<< addr<<"포트 번호 = " << ntohs(clientaddr.sin_port)<<endl;
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;

	InitializeCriticalSection(&cs);

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		cout << "[TCP 서버] 클라이언트  접속: IP 주소 = " << addr << "포트 번호 = " << ntohs(clientaddr.sin_port)<<endl;

		// 스레드 생성
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
	DeleteCriticalSection(&cs);
	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
