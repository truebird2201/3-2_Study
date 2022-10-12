#include "..\..\Common.h"
#include <chrono>
#include <iostream>
#include <fstream>

#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;
int fsize = 0; // 파일 크기
int nsize = 0; // 파일 이름 크기
int nowsize = 0; // 현재까지 받은 크기
int receiveSize = 10; // 한번에 받아오는 크기

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

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

			system("cls");
			printf("전송률 = %d%% [ %d / %d ] \n", (int)(((float)nowsize / (float)fsize) * 100), nowsize, fsize);

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
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		addr, ntohs(clientaddr.sin_port));
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
	HANDLE hThread;


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
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
