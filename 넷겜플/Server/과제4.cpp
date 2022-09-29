#include "Common.h"

int main(int argc, char* argv[])
{

	int retval;
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// �ּڰ�, �ִ� �ޱ�
	char* SERVERIP = argv[1];
	int MinP = atoi(argv[2]);
	int MaxP = atoi(argv[3]);


	for (int Pnum = MinP; Pnum < MaxP+1; ++Pnum) {

		// ���� ����
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		// connect()
		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(Pnum);
		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) {
			printf("��Ʈ��ȣ %d - LISTENING ���� �ƴ�\n", Pnum);
		}
		else {
			printf("��Ʈ��ȣ %d - LISTENING ����\n", Pnum);
		}
		closesocket(sock);

	}

	// ���� ����
	WSACleanup();
	return 0;
}