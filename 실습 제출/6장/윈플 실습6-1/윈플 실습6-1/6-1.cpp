#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "resource.h"

BOOL SIN;
BOOL ZIG;
BOOL SPRING;
BOOL LINE;

BOOL MOVE_X;
BOOL MOVE_Y;

BOOL MovingRect;
BOOL Exit;
BOOL r, g, b;
BOOL Reverse;

int Window_Size_X = 800;
int Window_Size_Y = 600;
HINSTANCE g_hInst;	
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 6-1";
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
typedef struct po {
	int x;
	int y;
}po;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	COLORREF text_color;
	COLORREF bk_color;
	static HDC bkdc;
	static HBITMAP line;
	static int x, c;
	static int px = 0;
	static int py = 0;
	static int pc = 0;
	static int rv, ry;
	static int count;
	static po p[3600];
	static bool open;
	static int R = 0, G = 0, B = 0;

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 50, NULL);
		SIN = FALSE;
		ZIG = FALSE;
		SPRING = FALSE;
		MOVE_X = FALSE;
		MovingRect = FALSE;
		Exit = FALSE;
		Reverse = FALSE;
		r = FALSE;
		g = FALSE;
		b = FALSE;
		open = FALSE;
		rv = 1;
		ry = 1;
		count = 0;
		x = 0;
		c = 0;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//bkdc = CreateCompatibleDC(hdc);
		BitBlt(hdc, 40, 0, 720, 520, bkdc, 40, 40, SRCCOPY);
		DeleteDC(bkdc);

		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		bkdc = CreateCompatibleDC(hdc);
		line = CreateCompatibleBitmap(hdc, 800, 600);
		SelectObject(bkdc, line);

		Rectangle(bkdc, 40, 40, 760, 560);
		MoveToEx(bkdc, 0, 300, NULL);
		LineTo(bkdc, 800, 300);
		MoveToEx(bkdc, 400, 0, NULL);
		LineTo(bkdc, 400, 600);
		if (Exit) {
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		if (r) {
			R = 255;
		}
		else R = 0;

		if (g) {
			G = 255;
		}
		else G = 0;

		if (b) {
			B = 255;
		}
		else B = 0;

		if (!MovingRect) {
			x = 0;
			c = 0;
		}


		if (SIN) {
			for (int i = 0; i < 1800; i++) {
				p[i].x = i + 40;
				p[i].y = int(sin(i * 3.14 / 180) * 100) + 300;
			}
			for (int i = 0; i < 1800; i++) {
				if (Reverse) 
					SetPixel(bkdc, p[i].x + px, p[i].y + py, RGB(255 - R, 255 - G, 255 - B));
				else 
					SetPixel(bkdc, p[i].x + px, p[i].y + py, RGB(R, G, B));
			}
			if (MOVE_X) {
				px -= 5;
				if (p[360].x + px == 40) {
					px = 0;
				}
			}

			if (MovingRect == TRUE && MOVE_X == FALSE) {
				if (c == 0) {
					x = 0;
					for (int i = 0; i < 1800; i++) {
						if (p[i].x + px == 40) {
							x = i;
							c = 1;
						}
					}
				}
				Rectangle(bkdc, p[x].x + px - 10, p[x].y - 10, p[x].x + px + 10, p[x].y + 10);
				x += 5;
			}
		}
		else if (ZIG) {
			for (int i = 0; i < 1800; i++) {
				count++;
				if (count == 50) {
					rv++;
					count = 0;
				}
				if (rv % 2 == 0) {
					ry++;
				}
				else if (rv % 2 == 1)ry--;
				p[i].x = 40 + i;
				p[i].y = 340 + (2 * ry);
			}
			for (int i = 0; i < 1800; i++) {
				if (Reverse) SetPixel(bkdc, p[i].x + px, p[i].y, RGB(255 - R, 255 - G, 255 - B));
				else SetPixel(bkdc, p[i].x + px, p[i].y, RGB(R, G, B));
			}
			if (MOVE_X) {
				px -= 5;
				if (p[360].x + px == 40) {
					px = 0;
				}
			}
			if (MovingRect == TRUE && MOVE_X == FALSE) {
				if (c == 0) {
					x = 0;
					for (int i = 0; i < 1800; i++) {
						if (p[i].x + px == 40) {
							x = i;
							c = 1;
						}
					}
				}
				Rectangle(bkdc, p[x].x + px - 10, p[x].y - 10, p[x].x + px + 10, p[x].y + 10);
				x += 5;
			}
		}
		else if (SPRING) {
			for (int i = 0, r = 0; i < 1800; i++) {
				p[i].x = int(cos(i * 3.14 / 180) * (180)) + 40 + i;
				p[i].y = int(sin(i * 3.14 / 180) * 180) + 300;
				if (i % 180 == 1) r++;

			}
			for (int i = 0; i < 1800; i++) {
				if (Reverse) SetPixel(bkdc, p[i].x + px, p[i].y, RGB(255 - R, 255 - G, 255 - B));
				else SetPixel(bkdc, p[i].x + px, p[i].y, RGB(R, G, B));

			}
			if (MOVE_X) {
				px -= 5;
				if (p[360].x + px == 40) {
					px = 0;
				}
			}
			if (MovingRect == TRUE && MOVE_X == FALSE) {
				if (c == 0) {
					x = 0;
					for (int i = 0; i < 1800; i++) {
						if (p[i].x + px == 40) {
							x = i;
							c = 1;
						}
					}
				}
				Rectangle(bkdc, p[x].x + px - 10, p[x].y - 10, p[x].x + px + 10, p[x].y + 10);
				x += 5;
			}
		}
		else if (LINE) {
			for (int i = 0; i < 1800; i++) {
				p[i].x = i + 40;
				p[i].y = 400;

				if (Reverse) SetPixel(bkdc, p[i].x + px, p[i].y, RGB(255 - R, 255 - G, 255 - B));
				else SetPixel(bkdc, p[i].x + px, p[i].y, RGB(R, G, B));
			}

			for (int i = 0; i < 1800; i++) {
				p[i].x = i + 40;
				p[i].y = 200;
				if (Reverse) SetPixel(bkdc, p[i].x + px, p[i].y, RGB(255 - R, 255 - G, 255 - B));
				else SetPixel(bkdc, p[i].x + px, p[i].y, RGB(R, G, B));
			}

			if (MovingRect == TRUE && MOVE_X == FALSE) {
				if (c == 0) {
					x = 0;
					for (int i = 0; i < 1800; i++) {
						if (p[i].x + px == 40) {
							x = i;
							c = 1;
						}
					}
				}
				Rectangle(bkdc, p[x].x + px - 10, p[x].y - 10, p[x].x + px + 10, p[x].y + 10);
				Rectangle(bkdc, p[x].x + px - 10, p[x].y + 190, p[x].x + px + 10, p[x].y + 210);

				x += 5;
			}

		}
		
		if (open != TRUE) {
			open = TRUE;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DIalog_Proc);
		}
		ReleaseDC(hWnd, hdc);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		//DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DIalog_Proc);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
BOOL CALLBACK DIalog_Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1: //sin
			SIN = TRUE;
			ZIG = FALSE;
			SPRING = FALSE;
			LINE = FALSE;
			break;
		case IDC_RADIO2: //zigzag
			ZIG = TRUE;
			SIN = FALSE;
			SPRING = FALSE;
			LINE = FALSE;
			break;
		case IDC_RADIO3: // spring
			SPRING = TRUE;
			SIN = FALSE;
			ZIG = FALSE;
			LINE = FALSE;
			break;
		case IDC_RADIO4: // line
			LINE = TRUE;
			SIN = FALSE;
			SPRING = FALSE;
			ZIG = FALSE;
			break;
		case IDC_BUTTON1: // Move_X
			MOVE_X = TRUE;
			MovingRect = FALSE;
			break;
		case IDC_BUTTON4: // Move_Y
			MOVE_Y = TRUE;
			MovingRect = FALSE;
			break;
		case IDC_BUTTON2: // Stop
			MOVE_X = FALSE;
			break;
		case IDC_BUTTON5: // Reset
			SIN = FALSE;
			ZIG = FALSE;
			SPRING = FALSE;
			LINE = FALSE;
			MOVE_X = FALSE;
			MovingRect = FALSE;
			break;
		case IDC_BUTTON3: //도형 움직임
			if (MOVE_X) {
				MovingRect = FALSE;
			}
			else
				MovingRect = TRUE;
			break;
		case IDCANCEL: //EXIT
			EndDialog(hDlg, 0);
			Exit = TRUE;
			break;
		case IDCANCEL2: //RESET
			SIN = FALSE;
			ZIG = FALSE;
			SPRING = FALSE;
			LINE = FALSE;
			MOVE_X = FALSE;
			MovingRect = FALSE;
			break;
		case IDC_CHECK1: //Red			
			if (r) {
				r = FALSE;
			}
			else r = TRUE;
			break;
		case IDC_CHECK2: //Green
			if (g) {
				g = FALSE;
			}
			else g = TRUE;
			break;
		case IDC_CHECK3: //Blue
			if (b) {
				b = FALSE;
			}
			else b = TRUE;
			break;
		case IDC_CHECK4: //Reverse
			if (Reverse) {
				Reverse = FALSE;
			}
			else Reverse = TRUE;
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	}
	return 0;
}