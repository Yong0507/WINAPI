#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "resource.h"
#define BSIZE 8

int Window_Size_X = 1260;
int Window_Size_Y = 800;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 5-6";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProc(HWND hWnd, UINT iMessage, UINT idEvent, DWORD dwTime);

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


float LengthPts(int x1, int y1, int x2, int y2) {
    return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL inCircle(int x, int y, int x2, int y2) {
    if (LengthPts(x, y, x2, y2) < BSIZE) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc,mdc;
    HDC packdc, bkdc;
    static HBITMAP pack,pack_mask,bk,oldbit1,oldbit2,hbit;
    static HBITMAP c,c_mask;
    PAINTSTRUCT ps;
    COLORREF text_color;
    COLORREF bk_color;
    static int click;
    static int x, y;
    static int tc1, tc2;
    static int LR, TB;
    static int mx, my;
    static int location;
    static int speed;
    static int speed_c;
    static BOOL Selection;
    static BOOL Selection_s;
    static RECT food[20];
    static int food_r[20];
    static int food_g[20];
    static int food_b[20];
    static int foodc=0;
    static int fmove[20] = { 0, };
    static int up[4] = { 0, };
    static int Rx[10] = { 0, };
    static int Ry[10] = { 0, };
    static int w[10] = { 0, };
    static int make;
    static int timer;
    static int baby[5] = { 0, };
    static int J, K, L, M, N;
    static int RC;
    static int Jc[5] = { 0, };
    static int stop;
    static int Jump;
    static int ani;
    static int ani_m;
    static int look[4] = { 1,1,1,1 };
    static int k, kc=0;
    static int tx[4] = { 0, };
    static int ty[4] = { 0, };
    static int t = 0;
    static HBRUSH hBrush, oldBrush;


    switch (iMessage)
    {
    case WM_CREATE:
        click = 0;
        ani_m = 0;
        ani = 0;
        pack = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        pack_mask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        c = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        c_mask = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
        bk = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        srand(unsigned(time(NULL)));
        for (int i = 0; i < 20; ++i) {
            Rx[i] = (rand() % 7 * 16) + 192;
            Ry[i] = (rand() % 7 * 16) + 192;
        }
        for (int i = 0; i < 20; ++i) {
            food_r[i] = rand() % 255;
            food_g[i] = rand() % 255;
            food_b[i] = rand() % 255;
        }

        Jump = 0;
        stop = 0;
        x = 0;
        y = 10;
        LR = 0;
        TB = 0;
        mx = 0;
        my = 0;
        location = 0;
        speed = 80;
        speed_c = 0;
        Selection = FALSE;
        Selection_s = FALSE;
        make = 0;
        timer = 0;
        RC = 0;
        J = 9;
        K = 9;
        L = 9;
        M = 9;
        SetTimer(hWnd, 1, speed, NULL);
        SetTimer(hWnd, 5, 1500, NULL);
        SetTimer(hWnd, 6, 150, NULL);
        SetTimer(hWnd, 15, 50, NULL);
        break;
    case WM_TIMER:
        switch (wParam) {
        case 15:           
            hdc = GetDC(hWnd);
            if (hbit == NULL) hbit = CreateCompatibleBitmap(hdc, 650, 650);
            packdc = CreateCompatibleDC(hdc);
            bkdc = CreateCompatibleDC(packdc);
            oldbit1 = (HBITMAP)SelectObject(packdc, hbit);
            oldbit2 = (HBITMAP)SelectObject(bkdc, bk);
            StretchBlt(packdc, 0, 0, 650, 650, bkdc, 0, 0, 500, 970, SRCCOPY);

            if (click > 0 && click <= 10) {
                oldbit2 = (HBITMAP)SelectObject(bkdc, c_mask);
                StretchBlt(packdc, LR + up[0] + look[1], TB + 10 + up[1], (16 + up[2]) * look[0], 16 + up[3], bkdc, 30, 10, 50, 60, SRCAND);
                oldbit2 = (HBITMAP)SelectObject(bkdc, c);
                StretchBlt(packdc, LR + up[0] + look[1], TB + 10 + up[1], (16 + up[2]) * look[0], 16 + up[3], bkdc,30, 10, 50,60, SRCPAINT);
                click--;
            }
            else {
                oldbit2 = (HBITMAP)SelectObject(bkdc, pack_mask);
                StretchBlt(packdc, LR + up[0] + look[1], TB + 10 + up[1]+look[3], (16+up[2]) * look[0], (16 + up[3])*look[2], bkdc, ani_m, 0, 50, 40, SRCAND);
                for (int i = 0; i < t; i++) {
                    StretchBlt(packdc, tx[i] + up[0] + look[1], ty[i] + 10 + up[1], (16 + up[2]) * look[0], 16 + up[3], bkdc, ani_m, 0, 50, 40, SRCAND);
                }
                oldbit2 = (HBITMAP)SelectObject(bkdc, pack);
                StretchBlt(packdc,  LR + up[0] + look[1], TB + 10 + up[1] + look[3], (16 + up[2]) * look[0], (16 + up[3])*look[2], bkdc, ani_m, 0, 50, 40, SRCPAINT);
                for (int i = 0; i < t; i++) {
                    StretchBlt(packdc, tx[i] + up[0] + look[1], ty[i] + 10 + up[1], (16 + up[2]) * look[0], 16 + up[3], bkdc, ani_m, 0, 50, 40, SRCPAINT);
                }
            }
            //Ellipse(hdc, LR + up[0], 10 + TB + up[1], 16 + LR + up[2], 26 + TB + up[3]);
           
            if (ani == 0) {
                ani_m = 60;
                ani = 1;
            }
            else{
                ani_m = 0;
                ani = 0;
            }
            
            
            SelectObject(packdc, oldbit1);  DeleteDC(packdc);
            SelectObject(bkdc, oldbit2); DeleteDC(bkdc);
            ReleaseDC;
            InvalidateRect(hWnd, NULL, false);
            break;
        case 1:
            look[0] = 1;
            look[2] = 1;
            look[1] = 0;
            look[3] = 0;
            timer = 1;
            location = 1;
            for (int i = 0; i < 4; i++) {
                tx[i] = LR - 20 * (i + 1);
                ty[i] = TB;
            }
            if (LR == 624) {
                TB += 16;
                timer = 2;
                SetTimer(hWnd, timer, speed, NULL);
                KillTimer(hWnd, 1);
            }

            else {
                LR += 8;
                if (J >= 0 && J <= 5) {
                    Rx[J] = LR - 16;
                    Ry[J] = TB;
                }
            }

            for (int i = 0; i < 20; i++) {
                if (food[i].left >= LR+up[0] && food[i].top >= TB + 10+up[1] &&
                    food[i].right <= LR + 16 + up[2] && food[i].bottom <= TB + 26 + up[3]) {
                    food[i].left = 0;
                    food[i].right = 0;
                    food[i].top = 0;
                    food[i].bottom = 0;
                    up[0] -= 2;
                    up[1] -= 2;
                    up[2] += 2;
                    up[3] += 2;
                }
            }
            break;

        case 2:
            timer = 2;
            location = 2;
            look[0] = -1;
            look[2] = 1;
            look[3] = 0;
            look[1] = 16;
            for (int i = 0; i < 4; i++) {
                tx[i] = LR + 20 * (i + 1);
                ty[i] = TB;
            }
            if (LR == 0) {
                TB += 16;
                if (TB > 624) {
                    TB = 0;
                }
                timer = 1;
                SetTimer(hWnd, timer, speed, NULL);
                KillTimer(hWnd, 2);
            }
            else {
                LR -= 8;
            }
            for (int i = 0; i < 20; i++) {
                if (food[i].left >= LR + up[0] && food[i].top >= TB + 10 + up[1] &&
                    food[i].right <= LR + 16 + up[2] && food[i].bottom <= TB + 26 + up[3]) {
                    food[i].left = 0;
                    food[i].right = 0;
                    food[i].top = 0;
                    food[i].bottom = 0;
                    up[0] -= 2;
                    up[1] -= 2;
                    up[2] += 2;
                    up[3] += 2;
                }
            }
            break;

        case 3:
            timer = 3;
            location = 3;
            look[2] = -1;
            look[3] = 16;
            for (int i = 0; i < 4; i++) {
                tx[i] = LR;
                ty[i] = TB - 20 * (i + 1);
            }
            if (TB == 624) {
                LR += 16;
                if (LR > 624) {
                    LR = 0;
                }
                timer = 4;
                SetTimer(hWnd, timer, speed, NULL);
                KillTimer(hWnd, 3);
            }
            else {
                TB += 8;
            }
            for (int i = 0; i < 20; i++) {
                if (food[i].left >= LR + up[0] && food[i].top >= TB + 10 + up[1] &&
                    food[i].right <= LR + 16 + up[2] && food[i].bottom <= TB + 26 + up[3]) {
                    food[i].left = 0;
                    food[i].right = 0;
                    food[i].top = 0;
                    food[i].bottom = 0;
                    up[0] -= 2;
                    up[1] -= 2;
                    up[2] += 2;
                    up[3] += 2;
                }
            }
            break;

        case 4:
            timer = 4;
            location = 4;
            look[2] = 1;
            look[3] = 0;
            for (int i = 0; i < 4; i++) {
                tx[i] = LR;
                ty[i] = TB + 20 * (i + 1);
            }
            if (TB == 0) {
                LR += 16;
                if (LR > 624) {
                    LR = 0;
                }

                timer = 3;
                SetTimer(hWnd, timer, speed, NULL);
                KillTimer(hWnd, 4);
            }
            else {
                TB -= 8;             
            }
            for (int i = 0; i < 20; i++) {
                if (food[i].left >= LR + up[0] && food[i].top >= TB + 10 + up[1] &&
                    food[i].right <= LR + 16 + up[2] && food[i].bottom <= TB + 26 + up[3]) {
                    food[i].left = 0;
                    food[i].right = 0;
                    food[i].top = 0;
                    food[i].bottom = 0;
                    up[0] -= 2;
                    up[1] -= 2;
                    up[2] += 2;
                    up[3] += 2;
                }
            }
            break;
          
            //먹이 생성
            
        case 5:
            food[foodc].left = (rand() % 30 )* 16;
            food[foodc].right = food[foodc].left + 16;
            food[foodc].top = ((rand() % 30 )* 16 +10);
            food[foodc].bottom = (food[foodc].top + 16 );
            foodc++;
            if (foodc == 20) {
                KillTimer(hWnd, 5);
            }

            break;
        case 6:
            for (int i = 0; i < 20; i++) {
                if (i % 2 == 0) {
                    if (fmove[i] == 0) {
                        food[i].left += 8;
                        food[i].right += 8;
                        if (food[i].right == 624) {
                            fmove[i] = 1;
                        }
                    }
                    else if (fmove[i] == 1) {
                        food[i].left -= 8;
                        food[i].right -= 8;
                        if (food[i].left == 0) fmove[i] = 0;
                    }
                }
                else if (i % 2 == 1) {
                    if (fmove[i] == 0) {
                        food[i].top += 8;
                        food[i].bottom += 8;
                        if (food[i].bottom == 634) {
                            fmove[i] = 1;
                        }
                    }
                    else if (fmove[i] == 1) {
                        food[i].top -= 8;
                        food[i].bottom -= 8;
                        if (food[i].top == 10) {
                            fmove[i] = 0;
                        }
                    }
                }
            }
            break;
            //점프

        case 10:
            TB -= 16;
            Jump++;
            if (Jump > 5)
                TB += 32;
            for (int i = 0; i < 20; i++) {
                if (food[i].left >= LR + up[0] && food[i].top >= TB + 10 + up[1] &&
                    food[i].right <= LR + 16 + up[2] && food[i].bottom <= TB + 26 + up[3]) {
                    food[i].left = 0;
                    food[i].right = 0;
                    food[i].top = 0;
                    food[i].bottom = 0;
                    up[0] -= 2;
                    up[1] -= 2;
                    up[2] += 2;
                    up[3] += 2;
                }
            }
            if (Jump > 10) {
                Jump = 0;
                KillTimer(hWnd, 10);
            }

            break;

        case 11:
            LR += 16;
            Jump++;
            if (Jump > 5)
                LR -= 32;
            for (int i = 0; i < 20; i++) {
                if (food[i].left >= LR + up[0] && food[i].top >= TB + 10 + up[1] &&
                    food[i].right <= LR + 16 + up[2] && food[i].bottom <= TB + 26 + up[3]) {
                    food[i].left = 0;
                    food[i].right = 0;
                    food[i].top = 0;
                    food[i].bottom = 0;
                    up[0] -= 2;
                    up[1] -= 2;
                    up[2] += 2;
                    up[3] += 2;
                }
            }
            if (Jump > 10) {
                Jump = 0;
                KillTimer(hWnd, 11);
            }
            break;
        }

        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        mdc = CreateCompatibleDC(hdc);
        oldbit1 = (HBITMAP)SelectObject(mdc, hbit);
        BitBlt(hdc, 0, 0, 650, 650, mdc, 0, 0, SRCCOPY);
        SelectObject(mdc, oldbit1);
        SelectObject(mdc, oldbit2);
        DeleteDC(mdc);


        for (int i = 0; i < 20; i++) {
            hBrush = CreateSolidBrush(RGB(food_r[i], food_g[i], food_b[i]));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Ellipse(hdc, food[i].left, food[i].top, food[i].right, food[i].bottom);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);
        }

        EndPaint(hWnd, &ps);
        break;

    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        if (inCircle(mx, my, 8+ LR +up[1], 18 + TB+up[1])) {
            click = 10;

        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
        
    case WM_LBUTTONUP:
        Selection = FALSE;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_CHAR:
        hdc = GetDC(hWnd);
        //시작

        if (wParam == 'a' || wParam == 'A') {
            for (int i = 0; i < 20; ++i) {
                food[i].right = 0;
                food[i].left = 0;
                food[i].top = 0;
                food[i].bottom = 0;
            }
        }

        else if (wParam == 't' || wParam == 'T') {
            t++;
            if (t == 4) t = 0;
            
        }
        //종료
        else if (wParam == 'q' || wParam == 'Q') {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
        else if (wParam == 'j' || wParam == 'J') {
            if (timer == 1 || timer == 2)
                SetTimer(hWnd, 10, speed, NULL);
            if (timer == 3 || timer == 4)
                SetTimer(hWnd, 11, speed, NULL);
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYDOWN:
        hdc = GetDC(hWnd);
        if (wParam == VK_LEFT)
        {


            SetTimer(hWnd, 2, speed, NULL);
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 3);
            KillTimer(hWnd, 4);
        }
        if (wParam == VK_RIGHT)
        {
            SetTimer(hWnd, 1, speed, NULL);
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 3);
            KillTimer(hWnd, 4);
        }
        if (wParam == VK_UP)
        {

            SetTimer(hWnd, 4, speed, NULL);
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 3);
        }
        if (wParam == VK_DOWN)
        {

            SetTimer(hWnd, 3, speed, NULL);
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            KillTimer(hWnd, 4);
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
        KillTimer(hWnd, 3);
        KillTimer(hWnd, 4);
        KillTimer(hWnd, 5);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}
