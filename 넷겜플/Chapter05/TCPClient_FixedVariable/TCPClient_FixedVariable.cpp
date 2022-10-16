#include "..\..\Common.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <array>
#include <Windows.h>

using namespace std;

#define SERVERPORT 9000
#define BUFSIZE    50

char* SERVERIP = (char*)"127.0.0.1";
int main(int argc, char *argv[])
{
	int retval;
	
	char* filename = (char*)"Rolling.mp4";

	if (argc>1) SERVERIP = (char*)argv[1];
	if (argc>2) filename = (char*)argv[2];


	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];

	// ������ ������ ���
	// 
	// ������ �Է�(�ùķ��̼�)
	ifstream in{ filename,ios::binary};

	int fsize = 0; // ���� ������
	int nsize = 0; // ���� �̸� ����
	int nowsize = 0; // ���� ���� ������
	int sendsize = 8000; // �ѹ��� ���� ������

	in.seekg(0, ios::end); // ������ �����ΰ���
	fsize = in.tellg(); // ������ ���� �о�´�
	in.seekg(0, ios::beg); // ������ ������ �ٽ� ����.

	nsize = strlen(filename);
	char* dt = new char[fsize];

	in.read(dt, fsize);
	cout << fsize << endl;

	// ������ ������(���� ����)
	retval = send(sock, (char*)&nsize, sizeof(int), 0);		// �����̸��� ũ�� ������
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	retval = send(sock, (char*)&fsize, sizeof(int), 0);		// ������ ũ�� ������
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ������ ������(���� ����)

	retval = send(sock, filename, nsize, 0);		// ������ �̸� ������
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	while (nowsize < fsize) {
		if (nowsize + sendsize <= fsize) {
			retval = send(sock, (char*)&dt[nowsize], sendsize, 0);						// ���� ������
		}
		else {
			retval = send(sock, (char*)&dt[nowsize], fsize % sendsize, 0);
		}
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		nowsize += sendsize;

		if (nowsize > fsize) nowsize = fsize;

		system("cls");
		printf("[TCP Ŭ���̾�Ʈ] ���۷� = %d%% [ %d / %d ]\n", (int)(((float)nowsize / (float)fsize) * 100),nowsize,fsize);
		
	}
	nowsize = 0;

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
