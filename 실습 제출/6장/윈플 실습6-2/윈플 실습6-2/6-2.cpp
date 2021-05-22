#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "resource.h"

bool EXIT;

int Window_Size_X = 800;
int Window_Size_Y = 600;
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";
LPCTSTR lpszWindowName = "Window Programming 6-2";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DIalog_Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow
	(lpszClass,
		lpszWindowName,
		(WS_OVERLAPPEDWINDOW),
		0,
		0,
		Window_Size_X,
		Window_Size_Y,
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	COLORREF text_color;
	COLORREF bk_color;

	switch (iMessage)
	{
	case WM_CREATE:
		EXIT = FALSE;
		SetTimer(hWnd, 1, 50, NULL);
		DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DIalog_Proc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);



		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if (EXIT) {
			KillTimer(hWnd, 1);
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case WM_LBUTTONDOWN:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

/*
for (int i = 0; i < 50; i++) {
	if (cc==0)result = op[0];
}
_itoa_s(result, str, 10);
count = strlen(str);
SetDlgItemText(hDlg, IDC_EDIT1, str);
for (int i = 0; i < 100; i++) {
	op[i] = 0;
	num = 0;
	blank = 0;
	cur_n = 1;
}*/
BOOL CALLBACK DIalog_Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[100];
	static int count;
	static char tp[100];
	static int temp = 0;
	static int op[100] = { 0, };
	static int result;
	static int blank;
	static char cc;
	static char t;
	static int tc;
	static int set;
	switch (iMessage)
	{
	case WM_INITDIALOG:
		count = 0;
		blank = 0;
		result = 0;
		cc = 0;
		tc = 0;
		set = 0;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON20: // =
			if (strlen(str) > 2) {
				for (int i = 0; i < strlen(str) + 1; i++) {
					if (str[i] == '+' || str[i] == '*' || str[i] == '-' || str[i] == '/' || str[i] == NULL) {
						for (int j = blank; j < i; j++) {
							tp[temp] = str[j];
							temp++;
						}
						op[0] = atoi(tp);
						temp = 0;
						blank = i + 1;
						for (int t = 0; t < 100; t++) {
							tp[t] = 0;
						}
						if (cc == 0) {
							result = op[0];
							op[0] = 0;
							cc = str[i];
						}
						else if (cc == '+')
						{
							result += op[0];
							op[0] = 0;
							cc = str[i];
						}
						else if (cc == '-')
						{
							result -= op[0];
							op[0] = 0;
							cc = str[i];
						}
						else if (cc == '*')
						{
							result *= op[0];
							op[0] = 0;
							cc = str[i];
						}
						else if (cc == '/')
						{
							result /= op[0];
							op[0] = 0;
							cc = str[i];
						}
					}
				}
				_itoa_s(result, str, 10);
				count = strlen(str);
				temp = 0;
				cc = 0;
				result = 0;
				op[0] = 0;
				blank = 0;
				set = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;




		case IDC_NUM1:
			if (count < 100) {
				str[count++] = '1';
				str[count] = 0;
				SetDlgItemText(hDlg, IDC_EDIT1, str);
			}
			break;
		case IDC_NUM2:
			if (count < 100) {
				str[count++] = '2';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM3:
			if (count < 100) {
				str[count++] = '3';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM4:
			if (count < 100) {
				str[count++] = '4';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM5:
			if (count < 100) {
				str[count++] = '5';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM6:
			if (count < 100) {
				str[count++] = '6';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM7:
			if (count < 100) {
				str[count++] = '7';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM8:
			if (count < 100) {
				str[count++] = '8';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM9:
			if (count < 100) {
				str[count++] = '9';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_NUM0:
			if (count < 100) {
				str[count++] = '0';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
			//사칙연산
		case IDC_BUTTON_1: // 나누기
			if (count < 100)
			{
				str[count++] = '/';
				str[count] = 0;
				set = 1;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_2: // 곱하기
			if (count < 100)
			{
				str[count++] = '*';
				str[count] = 0;
				set = 1;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_3: // 빼기
			if (count < 100)
			{
				str[count++] = '-';
				str[count] = 0;
				set = 1;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_4: // 더하기
			if (count < 100)
			{
				str[count++] = '+';
				str[count] = 0;
				set = 1;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;

		case IDC_BUTTON_R:	//순서 바꾸기
			if (count > 0) {

				for (int i = 0; i < count / 2; i++)
				{
					t = str[i];
					str[i] = str[count - 1 - i];
					str[count - 1 - i] = t;
				}
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_CE: //마지막 입력값 삭제
			if (count > 0) {
				count--;
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_C:	//모두 삭제
			count = 0;
			str[count] = 0;
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_ROOT:	//2진수
			break;
		case IDC_BUTTON_Back://마지막 숫자 삭제
			if (set == 0) {
				if (count == 1) {
					count--;
					str[count] = '0';
				}
				else {
					count--;
					str[count] = 0;
				}
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_10:	 //입력된 숫자 10곱하기
			if (count > 0 && set == 0) {
				str[count++] = '0';
				str[count] = 0;
			}
			SetDlgItemText(hDlg, IDC_EDIT1, str);
			break;
		case IDC_BUTTON_Exit://종료
			EXIT = TRUE;
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return 0;
}