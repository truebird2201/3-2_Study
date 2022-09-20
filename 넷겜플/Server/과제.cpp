#include "..\..\Common.h"

// ������ �̸� -> IPv4 �ּ�
bool GetIPAddr(const char* name, struct in_addr* addr)
{
	struct hostent* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET)
		return false;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return true;
}

int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ������ �̸� -> IP �ּ�
	struct hostent* ptr = gethostbyname(argv[1]);
	int i = 0;

	while (ptr->h_aliases[i] != NULL) {
		printf("%d��° ���� = ", i);
		printf("%s\n", ptr->h_aliases[i]);
		++i;
	}
	i = 0;

	while (ptr->h_addr_list[i] != NULL) {
		struct in_addr addr;
		GetIPAddr(ptr->h_addr_list[i], &addr);
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr, str, sizeof(str));
		printf("%d��° IP�ּ� = ", i);
		printf("%s\n", str);
		++i;
	}

	// ���� ����
	WSACleanup();
	return 0;
}