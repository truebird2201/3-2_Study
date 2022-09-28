#include "..\..\Common.h"
#include <ws2bth.h>
#include <initguid.h>
#include <locale.h>

#define SERVERADDR _T("E0:06:E6:AF:78:9F") /* �ǽ� ȯ�濡 ���� ���� �ʿ�! */
#define BUFSIZE    512

DEFINE_GUID(BthServer_Service, 0x4672de25, 0x588d, 0x48af,
	0x80, 0x73, 0x5f, 0x2b, 0x7b, 0x0, 0x60, 0x1f);

int main(int argc, char *argv[])
{
	int retval;

	// Unicode �ѱ��� ���
	setlocale(LC_ALL, "");

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ������� ��ġ �˻� (����)
	// - ���ǻ� SERVERADDR ����� ����Ѵ�.

	// ������� ���� �˻� �غ�
	DWORD qslen = sizeof(WSAQUERYSET);
	WSAQUERYSET *qs = (WSAQUERYSET *)malloc(qslen);
	memset(qs, 0, qslen);
	qs->dwSize = qslen;
	qs->dwNameSpace = NS_BTH;
	qs->lpServiceClassId = (GUID *)&BthServer_Service;
	qs->lpszContext = (LPTSTR)SERVERADDR;
	DWORD flags = LUP_FLUSHCACHE | LUP_RETURN_ADDR;

	// ������� ���� �˻� ����
	HANDLE hLookup;
	retval = WSALookupServiceBegin(qs, flags, &hLookup);
	if (retval == SOCKET_ERROR) {
		printf("[����] �߰ߵ� ������� ��ġ ����!\n");
		exit(1);
	}

	// �˻��� ������� ���� ���� Ȯ��
	SOCKADDR_BTH *sa = NULL;
	int serverport = 0;
	bool done = false;
	while (!done) {
		retval = WSALookupServiceNext(hLookup, flags, &qslen, qs);
		if (retval == NO_ERROR) {
			// ��ġ ������ ���� ���� �ּ� ����ü ����
			sa = (SOCKADDR_BTH *)qs->lpcsaBuffer->RemoteAddr.lpSockaddr;
			// ���� ��Ʈ ��ȣ ���� �� Ż��
			serverport = sa->port;
			break;
		}
		else {
			if (WSAGetLastError() == WSAEFAULT) {
				free(qs);
				qs = (WSAQUERYSET *)malloc(qslen);
			}
			else {
				done = true;
			}
		}
	}
	if (sa == NULL) {
		_tprintf(_T("[����] %s���� ���� ���� ���� ����!\n"), SERVERADDR);
		exit(1);
	}

	// ������� ���� �˻� ����
	WSALookupServiceEnd(hLookup);
	free(qs);

	// ���� ����
	SOCKET sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_BTH serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	int addrlen = sizeof(serveraddr);
	WSAStringToAddress((LPTSTR)SERVERADDR, AF_BTH, NULL,
		(struct sockaddr *)&serveraddr, &addrlen);
	serveraddr.port = serverport;
	retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ������ ������ ���
	while (1) {
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' ���� ����
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';
		if (strlen(buf) == 0) break;

		// ������ ������
		retval = send(sock, buf, (int)strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[������� Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
	}

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
