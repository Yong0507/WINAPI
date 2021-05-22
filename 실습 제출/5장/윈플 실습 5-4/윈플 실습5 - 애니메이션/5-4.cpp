#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <atlimage.h>

#define Window_Size_X 1000
#define Window_Size_Y 1000

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 5-3";
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
    static HDC hdc, memdc1, memdc2;
    HBITMAP hBitmap1, hBitmap2;
    HBRUSH hBrush, oldBrush;

    // 이미지
    static CImage img, backimg;
    static int w, h;

    static int xPos, yPos;
    static int anim_moving;
    static int anim_dir;

    // 확대 및 축소 
    static float scale_rate;

    // 점프
    static int jump;
    static bool is_jump;

    // 쌍둥이
    static bool is_twin;
    static int twin_xPos, twin_yPos;
    static int anim_moving_twin;
    static int anim_dir_twin;

    // 확대 축소 타이머 시간 재기
    static int scale_time1;
    static int scale_time2;

    // 마우스 좌표
    static int LM_x;
    static int LM_y;
    static bool is_me_touch;
    static bool is_other_touch;
    static int touch_time;
    static int touch_time1;


    switch (uMsg) {

    case WM_CREATE:
        backimg.Load(L"back.jpg");
        img.Load(L"character.png");
        scale_rate = 1.f;
        twin_xPos = 100;
        break;
    case WM_TIMER:
        switch (wParam) {
        case 1:
            anim_dir = 128;
            anim_moving += 64;
            if (anim_moving == 576)
                anim_moving = 0;

            anim_dir_twin = 128;
            anim_moving_twin += 64;
            if (anim_moving_twin == 576)
                anim_moving_twin = 0;

            break;

        case 2:
            if (is_jump)
                jump += 5;
            else
                jump -= 5;

            if (jump == -20)
                is_jump = true;
            if (jump == 0) {
                is_jump = false;
                KillTimer(hWnd, 2);
            }
            break;
        case 3:
            scale_time1++;

            if (scale_time1 > 5) {
                scale_rate = 1.0f;
                KillTimer(hWnd, 3);
                scale_time1 = 0;
            }
            else {
                scale_rate = 1.5f;
            }
            break;
        case 4:
            scale_time2++;

            if (scale_time2 > 5) {
                scale_rate = 1.0f;
                KillTimer(hWnd, 4);
                scale_time2 = 0;
            }
            else {
                scale_rate = 0.7f;
            }
            break;

        case 5:
            if (is_me_touch)
            {
                is_other_touch = false;

                touch_time++;

                if (touch_time > 4) {
                    is_me_touch = false;
                    KillTimer(hWnd, 5);
                    touch_time = 0;
                }
                else {
                    xPos += 10;
                    yPos += 10;
                    anim_dir += 64;
                    if (anim_dir == 256)
                        anim_dir = 0;

                    //twin_xPos += 10;
                    //twin_yPos += 10;
                    anim_dir_twin += 64;
                    if (anim_dir_twin == 256)
                        anim_dir_twin = 0;
                }
            }

            if (is_other_touch)
            {
                is_me_touch = false;

                touch_time1++;

                if (touch_time1 > 4) {
                    is_other_touch = false;
                    KillTimer(hWnd, 5);
                    touch_time1 = 0;
                }
                else {
                    twin_xPos += 10;
                    twin_yPos += 10;
                    anim_dir_twin += 64;
                    if (anim_dir_twin == 256)
                        anim_dir_twin = 0;
                }
            }

            break;

        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_LBUTTONDOWN:
        LM_x = LOWORD(lParam);
        LM_y = HIWORD(lParam);
        if (LM_x > xPos && LM_x < xPos + 64 && LM_y > yPos && LM_y < yPos + 64)
        {
            is_me_touch = true;
            SetTimer(hWnd, 5, 60, NULL);
            KillTimer(hWnd, 1);
        }

        if (is_twin && LM_x > xPos + twin_xPos && LM_x < xPos + twin_xPos + 64 && LM_y > yPos + twin_yPos && LM_y < yPos + twin_yPos + 64)
        {
            is_other_touch = true;
            SetTimer(hWnd, 5, 60, NULL);
            KillTimer(hWnd, 1);
        }

        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
            KillTimer(hWnd, 1);
            anim_dir = 0;
            yPos -= 5;
            anim_moving += 64;
            if (anim_moving == 576)
                anim_moving = 0;

            anim_dir_twin = 0;
            anim_moving_twin += 64;
            if (anim_moving_twin == 576)
                anim_moving_twin = 0;

            break;
        case VK_LEFT:
            KillTimer(hWnd, 1);
            anim_dir = 64;
            xPos -= 5;
            anim_moving += 64;
            if (anim_moving == 576)
                anim_moving = 0;

            anim_dir_twin = 64;
            anim_moving_twin += 64;
            if (anim_moving_twin == 576)
                anim_moving_twin = 0;

            break;
        case VK_DOWN:
            KillTimer(hWnd, 1);
            anim_dir = 128;
            yPos += 5;
            anim_moving += 64;
            if (anim_moving == 576)
                anim_moving = 0;

            anim_dir_twin = 128;
            anim_moving_twin += 64;
            if (anim_moving_twin == 576)
                anim_moving_twin = 0;

            break;
        case VK_RIGHT:
            KillTimer(hWnd, 1);
            anim_dir = 192;
            xPos += 5;
            anim_moving += 64;
            if (anim_moving == 576)
                anim_moving = 0;

            anim_dir_twin = 192;
            anim_moving_twin += 64;
            if (anim_moving_twin == 576)
                anim_moving_twin = 0;

            break;
        case VK_SPACE:
            KillTimer(hWnd, 1);
            anim_dir = 0;
            anim_dir_twin = 0;
            SetTimer(hWnd, 2, 10, NULL);
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        SetTimer(hWnd, 1, 60, NULL);
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'e':
        case 'E':
            SetTimer(hWnd, 3, 100, NULL);
            break;
        case 's':
        case 'S':
            SetTimer(hWnd, 4, 100, NULL);
            break;
        case 't':
            is_twin = true;
            break;
        case 'T':
            is_twin = false;
            break;

        }

        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);


        w = backimg.GetWidth();
        h = backimg.GetHeight();

        memdc1 = CreateCompatibleDC(hdc);
        hBitmap1 = CreateCompatibleBitmap(hdc, Window_Size_X, Window_Size_Y);
        SelectObject(memdc1, hBitmap1);
        backimg.Draw(memdc1, 0, 0, Window_Size_X, Window_Size_Y, 0, 0, w, h);


        memdc2 = CreateCompatibleDC(memdc1);
        hBitmap2 = CreateCompatibleBitmap(memdc1, Window_Size_X, Window_Size_Y);
        SelectObject(memdc2, hBitmap2);

        img.Draw(memdc2, xPos, yPos, 64, 64, anim_moving, anim_dir, 64, 64);
        StretchBlt(memdc1, xPos, yPos + jump, 64 * scale_rate, 64 * scale_rate, memdc2, xPos, yPos, 64, 64, SRCCOPY);

        if (is_twin)
        {
            img.Draw(memdc2, xPos + twin_xPos, yPos + twin_yPos + jump, 64, 64, anim_moving_twin, anim_dir_twin, 64, 64);
            StretchBlt(memdc1, xPos + twin_xPos, yPos + twin_yPos + jump, 64 * scale_rate, 64 * scale_rate, memdc2, xPos + twin_xPos, yPos + twin_yPos + jump, 64, 64, SRCCOPY);

        }

        BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);

        DeleteObject(SelectObject(memdc1, hBitmap1));
        DeleteObject(SelectObject(memdc2, hBitmap2));
        DeleteDC(memdc1);
        DeleteDC(memdc2);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}