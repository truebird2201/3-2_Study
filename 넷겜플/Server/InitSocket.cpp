#include "..\..\Common.h"

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	printf("\n wVersion = %d.%d\n wHighVersion = %d.%d\n szDescription = %s\n szSystemStatus = %s\n\n", LOBYTE(wsa.wVersion), HIBYTE(wsa.wVersion), LOBYTE(wsa.wHighVersion), HIBYTE(wsa.wHighVersion), wsa.szDescription, wsa.szSystemStatus);

	// ���� ����
	WSACleanup();
	return 0;
}