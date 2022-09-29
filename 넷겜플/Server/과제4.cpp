#include "Common.h"

int main(int argc, char* argv[])
{

	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 최솟값, 최댓값 받기
	char* SERVERIP = argv[1];
	int MinP = atoi(argv[2]);
	int MaxP = atoi(argv[3]);


	for (int Pnum = MinP; Pnum < MaxP+1; ++Pnum) {

		// 소켓 생성
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
			printf("포트번호 %d - LISTENING 상태 아님\n", Pnum);
		}
		else {
			printf("포트번호 %d - LISTENING 상태\n", Pnum);
		}
		closesocket(sock);

	}

	// 윈속 종료
	WSACleanup();
	return 0;
}