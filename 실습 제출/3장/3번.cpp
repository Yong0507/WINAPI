#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>

#define Window_Size_X 1000
#define Window_Size_Y 1000
#define SIZE 50

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 3-3";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

struct Circle {
    POINT pos;
    int color[3];
    bool isStop;
};

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
    HBRUSH hBrush, oldBrush;

    // 원
    static Circle circles[100];
    static int rand_speed;
    static int speed;

    // 고무줄 사각형 왼쪽 마우스 버튼
    static int LM_startX, LM_startY, LM_oldX, LM_oldY;
    int LM_endX, LM_endY;
    static BOOL left_Drag;

    // 고무줄 사각형 오른쪽 마우스 버튼
    static int RM_startX, RM_startY, RM_oldX, RM_oldY;
    int RM_endX, RM_endY;
    static BOOL right_Drag;
    static int distX, distY;
    static int temp_distX, temp_distY;

    // 원을 담을 바구니 
    static RECT bucket;

    // 타이머 속도
    static int timer_speed;

    static int tempX, tempY;

    switch (uMsg) {

    case WM_CREATE:
        srand((unsigned)time(NULL));

        for (int i = 0; i < 100; ++i) {
            circles[i].pos.x = rand() % 900;
            circles[i].pos.y = 20;
            circles[i].color[0] = rand() % 256;
            circles[i].color[1] = rand() % 256;
            circles[i].color[2] = rand() % 256;
            circles[i].isStop = false;
        }

        timer_speed = 30;

        break;

    case WM_TIMER:
        switch (wParam) {

        case 1:
            for (int i = 0; i < 100; ++i) {
                if ((circles[i].pos.x > bucket.left && circles[i].pos.x < bucket.right) &&
                    circles[i].pos.y < bucket.bottom && circles[i].pos.y > bucket.bottom - 10) {
                    if (right_Drag) {
                        circles[i].isStop = false;
                    }
                    else {
                        circles[i].isStop = true;
                    }
                }
                if (!circles[i].isStop)
                {
                    rand_speed = rand() % 5;

                    if (rand_speed == 1)
                        speed = 1;
                    else if (rand_speed == 2)
                        speed = 2;
                    else if (rand_speed == 3)
                        speed = 3;
                    else if (rand_speed == 4)
                        speed = 4;
                    else if (rand_speed == 5)
                        speed = 5;

                    circles[i].pos.y += speed;
                    if (circles[i].pos.y > 950)
                        circles[i].pos.y = 0;
                }
            }



            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;


    case WM_CHAR:
        switch (wParam) {
        case 's':
        case 'S':
            SetTimer(hWnd, 1, timer_speed, NULL);
            break;

        case 'd':
        case 'D':
            LM_startX = 0;
            LM_startY = 0;
            LM_oldX = 0;
            LM_oldY = 0;
            for (int i = 0; i < 100; ++i) {
                circles[i].isStop = false;
            }
            // 바구니 초기화
            bucket.left = 0;
            bucket.right = 0;
            bucket.top = 0;
            bucket.bottom = 0;

            break;

        case '+':
            KillTimer(hWnd, 1);
            timer_speed -= 5;
            SetTimer(hWnd, 1, timer_speed, NULL);
            break;
        case '-':
            KillTimer(hWnd, 1);
            timer_speed += 5;
            SetTimer(hWnd, 1, timer_speed, NULL);
            break;
        case 'q':
        case 'Q':
            exit(1);
        }
        break;
    case WM_LBUTTONDOWN:
        hdc = GetDC(hWnd);

        //distX = 0;
        //distY = 0;

        for (int i = 0; i < 100; ++i) {
            circles[i].isStop = false;
        }

        // 바구니 초기화
        bucket.left = 0;
        bucket.right = 0;
        bucket.top = 0;
        bucket.bottom = 0;

        LM_oldX = LM_startX = LOWORD(lParam);
        LM_oldY = LM_startY = HIWORD(lParam);

        Rectangle(hdc, LM_startX, LM_startY, LM_oldX, LM_oldY);

        ReleaseDC(hWnd, hdc);
        left_Drag = TRUE;

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_MOUSEMOVE:
        hdc = GetDC(hWnd);

        if (left_Drag)
        {
            SetROP2(hdc, R2_NOT);
            SelectObject(hdc, (HPEN)GetStockObject(NULL_BRUSH));

            LM_endX = LOWORD(lParam);
            LM_endY = HIWORD(lParam);

            Rectangle(hdc, LM_startX, LM_startY, LM_oldX, LM_oldY);
            Rectangle(hdc, LM_startX, LM_startY, LM_endX, LM_endY);

            LM_oldX = LM_endX;
            LM_oldY = LM_endY;
        }

        if (right_Drag)
        {
            SetROP2(hdc, R2_NOT);
            SelectObject(hdc, (HPEN)GetStockObject(NULL_BRUSH));

            RM_endX = LOWORD(lParam);
            RM_endY = HIWORD(lParam);

            distX = RM_endX - RM_startX;
            distY = RM_endY - RM_startY;

            tempX = distX;
            tempY = distY;

            TCHAR str[30];
            wsprintf(str, TEXT("x : %d, y : %d "), bucket.left, bucket.top);
            TextOut(hdc, 100, 100, str, lstrlen(str));

            RM_oldX = RM_endX;
            RM_oldY = RM_endY;
        }

        ReleaseDC(hWnd, hdc);

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_LBUTTONUP:

        left_Drag = FALSE;
        if (LM_startX > LM_oldX) {
            bucket.left = LM_oldX;
            bucket.right = LM_startX;
        }
        else if (LM_startX < LM_oldX) {
            bucket.left = LM_startX;
            bucket.right = LM_oldX;
        }

        if (LM_startY > LM_oldY) {
            bucket.top = LM_oldY;
            bucket.bottom = LM_startY;
        }
        else if (LM_startY < LM_oldY) {
            bucket.top = LM_startY;
            bucket.bottom = LM_oldY;
        }


        break;
    case WM_RBUTTONDOWN:
        hdc = GetDC(hWnd);



        distX = 0;
        distY = 0;
        tempX = 0;
        tempY = 0;
        RM_oldX = RM_startX = LOWORD(lParam);
        RM_oldY = RM_startY = HIWORD(lParam);

        ReleaseDC(hWnd, hdc);

        // 오른쪽 마우스 버튼을 클릭했을 때 그려진 네모 안에 있는지 확인
        if (RM_startX > bucket.left && RM_startX < bucket.right && RM_startY > bucket.top && RM_startY < bucket.bottom) {

            right_Drag = TRUE;
        }



        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_RBUTTONUP:

        for (int i = 0; i < 100; i++) {
            circles[i].isStop = false;
        }
        right_Drag = FALSE;
        tempX = 0;
        tempY = 0;
        bucket.left += distX;
        bucket.right += distX;
        bucket.top += distY;
        bucket.bottom += distY;
        InvalidateRect(hWnd, NULL, TRUE);

        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        Rectangle(hdc, bucket.left + tempX, bucket.top + tempY, bucket.right + tempX, bucket.bottom + tempY);

        for (int i = 0; i < 100; ++i) {

            hBrush = CreateSolidBrush(RGB(circles[i].color[0], circles[i].color[1], circles[i].color[2]));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Ellipse(hdc, circles[i].pos.x, circles[i].pos.y, circles[i].pos.x + 10, circles[i].pos.y + 10);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);
        }

        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}