#include "..\..\Common.h"

// 도메인 이름 -> IPv4 주소
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
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 도메인 이름 -> IP 주소
	struct hostent* ptr = gethostbyname(argv[1]);
	int i = 0;

	while (ptr->h_aliases[i] != NULL) {
		printf("%d번째 별명 = ", i);
		printf("%s\n", ptr->h_aliases[i]);
		++i;
	}
	i = 0;

	while (ptr->h_addr_list[i] != NULL) {
		struct in_addr addr;
		GetIPAddr(ptr->h_addr_list[i], &addr);
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr, str, sizeof(str));
		printf("%d번째 IP주소 = ", i);
		printf("%s\n", str);
		++i;
	}

	// 윈속 종료
	WSACleanup();
	return 0;
}