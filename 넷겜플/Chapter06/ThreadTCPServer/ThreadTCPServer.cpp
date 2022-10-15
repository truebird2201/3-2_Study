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


// Ŭ���̾�Ʈ�� ������ ���
DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;
	struct sockaddr_in clientaddr;
	char addr[INET_ADDRSTRLEN];
	int addrlen;
	char buf[BUFSIZE + 1];

	int fsize = 0; // ���� ũ��
	int nsize = 0; // ���� �̸� ũ��
	int nowsize = 0; // ������� ���� ũ��
	int receiveSize = 3000; // �ѹ��� �޾ƿ��� ũ��

	DWORD retval;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (struct sockaddr *)&clientaddr, &addrlen);
	inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));

	while (1) {
		// Ŭ���̾�Ʈ�� ������ ���
		// ������ �ޱ�(���� ����)
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();	// ����

		retval = recv(client_sock, (char*)&nsize, sizeof(int), MSG_WAITALL);	// ���� �̸� ũ�� �ޱ�

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		retval = recv(client_sock, (char*)&fsize, sizeof(int), MSG_WAITALL);	// ���� ũ�� �ޱ�

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// ������ �ޱ�(���� ����)
		char* buf = new char[fsize]; // ���� ���� ������
		char* filename = new char[nsize + 1]; // ���� �̸�

		retval = recv(client_sock, filename, nsize, MSG_WAITALL);	// ���� �̸�
		filename[nsize] = '\0';

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		while (nowsize < fsize) {
			if (nowsize + receiveSize <= fsize) {
				retval = recv(client_sock, (char*)&buf[nowsize], receiveSize, MSG_WAITALL);					// ���� ������
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
			cout << "\n���۷� = " << (int)(((float)nowsize / (float)fsize) * 100) << "[ " << nowsize << "/" << fsize << " ]\n";
			SetEvent(hWriteEvent);
		}
		nowsize = 0;

		std::chrono::system_clock::time_point end = std::chrono::system_clock::now(); // ��
		std::chrono::nanoseconds nano = end - start;

		cout << " ������ �޾ƿԽ��ϴ�. " << endl;

		cout << "-----------------------------��� �ð� = " << nano.count() << endl << endl;

		ofstream out{ filename, ios::binary };
		out.write(buf, fsize);
	}

	// ���� �ݱ�
	closesocket(client_sock);
	cout << "[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = "<< addr<<"��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port)<<endl;
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	// ������ ��ſ� ����� ����
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

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		cout << "[TCP ����] Ŭ���̾�Ʈ  ����: IP �ּ� = " << addr << "��Ʈ ��ȣ = " << ntohs(clientaddr.sin_port)<<endl;

		// ������ ����
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}
	DeleteCriticalSection(&cs);
	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
