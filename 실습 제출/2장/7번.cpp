#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>

int Window_Size_X = 800;
int Window_Size_Y = 600;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 2-7";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

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
	HBRUSH hBrush, oldBrush;
	HBRUSH hBrush_t, oldBrush_t;
	static int x, y, z;
	static int rx, ry, rx_1, ry_1;
	static int tL, tT, tR, tB;
	static int mL, mT, mR, mB;
	static int oL, oT, oR, oB;
	static int pL, pT, pR, pB;
	
	static int ttL, ttT, ttR, ttB;
	static int mmL, mmT, mmR, mmB;
	static int ooL, ooT, ooR, ooB;
	static int ppL, ppT, ppR, ppB;

	static int count;
	static int Rc[30] = { 0, };
	static int Rstore[30] = { 0, };
	RECT rect = { rx,ry,rx_1,ry_1 };
	POINT tri[3] = { {tL - 100,tT},{tL - 150,tB},{tR - 150,tB} };
	POINT morae[5] = { {mL,mT - 130},{mR,mT - 130},{(mL + mR) / 2,mT - 90 },{mL,mT - 50},{mR,mT - 50} };
	POINT Five[5] = { {oR + 100,oT},{oR + 50,oT + 30},{oR + 70,oB},{oR + 130,oB},{oR + 150,oT + 30} };
	
	POINT temp_tri[3] = { {ttL - 100,tT},{ttL - 150,ttB},{ttR - 150,ttB} };
	POINT temp_morae[5] = { {mmL,mmT - 130},{mmR,mmT - 130},{(mmL + mmR) / 2,mmT - 90 },{mmL,mmT - 50},{mmR,mmT - 50} };
	POINT temp_Five[5] = { {ooR + 100,ooT},{ooR + 50,ooT + 30},{ooR + 70,ooB},{ooR + 130,ooB},{ooR + 150,ooT + 30} };

	static bool left = false;
	static bool right = false;
	static bool top = false;
	static bool bottom = false;

	switch (iMessage)
	{
	case WM_CREATE:
		tL = Window_Size_X / 2 - 50;
		mL = Window_Size_X / 2 - 50;
		oL = Window_Size_X / 2 - 50;
		pL = Window_Size_X / 2 - 50;

		tT = Window_Size_Y / 2 - 80;
		mT = Window_Size_Y / 2 - 80;
		oT = Window_Size_Y / 2 - 80;
		pT = Window_Size_Y / 2 - 80;

		tR = Window_Size_X / 2 + 50;
		mR = Window_Size_X / 2 + 50;
		oR = Window_Size_X / 2 + 50;
		pR = Window_Size_X / 2 + 50;

		tB = Window_Size_Y / 2;
		mB = Window_Size_Y / 2;
		oB = Window_Size_Y / 2;
		pB = Window_Size_Y / 2;

		ttL = tL + 150;
		ttT = tT;
		ttR = tR + 150;
		ttB = tB;

		ooL = oL - 150;
		ooT = oT;
		ooR = oR - 150;
		ooB = oB;

		mmL = mL;
		mmT = mT + 130;
		mmR = mR;
		mmB = mB + 130;

		srand(time(NULL));
		for (int i = 0; i < 30; i++)
		{
			Rc[i] = rand() % 255 + 1;
			Rstore[i] = Rc[i];
		}
		count = 4;
		x = 0;
		y = 0;
		z = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, Window_Size_X / 2 - 50, Window_Size_Y / 2 - 80, Window_Size_X / 2 + 50, Window_Size_Y / 2);

		if (left == true)
		{
			hBrush_t = CreateSolidBrush(RGB(Rc[3], Rc[4], Rc[5]));
			oldBrush_t = (HBRUSH)SelectObject(hdc, hBrush_t);
			Polygon(hdc, temp_tri, 3);
		}

		if (top == true)
		{
			hBrush = CreateSolidBrush(RGB(Rc[6], Rc[7], Rc[8]));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Polygon(hdc, temp_morae, 5);
		}

		if (right == true)
		{
			hBrush = CreateSolidBrush(RGB(Rc[9], Rc[10], Rc[11]));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Polygon(hdc, temp_Five, 5);
		}


		if (bottom == true)
		{
			hBrush = CreateSolidBrush(RGB(Rc[12], Rc[13], Rc[14]));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Pie(hdc, pL, pT  , pR, pB  , (pL + pR) / 2, pT  , pR, (pT + pB) / 2 );
		}


		hBrush_t = CreateSolidBrush(RGB(Rc[3], Rc[4], Rc[5]));
		oldBrush_t = (HBRUSH)SelectObject(hdc, hBrush_t);
		Polygon(hdc, tri, 3);

		hBrush = CreateSolidBrush(RGB(Rc[6], Rc[7], Rc[8]));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Polygon(hdc, morae, 5);

		hBrush = CreateSolidBrush(RGB(Rc[9], Rc[10], Rc[11]));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Polygon(hdc, Five, 5);

		hBrush = CreateSolidBrush(RGB(Rc[12], Rc[13], Rc[14]));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Pie(hdc, pL, pT + 130, pR, pB + 130, (pL + pR) / 2, pT + 130, pR, (pT + pB) / 2 + 130);

		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;

	case WM_KEYDOWN:
		switch (wParam) 
		{
		case VK_LEFT:
			for (int i = 3; i < 6; i++) {
				Rc[i] = rand() % 256;
			}
			left = true;
			right = false;
			top = false;
			bottom = false;
	
			break;
		case VK_RIGHT:
			for (int i = 9; i < 12; i++) {
				Rc[i] = rand() % 256;
			}

			right = true;
			left = false;
			top = false;
			bottom = false;
			break;
		case VK_UP:
			for (int i = 6; i < 9; i++) {
				Rc[i] = rand() % 256;
			}

			top = true;
			left = false;
			right = false;
			bottom = false;
			break;
		case VK_DOWN:
			for (int i = 12; i < 15; i++) {
				Rc[i] = rand() % 256;
			}

			bottom = true;
			left = false;
			right = false;
			top = false;
			break;
		}
	case WM_CHAR:

		if (wParam == 'q' || wParam == 'Q')
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}

		if (wParam == 'a' || wParam == 'A') {
			for (int i = 3; i < 6; i++) {
				Rc[i] = rand() % 255 + 1;
			}
		}
		if (wParam == 'w' || wParam == 'W') {
			for (int i = 6; i < 9; i++) {
				Rc[i] = rand() % 255 + 1;
			}
		}
		if (wParam == 'd' || wParam == 'D') {
			for (int i = 9; i < 12; i++) {
				Rc[i] = rand() % 255 + 1;
			}
		}
		if (wParam == 's' || wParam == 'S') {
			for (int i = 12; i < 15; i++) {
				Rc[i] = rand() % 255 + 1;
			}
		}

		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case  WM_KEYUP:
		hdc = GetDC(hWnd);
		for (int i = 0; i < 15; i++)
			Rc[i] = Rstore[i];

		InvalidateRect(hWnd, NULL, TRUE);
		ReleaseDC(hWnd, hdc);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}