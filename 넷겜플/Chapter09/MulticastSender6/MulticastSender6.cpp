#include "..\..\Common.h"

#define MULTICASTIP "FF12::1:2:3:4"
#define REMOTEPORT  9000
#define BUFSIZE     512

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET sock = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// ��Ƽĳ��Ʈ TTL ����
	DWORD ttl = 2;
	retval = setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS,
		(const char *)&ttl, sizeof(ttl));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	// ���� �ּ� ����ü �ʱ�ȭ
	struct sockaddr_in6 remoteaddr;
	memset(&remoteaddr, 0, sizeof(remoteaddr));
	remoteaddr.sin6_family = AF_INET6;
	inet_pton(AF_INET6, MULTICASTIP, &remoteaddr.sin6_addr);
	remoteaddr.sin6_port = htons(REMOTEPORT);

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	int len;

	// ��Ƽĳ��Ʈ ������ ������
	while (1) {
		// ������ �Է�
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		// '\n' ���� ����
		len = (int)strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;

		// ������ ������
		retval = sendto(sock, buf, (int)strlen(buf), 0,
			(struct sockaddr *)&remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			break;
		}
		printf("[UDP] %d����Ʈ�� ���½��ϴ�.\n", retval);
	}

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}
