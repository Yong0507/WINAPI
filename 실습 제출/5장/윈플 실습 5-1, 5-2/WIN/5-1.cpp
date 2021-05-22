#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
//#include "Resource.h"
#include "Resource_Bit.h"


#define Window_Size_X 915
#define Window_Size_Y 940
#define BLOCK_SIZE 20

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 4-3";
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
    static HDC hdc;
  
    //BitMAp
    HDC BackDC;
    HDC MemDC;
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;

    static HBITMAP hBitmap;
    HBITMAP oldBitmap;
    static HBITMAP backBit = NULL;

    static BITMAP bmp;

    static int hBitx;
    static int hBity;

    static RECT crt;

    static bool state;

    static int paintX;
    static int paintY;

    static int parts;

    static int parts2x[4] = {0,0,450,450};
    static int parts2y[4] = {0,450,450,0};

    static int parts3x[9] = {0,0,0,300,300,300,600,600,600};
    static int parts3y[9] = { 0,300,600,0,300,600,0,300,600 };

    static int MoveX;

    static bool check2[4];
    static bool check3[9];


    static RECT Part1;

    static int mx, my;

    switch (uMsg) {

    case WM_CREATE:

        //hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE("zoro.bmp"));
        parts = 1;

        hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        hBitx = bmp.bmWidth;
        hBity = bmp.bmHeight;

        paintX = bmp.bmWidth;
        paintY = bmp.bmHeight;

        break;
    case WM_CHAR:
        if (wParam == 'r')
        {
            state = !state;
        }
        if (wParam == 'a')
        {
            if (paintX == 900) {
                paintX = bmp.bmWidth;
                paintY = bmp.bmHeight;
            }
            else {
                paintX = 900;
                paintY = 900;

            }
        }
        if (wParam == 'q')
            exit(0);
            break;
        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_KEYDOWN:
        if (wParam == '1')
        {
            parts = 1;
        }
        if (wParam == '2')
        {
            parts = 2;
        }
        if (wParam == '3')
        {
            parts = 3;
        }

        if (wParam == VK_LEFT) {
            MoveX--;
        }

        if (wParam == VK_RIGHT) {
            MoveX++;

        }

        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        }
        break;

    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        for (int i = 0; i < 4; ++i) check2[i] = false;
        for (int i = 0; i < 9; ++i) check3[i] = false;
        MoveX = 0;
        if (parts == 2) {
            for (int i = 0; i < 4; ++i) {
                if (mx > parts2x[i] && mx < parts2x[i] + 450 && my > parts2y[i] && my < parts2y[i] + 450)
                    check2[i] = true;
            }
        }

        else if (parts == 3) {
            for (int i = 0; i < 9; ++i) {
                if (mx > parts3x[i] && mx < parts3x[i] + 300 && my > parts3y[i] && my < parts3y[i] +300)
                    check3[i] = true;
            }
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &crt);  // 그릴 영역 crt에 받아오기

        BackDC = CreateCompatibleDC(hdc);   // 더블 버퍼링 용 hdc < - back <- rectangle
        MemDC = CreateCompatibleDC(BackDC);   // 더블 버퍼링 용  back <- mdc  <- 비트맵 이미지용
        //이미지 넣을때는 DC 하나를 만든다음에 그림 을 넣고 내가 보일 화면에 복사를 한다.

        backBit = CreateCompatibleBitmap(hdc, 950, 950);   //도화지 준비

        SetBkMode(BackDC, WHITE_BRUSH);
        oldBitmap = (HBITMAP)SelectObject(BackDC, backBit); //도화지 세팅

        FillRect(BackDC, &crt, GetSysColorBrush(COLOR_WINDOW)); //그릴 영역 하얀색 할려고 ㅇㅇ
        ////////////////////////////////////////////////////

        hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        SelectObject(MemDC, hBitmap);   //MemDC에 루피 넣어 줬어요.


        if (parts == 1)
        {
            if (state)
                StretchBlt(BackDC, MoveX, 0, paintX, paintY, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
            else if (!state)
                StretchBlt(BackDC, MoveX, 0, paintX, paintY, MemDC, 0, 0, hBitx, hBity, SRCCOPY);

        }
        if (parts == 2)
        {
            for (int i = 0; i < 4; ++i) {
                if (i % 2 == 0) {
                    if(check2[i])
                        StretchBlt(BackDC, parts2x[i]+MoveX, parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
                    else
                        StretchBlt(BackDC, parts2x[i], parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, SRCCOPY);

                }
                else {
                    if (check2[i])
                        StretchBlt(BackDC, parts2x[i]+MoveX, parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
                    else
                        StretchBlt(BackDC, parts2x[i], parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);

                }
            }            
        }
        if (parts == 3)
        {
            for (int i = 0; i < 9; ++i) {
                if (i % 2 == 0) {
                    if (check3[i])
                        StretchBlt(BackDC, parts3x[i]+ MoveX, parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
                    else
                        StretchBlt(BackDC, parts3x[i], parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, SRCCOPY);

                }
                else {
                    if (check3[i])
                        StretchBlt(BackDC, parts3x[i]+ MoveX, parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
                    else
                        StretchBlt(BackDC, parts3x[i], parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);

                }
            }

        }


        if (parts == 1) {
            if (MoveX < 0) {
                if (state)
                    StretchBlt(BackDC, 900 + MoveX, 0, paintX, paintY, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
                else if (!state)
                    StretchBlt(BackDC, 900 + MoveX, 0, paintX, paintY, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
            }

            if (paintX + MoveX > 900) {
                if (state)
                    StretchBlt(BackDC, MoveX, 0, paintX, paintY, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
                else if (!state)
                    StretchBlt(BackDC, -900 + MoveX, 0, paintX, paintY, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
            }
        }
        if (parts == 2) {
            for (int i = 0; i < 4; ++i) {
                if (check2[i] == true) {
                    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    oldBrush = (HBRUSH)SelectObject(BackDC, hBrush);
                    hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
                    oldPen = (HPEN)SelectObject(BackDC, hPen);

                    Rectangle(BackDC, parts2x[i] + MoveX, parts2y[i], parts2x[i]+450 + MoveX, parts2y[i]+450);
                    if (parts2x[i] + MoveX < 0 ) {
                        if(i == 0 || i == 2)
                            StretchBlt(BackDC, 900 - parts2x[i] + MoveX, parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
                        else 
                            StretchBlt(BackDC, 900 - parts2x[i] + MoveX, parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
                        
                    }
                    if (parts2x[i] + MoveX + 450 > 900) {
                        if (i == 0 || i == 2)
                            StretchBlt(BackDC, -450 + MoveX, parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
                        else
                            StretchBlt(BackDC, -450 + MoveX, parts2y[i], 450, 450, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
                    }

                    SelectObject(BackDC, oldPen);
                    DeleteObject(hPen);               
                    SelectObject(BackDC, oldBrush);
                    DeleteObject(hBrush);                 
                }
            }
        }
     
        if (parts == 3) {
            for (int i = 0; i < 9; ++i) {
                if (check3[i] == true) {
                    hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                    oldBrush = (HBRUSH)SelectObject(BackDC, hBrush);
                    hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
                    oldPen = (HPEN)SelectObject(BackDC, hPen);

                    Rectangle(BackDC, parts3x[i] + MoveX, parts3y[i], parts3x[i] + 300 + MoveX, parts3y[i] + 300);
                    if (parts3x[i] + MoveX < 0) {
                        if (i % 2== 0)
                            StretchBlt(BackDC, 900 - parts3x[i] + MoveX, parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
                        else
                            StretchBlt(BackDC, 900 - parts3x[i] + MoveX, parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);

                    }
                    if (parts3x[i] + MoveX + 300 > 900) {
                        if (i % 2 == 0)
                            StretchBlt(BackDC, -300 + MoveX, parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, SRCCOPY);
                        else
                            StretchBlt(BackDC, -300 + MoveX, parts3y[i], 300, 300, MemDC, 0, 0, hBitx, hBity, NOTSRCCOPY);
                    }

                    SelectObject(BackDC, oldPen);
                    DeleteObject(hPen);
                    SelectObject(BackDC, oldBrush);
                    DeleteObject(hBrush);
                }
            }
        }



        ////////////////////////////////////////////////////

        BitBlt(hdc, 0, 0, 950, 950, BackDC, 0, 0, SRCCOPY);        
        
    

        DeleteObject(SelectObject(MemDC, hBitmap));
        DeleteObject(MemDC);
        DeleteObject(SelectObject(BackDC, oldBitmap));
        DeleteDC(BackDC);


        //back -> hdc
        // 결론적으로 한번만 그리는걸로하자.

        EndPaint(hWnd, &ps);

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}