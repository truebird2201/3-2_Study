#include "..\..\Common.h"

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
		return 1;
	printf("\n wVersion = %#x\n wHighVersion = %#x\n szDescription = %s\n szSystemStatus = %s\n\n", wsa.wVersion, wsa.wHighVersion, wsa.szDescription, wsa.szSystemStatus);

	// ���� ����
	WSACleanup();
	return 0;
}