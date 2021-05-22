#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <random>
#include <time.h>

#define Window_Size_X 870
#define Window_Size_Y 740

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Programming";
LPCTSTR lpszWindowName = L"window program 2-1";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

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

	hWnd = CreateWindow(lpszClass,
		lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, Window_Size_X, Window_Size_Y,
		NULL, (HMENU)NULL,
		hInstance, NULL
	);
	
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	HDC hdc;
	PAINTSTRUCT ps;

	RECT rect_LT{ 0, 0, 100, 50 };
	RECT rect_RT{ 700, 0, 850, 50 };
	RECT rect_CT{ 350, 300, 500, 350 };
	RECT rect_LB{ 0 , 650, 150, 700};
	RECT rect_RB{ 700, 650, 850, 700 };

	TCHAR szText1[] = L"Left-top (0, 0)";
	TCHAR szText2[] = L"Right-top (700, 0)";
	TCHAR szText3[] = L"Center (350, 350)";
	TCHAR szText4[] = L"Left-bottom (0, 650)";
	TCHAR szText5[] = L"Right-bottom (700, 650)";

	int rand_R[5];
	int rand_G[5];
	int rand_B[5];

	switch (uMsg) {
	case WM_CREATE:
		break;
	case WM_PAINT:		
		srand((unsigned)time(NULL));
		for (int i = 0; i < 5; ++i) {
			rand_R[i] = rand() % 256;
			rand_G[i] = rand() % 256;
			rand_B[i] = rand() % 256;
		}
		hdc = BeginPaint(hWnd, &ps);
		
		SetBkColor(hdc, RGB(rand_R[0], rand_G[0], rand_B[0]));
		SetTextColor(hdc, RGB(rand_G[0], rand_B[0], rand_R[0]));
		DrawText(hdc, szText1, _tcslen(szText1), &rect_LT, DT_SINGLELINE | DT_TOP | DT_LEFT );
		
		SetBkColor(hdc, RGB(rand_R[1], rand_G[1], rand_B[1]));
		SetTextColor(hdc, RGB(rand_G[1], rand_B[1], rand_R[1]));
		DrawText(hdc, szText2, _tcslen(szText2), &rect_RT, DT_SINGLELINE | DT_TOP | DT_RIGHT );

		SetBkColor(hdc, RGB(rand_R[2], rand_G[2], rand_B[2]));
		SetTextColor(hdc, RGB(rand_G[2], rand_B[2], rand_R[2]));
		DrawText(hdc, szText3, _tcslen(szText3), &rect_CT, DT_SINGLELINE | DT_CENTER | DT_LEFT);

		SetBkColor(hdc, RGB(rand_R[3], rand_G[3], rand_B[3]));
		SetTextColor(hdc, RGB(rand_G[3], rand_B[3], rand_R[3]));
		DrawText(hdc, szText4, _tcslen(szText4), &rect_LB, DT_SINGLELINE | DT_BOTTOM | DT_LEFT);

		SetBkColor(hdc, RGB(rand_R[4], rand_G[4], rand_B[4]));
		SetTextColor(hdc, RGB(rand_G[4], rand_B[4], rand_R[4]));
		DrawText(hdc, szText5, _tcslen(szText5), &rect_RB, DT_SINGLELINE | DT_BOTTOM | DT_RIGHT);


		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;	
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}