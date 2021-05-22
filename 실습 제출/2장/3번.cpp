#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <random>
#include <string>
#include <time.h>
#include <algorithm>
#include <stack>

using namespace std;


#define Window_Size_X 800
#define Window_Size_Y 600

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Programming";
LPCTSTR lpszWindowName = L"window program 2-3";

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

    static SIZE size;
    static TCHAR str[100];

    static stack <int> numbers;

    static int count = 0;

    static int temp;
    static bool check = false;

    RECT rect{};
    static int num1;
    static int num2;
    static int num3;
    static int newC;

    static int cnt;

    static TCHAR gugudan[10000];
    static TCHAR str_temp[10000];
    static TCHAR newLine[10000];



    switch (uMsg) {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 5, 15);
        ShowCaret(hWnd);
        count = 0;
        newC = 0;
        break;
    case WM_CHAR:

        if (wParam == VK_BACK && count > 0)
            count--;
        else {
            check = false;

            str[count++] = wParam;
        }
        str[count] = NULL;

        if (wParam == VK_RETURN)
        {
            temp = count;

            for (int i = 0; i < temp; ++i) {
                if (str[i] >= 48 && str[i] <= 57)
                {
                    if (str[i + 1] >= 48 && str[i + 1] <= 57 && count < 100)
                    {
                        if (str[i + 2] >= 48 && str[i + 2] <= 57 && count < 100)
                        {
                            numbers.push((str[i] - 48) * 100 + (str[i + 1] - 48) * 10 + str[i + 2] - 48);
                            i += 2;
                        }
                        else {
                            numbers.push((str[i] - 48) * 10 + str[i + 1] - 48);
                            i++;
                        }
                    }
                    else
                        numbers.push(str[i] - 48);
                }
            }

            for (int i = 0; i < temp; ++i) {
                str[i] = NULL;
            }

            count = 0;
            check = true;
            newC = 0;

            for (int i = 0; i < 10000; ++i) {
                gugudan[i] = NULL;
                newLine[i] = NULL;
            }

        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        GetTextExtentPoint32(hdc, str, lstrlen(str), &size);
        TextOut(hdc, 0, 0, str, lstrlen(str));

        if (check == true)
        {
            num3 = numbers.top();
            numbers.pop();
            num2 = numbers.top();
            numbers.pop();
            num1 = numbers.top();
            numbers.pop();

            rect.left = num1;
            rect.top = num2;
            rect.right = num1 + 500;
            rect.bottom = num2 + 500;

            if (num3 == 0)
                exit(1);

            // 숫자 출력 확인용 //   
            //TCHAR check[50];
            //wsprintf(check, TEXT("%d %d %d"), num1, num2, num3);
            //TextOut(hdc, 30, 30, check, lstrlen(check));                                    

            for (int i = 1; i < 20; ++i)
            {
                wsprintf(str_temp, TEXT("%d * %d = %d "), num3, i, num3 * i);
                wcscat(gugudan, str_temp);
            }

            //TextOut(hdc, num1, num2, gugudan, lstrlen(gugudan));
            for (int i = 0; i < 10000; ++i) {
                if (i > 10 && newC % 50 == 0) {
                    newLine[newC] = '\n';
                    i--;
                }
                else {
                    newLine[newC] = gugudan[i];
                }
                newC++;
            }
            DrawText(hdc, newLine, lstrlen(newLine), &rect, DT_LEFT);
        }

        SetCaretPos(size.cx, 0);

        EndPaint(hWnd, &ps);

        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}