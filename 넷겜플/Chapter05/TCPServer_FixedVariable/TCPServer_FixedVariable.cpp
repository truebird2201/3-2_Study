#include "..\..\Common.h"
#include <chrono>
#include <iostream>
#include <fstream>

#define SERVERPORT 9000
#define BUFSIZE    512

using namespace std;

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
	int fsize=0; // ���� ũ��
	int nsize=0; // ���� ũ��
	

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
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
			// ������ �ޱ�(���� ����)
			std::chrono::system_clock::time_point start = std::chrono::system_clock::now();	// ����

			retval = recv(client_sock, (char*)&nsize, sizeof(int), MSG_WAITALL);	// ���� �̸� ũ�� �ޱ�
			cout << " ���� �̸� ũ�� = " << nsize << endl;
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			retval = recv(client_sock, (char *)&fsize, sizeof(int), MSG_WAITALL);	// ���� ũ�� �ޱ�
			cout << " ���� ũ�� = " << fsize << endl;
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ������ �ޱ�(���� ����)
			char* buf = new char[fsize]; // ���� ���� ������
			char* filename = new char [nsize+1]; // ���� �̸�

			retval = recv(client_sock, filename, nsize, MSG_WAITALL);	// ���� �̸�
			filename[nsize] = '\0';
			cout << " ���� �̸� = " << filename << endl;
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			while (1) {
				retval = recv(client_sock, buf, fsize, MSG_WAITALL);	// ����
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}
				else if (retval == 0)
					break;
			}

			std::chrono::system_clock::time_point end = std::chrono::system_clock::now(); // ��
			std::chrono::nanoseconds nano = end - start;
			
			cout << " ������ �޾ƿԽ��ϴ�. " << endl;

			cout << "-----------------------------��� �ð� = "<< nano.count() << endl <<endl;

			ofstream out{ filename, ios::binary };
			out.write(buf, fsize);

		//}

		// ���� �ݱ�
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port));
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
