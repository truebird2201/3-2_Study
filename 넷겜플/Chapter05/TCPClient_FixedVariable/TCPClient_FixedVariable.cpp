#include "..\..\Common.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <array>
#include <Windows.h>
#include "resource.h"

#define SERVERPORT 9000
#define BUFSIZE    50

char* SERVERIP = (char*)"127.0.0.1";
char* filename = (char*)"Rolling.mp4";

INT_PTR CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI ClientMain(LPVOID arg);

SOCKET sock;

HANDLE hReadEvent, hWriteEvent; // 이벤트

HWND hFolderButton; // 파일 찾기 버튼
HWND hSendButton; // 보내기 버튼
HWND hProgress; // 진행 상황

// 데이터 통신에 사용할 변수
int fsize = 0; // 파일 사이즈
int nsize = 0; // 파일 이름 길이
int nowsize = 0; // 현재 보낸 사이즈
int sendsize = 8000; // 한번에 보낼 사이즈

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
		hProgress = GetDlgItem(hDlg, IDC_PROGRESS1);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDOK:	// 파일 선택 버튼
			return TRUE;

		case IDCANCEL:	// 파일 보내기 버튼
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

	int retval;

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

	// 서버와 데이터 통신
	// 
	// 데이터 입력(시뮬레이션)
	std::ifstream in{ filename,std::ios::binary };

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

		system("cls");
		printf("[TCP 클라이언트] 전송률 = %d%% [ %d / %d ]\n", (int)(((float)nowsize / (float)fsize) * 100), nowsize, fsize);

	}
	nowsize = 0;

	// 소켓 닫기
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}