#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>

#define Window_Size_X 900
#define Window_Size_Y 700

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 2-5";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

POINT test(POINT point1[4], RECT rect, int count);


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
    WndClass.hbrBackground =
        (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassEx(&WndClass);
    hWnd = CreateWindow
    (lpszClass, lpszWindowName,
        WS_OVERLAPPEDWINDOW,
        0, 0, Window_Size_X, Window_Size_Y,
        NULL, (HMENU)NULL,
        hInstance, NULL);
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
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR str[] = L"-----------------";
    static RECT rect[100];

    static POINT point1[5];
    static POINT point2[5];
    static POINT point3[3];
    static POINT point4[4];
    static POINT point5[4];

    static int width;
    static int height;
    static int count;
    static int sum;

    static int random;

    HPEN hPen, oldPen;


    switch (uMsg) {
    case WM_CREATE:

        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        srand((unsigned)time(NULL));

        width = rand() % 8 + 2;
        height = rand() % 8 + 2;

        //sum = width * height;

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {

                rect[count].left = (800 / width) * i;
                rect[count].top = (600 / height) * j;
                rect[count].right = (800 / width) * (i + 1);
                rect[count].bottom = (600 / height) * (j + 1);
                
                random = rand() % 6;

                if (random == 0) 
                {
                    point1[0].x = rect[count].left + 10;
                    point1[0].y = rect[count].top + 10;
                    point1[1].x = rect[count].right - 10;
                    point1[1].y = rect[count].top + 10;
                    point1[2].x = (rect[count].right + rect[count].left) / 2;
                    point1[2].y = (rect[count].top + rect[count].bottom) / 2;
                    point1[3].x = rect[count].left + 10;
                    point1[3].y = rect[count].bottom - 10;
                    point1[4].x = rect[count].right - 10;
                    point1[4].y = rect[count].bottom - 10;
                    Polygon(hdc, point1, 5);
                }

                else if (random == 1) 
                {
                    point2[0].x = rect[count].left + 10;
                    point2[0].y = rect[count].top + 10;
                    point2[1].x = rect[count].left + 10;
                    point2[1].y = rect[count].bottom - 10;
                    point2[2].x = (rect[count].right + rect[count].left) / 2;
                    point2[2].y = (rect[count].top + rect[count].bottom) / 2;
                    point2[3].x = rect[count].right - 10;
                    point2[3].y = rect[count].top + 10;
                    point2[4].x = rect[count].right - 10;
                    point2[4].y = rect[count].bottom - 10;
                    Polygon(hdc, point2, 5);
                }

                else if (random == 2)
                {
                    point3[0].x = rect[count].left + 10;
                    point3[0].y = rect[count].top + 10;
                    point3[1].x = rect[count].right - 10;
                    point3[1].y = rect[count].top + 10;
                    point3[2].x = (rect[count].right + rect[count].left) / 2;
                    point3[2].y = rect[count].bottom - 10;
                    Polygon(hdc, point3, 3);
                }

                else if (random == 3)
                {
                    point4[0].x = (rect[count].right + rect[count].left) / 2;
                    point4[0].y = rect[count].top + 10;
                    point4[1].x = rect[count].left + 10;
                    point4[1].y = (rect[count].top + rect[count].bottom) / 2;
                    point4[2].x = (rect[count].right + rect[count].left) / 2;
                    point4[2].y = rect[count].bottom - 10;
                    point4[3].x = rect[count].right - 10;
                    point4[3].y = (rect[count].top + rect[count].bottom) / 2;
                    Polygon(hdc, point4, 4);
                }

                else if (random == 4)
                {
                    point5[0].x = rect[count].left + 10;
                    point5[0].y = rect[count].top + 10;
                    point5[1].x = rect[count].left + 10;
                    point5[1].y = rect[count].bottom - 10;
                    point5[2].x = rect[count].right - 10;
                    point5[2].y = rect[count].bottom - 10;
                    point5[3].x = rect[count].right - 10;
                    point5[3].y = rect[count].top + 10;
                    Polygon(hdc, point5, 4);
                }

                else if (random == 5)
                {
                    hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
                    oldPen = (HPEN)SelectObject(hdc, hPen);

                    MoveToEx(hdc, rect[count].left + 10, rect[count].top + 10, NULL);
                    LineTo(hdc, rect[count].right - 10, rect[count].bottom - 10);
                    MoveToEx(hdc, rect[count].right - 10, rect[count].top + 10, NULL);
                    LineTo(hdc, rect[count].left + 10, rect[count].bottom - 10);

                    SelectObject(hdc, oldPen);
                    DeleteObject(hPen);
                }
                count++;
            }
        }

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}