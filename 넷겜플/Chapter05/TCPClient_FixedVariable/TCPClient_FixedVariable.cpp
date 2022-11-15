#include "..\..\Common.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <array>
#include <Windows.h>
#include "resource.h"

#define SERVERPORT 9000
#define BUFSIZE    50

SOCKET sock;

HWND hFolderButton; // ���� ã�� ��ư
HWND hSendButton; // ������ ��ư

char* SERVERIP = (char*)"127.0.0.1";
char* filename = (char*)"Rolling.mp4";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	WSACleanup();
	return 0;
}

// ��ȭ���� ���ν���
INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		hFolderButton = GetDlgItem(hDlg, IDC_BUTTON1);
		hSendButton = GetDlgItem(hDlg, IDC_BUTTON2);
		SendMessage(hEdit1, EM_SETLIMITTEXT, BUFSIZE, 0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDC_BUTTON1:	// ���� ���� ��ư
			EnableWindow(hSendButton, FALSE); // ������ ��ư ��Ȱ��ȭ
			WaitForSingleObject(hReadEvent, INFINITE); // �б� �Ϸ� ���
			GetDlgItemTextA(hDlg, IDC_EDIT1, buf, BUFSIZE + 1);
			SetEvent(hWriteEvent); // ���� �Ϸ� �˸�
			SetFocus(hEdit1); // Ű���� ��Ŀ�� ��ȯ
			SendMessage(hEdit1, EM_SETSEL, 0, -1); // �ؽ�Ʈ ��ü ����
			return TRUE;

		case IDC_BUTTON2:	// ���� ������ ��ư
			EndDialog(hDlg, IDCANCEL); // ��ȭ���� �ݱ�
			closesocket(sock); // ���� �ݱ�
			return TRUE;

		}
		return FALSE;
	}
	return FALSE;
}
// TCP Ŭ���̾�Ʈ ���� �κ�
DWORD WINAPI ClientMain(LPVOID arg)
{

	int retval;

	// ���� ����
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����
	int fsize = 0; // ���� ������
	int nsize = 0; // ���� �̸� ����
	int nowsize = 0; // ���� ���� ������
	int sendsize = 8000; // �ѹ��� ���� ������

	// ������ ������ ���
	// 
	// ������ �Է�(�ùķ��̼�)
	std::ifstream in{ filename,std::ios::binary };

	in.seekg(0, std::ios::end); // ������ �����ΰ���
	fsize = in.tellg(); // ������ ���� �о�´�
	in.seekg(0, std::ios::beg); // ������ ������ �ٽ� ����.

	nsize = strlen(filename);
	char* dt = new char[fsize];

	in.read(dt, fsize);

	// ������ ������(���� ����)
	retval = send(sock, (char*)&nsize, sizeof(int), 0);		// �����̸��� ũ�� ������
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	retval = send(sock, (char*)&fsize, sizeof(int), 0);		// ������ ũ�� ������
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ������ ������(���� ����)

	retval = send(sock, filename, nsize, 0);		// ������ �̸� ������
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	while (nowsize < fsize) {
		if (nowsize + sendsize <= fsize) {
			retval = send(sock, (char*)&dt[nowsize], sendsize, 0);						// ���� ������
		}
		else {
			retval = send(sock, (char*)&dt[nowsize], fsize % sendsize, 0);
		}
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		nowsize += sendsize;

		if (nowsize > fsize) nowsize = fsize;

		system("cls");
		printf("[TCP Ŭ���̾�Ʈ] ���۷� = %d%% [ %d / %d ]\n", (int)(((float)nowsize / (float)fsize) * 100), nowsize, fsize);

	}
	nowsize = 0;

	// ���� �ݱ�
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}