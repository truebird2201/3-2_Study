#include "Common.h"

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	struct hostent* ptr = gethostbyname(argv[1]);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET)
		return false;
	int i = 0;
	while (ptr->h_aliases[i] != NULL) {
		printf("%d��° ���� - %s\n", i, ptr->h_aliases[i]);
		++i;
	}
	i = 0;
	while (ptr->h_addr_list[i] != NULL) {
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, ptr->h_addr_list[i], str, sizeof(str));
		printf("%d��° IP�ּ� - %s\n", i, str);
		++i;
	}

	// ���� ����
	WSACleanup();
	return 0;
}