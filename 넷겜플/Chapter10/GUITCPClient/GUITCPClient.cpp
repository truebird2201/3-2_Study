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

HANDLE hReadEvent, hWriteEvent; // 이벤트

HWND hFolderButton; // 파일 찾기 버튼
HWND hSendButton; // 보내기 버튼
HWND hEdit;	// 파일 경로
HWND hProgress;
char Edit1buf[BUFSIZE + 1];	// 파일 경로 크기
char filename[BUFSIZE + 1]; // 파일 이름

OPENFILENAME Ofn;
TCHAR szPathname[BUFSIZE + 1] = { 0, };

bool SEND = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 이벤트 생성
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	// 소켓 통신 스레드 생성
	CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

	// 대화상자 생성
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

	// 이벤트 제거
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);

	// 윈속 종료
	WSACleanup();
	return 0;
}

// 대화상자 프로시저
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
			EnableWindow(hSendButton, FALSE); // 보내기 버튼 비활성화
			WaitForSingleObject(hReadEvent, INFINITE); // 읽기 완료 대기
			SetEvent(hWriteEvent); // 쓰기 완료 알림
			GetDlgItemTextA(hDlg, IDC_EDIT1, Edit1buf, EditBUFSIZE + 1);
			memcpy(filename, Edit1buf, sizeof(Edit1buf));
			SetFocus(hEdit); // 키보드 포커스 전환
			SendMessage(hEdit, EM_SETSEL, 0, -1); // 텍스트 전체 선택
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL); // 대화상자 닫기
			closesocket(sock); // 소켓 닫기
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


// TCP 클라이언트 시작 부분
DWORD WINAPI ClientMain(LPVOID arg)
{
	// 데이터 통신에 사용할 변수
	int fsize = 0; // 파일 사이즈
	int nsize = 0; // 파일 이름 길이
	int nowsize = 0; // 현재 보낸 사이즈
	int sendsize = 2000; // 한번에 보낼 사이즈
	int retval;
	int now = 0;

	// 소켓 생성
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
		WaitForSingleObject(hWriteEvent, INFINITE); // 쓰기 완료 대기

		// 데이터 입력(시뮬레이션)
		std::ifstream in{ szPathname,std::ios::binary };

		if (!in) {
			//	printf("파일이 없거나 파일 이름이 잘못 되었습니다.\n");
			exit(-1);
		}

		in.seekg(0, std::ios::end); // 파일의 끝으로간다
		fsize = in.tellg(); // 파일의 끝을 읽어온다
		in.seekg(0, std::ios::beg); // 파일의 앞으로 다시 간다.

		nsize = strlen(filename);
		char* dt = new char[fsize];

		in.read(dt, fsize);
		// 데이터 보내기(고정 길이)
		retval = send(sock, (char*)&nsize, sizeof(int), 0);		// 파일이름의 크기 보내기
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		retval = send(sock, (char*)&fsize, sizeof(int), 0);		// 파일의 크기 보내기
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		// 데이터 보내기(가변 길이)

		retval = send(sock, filename, nsize, 0);		// 파일의 이름 보내기
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
		while (nowsize < fsize) {
			if (nowsize + sendsize <= fsize) {
				retval = send(sock, (char*)&dt[nowsize], sendsize, 0);						// 파일 보내기
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
		EnableWindow(hSendButton, TRUE); // 보내기 버튼 비활성화
		SetEvent(hReadEvent); // 읽기 완료 알림
	}
	return 0;
}
