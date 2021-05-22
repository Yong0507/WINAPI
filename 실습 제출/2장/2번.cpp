#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <random>
#include <string>
#include <time.h>

using namespace std;


#define Window_Size_X 3000
#define Window_Size_Y 3000

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Programming";
LPCTSTR lpszWindowName = L"window program 2-2";

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

    int rand_posX{};
    int rand_posY{};
    int rand_num{};
    int rand_count{};

    int rand_R{};
    int rand_G{};
    int rand_B{};

    int temp{};
    RECT rect{};

    switch (uMsg) {
    case WM_CREATE:
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        srand((unsigned)time(NULL));

        rand_posX = rand() % 700;
        rand_posY = rand() % 500;
        rand_num = rand() % 10;
        rand_count = rand() % 80 + 20;
        
        rand_R = rand() % 256;
        rand_G = rand() % 256;
        rand_B = rand() % 256;

        // 숫자 출력 확인용 //   
        TCHAR check[50];
        wsprintf(check, TEXT("pos_X : %d , pos_Y : %d , num : %d , count : %d"), rand_posX, rand_posY, rand_num, rand_count);
        TextOut(hdc, 30, 30, check, lstrlen(check));

        // 박스 위치 랜덤값에 설정
        rect.left = rand_posX;
        rect.top = rand_posY;
        rect.right = rand_posX + 800;
        rect.bottom = rand_posY + 800;

        temp = rand_count;

        TCHAR str[10100];

        for (int i = 0; i < (rand_count) * (rand_count)+rand_count; ++i) {
            str[i] = rand_num + 48;
            if (i % rand_count == 0) str[i] = '\n';
        }

        for (int i = 1; i < rand_count; ++i) {
            str[temp * i] = 10;
        }


        SetBkColor(hdc, RGB(rand_R, rand_G, rand_B));
        SetTextColor(hdc, RGB(rand_G, rand_B, rand_R));
        DrawText(hdc, str, (rand_count) * (rand_count)+rand_count, &rect, DT_LEFT);

        EndPaint(hWnd, &ps);

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}