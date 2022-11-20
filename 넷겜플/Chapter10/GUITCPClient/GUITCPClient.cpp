#include "..\..\Common.h"
#include "resource.h"
#include <iostream>
#include <fstream>
#include "ShlObj.h"

#define SERVERPORT 9000
#define BUFSIZE    1024
#define EditBUFSIZE 512

char* SERVERIP = (char*)"127.0.0.1";


INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ClientMain(LPVOID arg);

//void DisplayText(const char *fmt, ...);
//void DisplayError(const char *msg);


SOCKET sock;

HANDLE hReadEvent, hWriteEvent; // �̺�Ʈ

HWND hFolderButton; // ���� ã�� ��ư
HWND hSendButton; // ������ ��ư
HWND hEdit;	// ���� ���
HWND hProgress;
char Edit1buf[BUFSIZE + 1];	// ���� ��� ũ��
char filename[BUFSIZE + 1]; // ���� �̸�

OPENFILENAME Ofn;
TCHAR szPathname[BUFSIZE + 1] = { 0, };

bool SEND = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// �̺�Ʈ ����
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// ���� ��� ������ ����
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	// ��ȭ���� ����
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	// �̺�Ʈ ����
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

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
		hEdit = GetDlgItem(hDlg, IDC_EDIT1);

		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			memset(&Ofn, 0, sizeof(OPENFILENAME));
			Ofn.lStructSize = sizeof(OPENFILENAME);
			Ofn.hwndOwner = hFolderButton;
			Ofn.lpstrFilter;
			Ofn.lpstrFile = szPathname;
			Ofn.nMaxFile = MAX_PATH - 4;
			Ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

			if (GetOpenFileName(&Ofn)) {
				SetDlgItemText(hDlg, IDC_EDIT1, szPathname);
			}

			return TRUE;

		case IDC_BUTTON2:
			EnableWindow(hSendButton, FALSE); // ������ ��ư ��Ȱ��ȭ
			WaitForSingleObject(hReadEvent, INFINITE); // �б� �Ϸ� ���
			SetEvent(hWriteEvent); // ���� �Ϸ� �˸�
			GetDlgItemTextA(hDlg, IDC_EDIT1, Edit1buf, EditBUFSIZE + 1);
			memcpy(filename, Edit1buf, sizeof(Edit1buf));
			SetFocus(hEdit); // Ű���� ��Ŀ�� ��ȯ
			SendMessage(hEdit, EM_SETSEL, 0, -1); // �ؽ�Ʈ ��ü ����
			return TRUE;
		case IDCANCEL:
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
	// ������ ��ſ� ����� ����
	int fsize = 0; // ���� ������
	int nsize = 0; // ���� �̸� ����
	int nowsize = 0; // ���� ���� ������
	int sendsize = 2000; // �ѹ��� ���� ������
	int retval;
	int now = 0;

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

	while (1) {
		WaitForSingleObject(hWriteEvent, INFINITE); // ���� �Ϸ� ���

		// ������ �Է�(�ùķ��̼�)
		std::ifstream in{ szPathname,std::ios::binary };

		if (!in) {
			//	printf("������ ���ų� ���� �̸��� �߸� �Ǿ����ϴ�.\n");
			exit(-1);
		}

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
		}
		nowsize = 0;
		EnableWindow(hSendButton, TRUE); // ������ ��ư ��Ȱ��ȭ
		SetEvent(hReadEvent); // �б� �Ϸ� �˸�
	}
	return 0;
}
