#include "..\..\Common.h"
#include <ws2bth.h>
#include <initguid.h>

#define BUFSIZE 512

DEFINE_GUID(BthServer_Service, 0x4672de25, 0x588d, 0x48af,
	0x80, 0x73, 0x5f, 0x2b, 0x7b, 0x0, 0x60, 0x1f);

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET listen_sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_BTH serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.addressFamily = AF_BTH;
	serveraddr.btAddr = 0;
	serveraddr.port = BT_PORT_ANY;
	retval = bind(listen_sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// ���� ��Ʈ ��ȣ ��� (�ɼ�)
	int addrlen = sizeof(serveraddr);
	retval = getsockname(listen_sock, (struct sockaddr *)&serveraddr, &addrlen);
	if (retval == SOCKET_ERROR) err_quit("bind()");
	printf("[������� ����] ��� ��Ʈ ��ȣ: %d\n", serveraddr.port);

	// ���� ���� ��� (�ʼ�)
	CSADDR_INFO addrinfo;
	addrinfo.LocalAddr.lpSockaddr = (struct sockaddr *)&serveraddr;
	addrinfo.LocalAddr.iSockaddrLength = sizeof(serveraddr);
	addrinfo.RemoteAddr.lpSockaddr = (struct sockaddr *)&serveraddr;
	addrinfo.RemoteAddr.iSockaddrLength = sizeof(serveraddr);
	addrinfo.iSocketType = SOCK_STREAM;
	addrinfo.iProtocol = BTHPROTO_RFCOMM;

	WSAQUERYSET qset;
	memset(&qset, 0, sizeof(qset));
	qset.dwSize = sizeof(qset);
	qset.lpszServiceInstanceName = (LPTSTR)_T("Bluetooth Server Test Service");
	qset.lpServiceClassId = (GUID *)&BthServer_Service;
	qset.dwNameSpace = NS_BTH;
	qset.dwNumberOfCsAddrs = 1;
	qset.lpcsaBuffer = &addrinfo;
	retval = WSASetService(&qset, RNRSERVICE_REGISTER, 0);
	if (retval == SOCKET_ERROR) err_quit("WSASetService()");

	// listen()
	retval = listen(listen_sock, 1);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_BTH clientaddr;
	char buf[BUFSIZE + 1];

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		printf("\n[������� ����] Ŭ���̾�Ʈ ����!\n");

		// Ŭ���̾�Ʈ�� ������ ���
		while (1) {
			// ������ �ޱ�
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			else if (retval == 0)
				break;

			// ���� ������ ���
			buf[retval] = '\0';
			printf("[������� ����] %s\n", buf);
		}

		// ���� �ݱ�
		closesocket(client_sock);
		printf("[������� ����] Ŭ���̾�Ʈ ����!\n");
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}
