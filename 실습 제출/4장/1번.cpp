#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include "resource.h"

#define Window_Size_X 1000
#define Window_Size_Y 1000
#define BLOCK_SIZE 20
#define LAST_BLOCK 39
#define CIRCLE 1
#define RECTANGLE 2

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 4-1";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

struct Chase {
    int x;
    int y;

    int Dir_x = 1;
    int Dir_y = 1;

    int count;

    int scale;
    int scale_count;
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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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

    static RECT Board[40][40];

    static int my_Pos_x = 1;
    static int my_Pos_y = 0;

    static int r, g, b;
    static int shape;

    // 플레이어 Moving, Speed, Direction
    static int change_Dir_x = 1;
    static int change_Dir_y = 1;
    static bool Left_Right = false;
    static bool Up_Down = false;
    static bool Down = false;
    static bool UP = false;
    static bool LEFT = false;
    static bool RIGHT = false;
    static int speed;

    // 꼬리원 관련
    static int CreateChase;
    static Chase TailEl[30];
    static bool startChase;

    switch (uMsg) {

    case WM_CREATE:
        srand((unsigned)time(NULL));  
        r = 0;
        g = 255;
        b = 255;
        shape = CIRCLE;
        speed = 100;

        CreateChase = 20;
        startChase = false;
        for (int i = 0; i < 30; ++i) {
            TailEl[i].x = rand() % LAST_BLOCK;
            TailEl[i].y = rand() % LAST_BLOCK;
        }
        
        // TailEl[].y = 28;

        break;

    case WM_TIMER:
        switch (wParam)
        {
        case 1:

            if (Left_Right)
            {
                if (my_Pos_x == LAST_BLOCK || my_Pos_x == 0) {
                    if (my_Pos_y == 0) {
                        my_Pos_y += 1;
                        change_Dir_y = 1;
                        change_Dir_x *= -1;
                    }
                    else if (my_Pos_y == LAST_BLOCK) {
                        my_Pos_y -= 1;
                        change_Dir_y -= 1;
                        change_Dir_x *= -1;
                    }
                    else if (my_Pos_x == 0 && RIGHT) {
                        my_Pos_y += change_Dir_y;
                        change_Dir_x = 1;
                        RIGHT = false;
                    }
                    else if (my_Pos_x == LAST_BLOCK && UP) {
                        my_Pos_y += change_Dir_y;
                        change_Dir_x = -1;
                        UP = false;
                    }
                    else {
                        my_Pos_y += change_Dir_y;
                        change_Dir_x *= -1;
                    }
                }
                my_Pos_x += change_Dir_x;

            }

            if (Up_Down)
            {
                if (my_Pos_y == LAST_BLOCK || my_Pos_y == 0) {
                    if (my_Pos_x == 0) {
                        my_Pos_x = 1;
                        change_Dir_x = 1;
                        change_Dir_y *= -1;
                    }
                    else if (my_Pos_x == LAST_BLOCK) {
                        my_Pos_x -= 1;
                        change_Dir_x = -1;
                        change_Dir_y *= -1;
                    }
                    else if (my_Pos_y == 0 && Down) {
                        my_Pos_x += change_Dir_x;
                        change_Dir_y = 1;
                        Down = false;
                    }
                    else if (my_Pos_y == LAST_BLOCK && UP) {
                        my_Pos_x += change_Dir_x;
                        change_Dir_y = -1;
                        UP = false;
                    }
                    else {
                        my_Pos_x += change_Dir_x;
                        change_Dir_y *= -1;
                    }
                }
                my_Pos_y += change_Dir_y;
            }
            break;

        case 2:

            for (int i = 0; i < CreateChase; ++i) {

                if (i % 5 == 0) {
                    if (TailEl[i].x == 0) 
                        TailEl[i].Dir_x = 1;
                    else if (TailEl[i].x == LAST_BLOCK)  
                        TailEl[i].Dir_x = -1;
                    TailEl[i].x += TailEl[i].Dir_x;
                }

                if (i % 5 == 1) {
                    if (TailEl[i].count % 2 == 0)
                        TailEl[i].x += TailEl[i].Dir_x;
                    else TailEl[i].y += TailEl[i].Dir_y;
                    if (TailEl[i].x == 0) {
                        TailEl[i].Dir_x = 1;
                        if (rand() % 2 == 0)TailEl[i].Dir_y = 1;
                        else TailEl[i].Dir_y = -1;
                        TailEl[i].count++;
                    }
                    if (TailEl[i].x == LAST_BLOCK) {
                        TailEl[i].Dir_x = -1;
                        if (rand() % 2 == 0)TailEl[i].Dir_y = 1;
                        else TailEl[i].Dir_y = -1;
                        TailEl[i].count++;
                    }
                    if (TailEl[i].y == 0) {
                        TailEl[i].Dir_y = 1;
                        if (rand() % 2 == 0)TailEl[i].Dir_x = 1;
                        else TailEl[i].Dir_x = -1;
                        TailEl[i].count++;
                    }
                    if (TailEl[i].y == LAST_BLOCK) {
                        TailEl[i].Dir_y = -1;
                        if (rand() % 2 == 0)TailEl[i].Dir_x = 1;
                        else TailEl[i].Dir_x = -1;
                        TailEl[i].count++;
                    }
                }
                if (i % 5 == 2) {
                    TailEl[i].x += TailEl[i].Dir_x;
                    TailEl[i].y += TailEl[i].Dir_y;
                    if (TailEl[i].x == LAST_BLOCK || TailEl[i].y == LAST_BLOCK) {
                        TailEl[i].Dir_x = -1;
                        TailEl[i].Dir_y = -1;
                    }
                    else if (TailEl[i].y == 0 || TailEl[i].x == 0) {
                        TailEl[i].Dir_x = 1;
                        TailEl[i].Dir_y = 1;
                    }
                }
                if (i % 5 == 3) {
                    if (TailEl[i].y == 0) TailEl[i].Dir_y = 1;
                    else if (TailEl[i].y == LAST_BLOCK)  TailEl[i].Dir_y = -1;
                    TailEl[i].y += TailEl[i].Dir_y;
                }
                if (i % 5 == 4) {
                    if (TailEl[i].x == LAST_BLOCK) TailEl[i].x = 0;
                    TailEl[i].x += 1;
                }

                //for (int j = 0; j < CreateChase; ++j) {
                //    if (i != j) {
                //        if (TailEl[i].x == TailEl[j].x && TailEl[i].y == TailEl[j].y) {
                //            TailEl[i].scale_count = 4;
                //            TailEl[j].scale_count = 4;
                //        }
                //    }
                //}

                //if (TailEl[i].scale_count != 0) {
                //    TailEl[i].scale = 4;
                //    TailEl[i].scale_count--;
                //}

                //else 
                //    TailEl[i].scale = 0;
            }
        }

        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_RIGHT:
            Left_Right = true;
            Up_Down = false;
            RIGHT = true;
            if (change_Dir_x == -1)
                change_Dir_x *= -1;
            else
                change_Dir_x *= 1;
            break;
        case VK_LEFT:
            Left_Right = true;
            Up_Down = false;
            LEFT = true;
            if (change_Dir_x == 1)
                change_Dir_x *= -1;
            else
                change_Dir_x *= 1;
            break;
        case VK_UP:
            Up_Down = true;
            Left_Right = false;
            UP = true;
            if (change_Dir_y == 1)
                change_Dir_y = -1;

            break;
        case VK_DOWN:
            Up_Down = true;
            Left_Right = false;
            Down = true;
            if (change_Dir_y == -1)
                change_Dir_y = 1;

            break;
        }

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

        case ID_GAME_START:
            Left_Right = true;
            Up_Down = false;
            SetTimer(hWnd, 1, speed, NULL);
            break;
        case ID_GAME_RESET:
            if (MessageBox(hWnd, L"Do you really want to reset Game?", NULL, MB_YESNO) == IDYES) {
                KillTimer(hWnd, 1);
                KillTimer(hWnd, 2);

                my_Pos_x = 1;
                my_Pos_y = 0;
                Left_Right = false;
                Up_Down = false;

                startChase = false;
            }
            break;
        case ID_GAME_END:
            if (MessageBox(hWnd, L"Do you really want to end Game?", NULL, MB_YESNO) == IDYES)
                exit(1);
            break;
        case ID_SPEED_FAST:
            KillTimer(hWnd, 1);
            speed -= 10;
            SetTimer(hWnd, 1, speed, NULL);
            break;
        case ID_SPEED_MEDIUM:
            KillTimer(hWnd, 1);
            speed = 100;
            SetTimer(hWnd, 1, speed, NULL);
            break;
        case ID_SPEED_SLOW:
            KillTimer(hWnd, 1);
            speed += 10;
            SetTimer(hWnd, 1, speed, NULL);
            break;
        case ID_COLOR_CYAN:
            r = 0;
            g = 255;
            b = 255;
            break;
        case ID_COLOR_MAGENTA:
            r = 255;
            g = 0;
            b = 255;
            break;
        case ID_COLOR_YELLOW:
            r = 255;
            g = 255;
            b = 0;
            break;
        case ID_SHAPE_CIRCLE:
            shape = CIRCLE;
            break;
        case ID_SHAPE_RECTANGLE:
            shape = RECTANGLE;
            break;
        case ID_TAIL_20:
            startChase = true;
            KillTimer(hWnd, 2);
            CreateChase = 20;
            SetTimer(hWnd, 2, 50, NULL);
            break;
        case ID_TAIL_25:
            startChase = true;
            KillTimer(hWnd, 2);
            CreateChase = 25;
            SetTimer(hWnd, 2, 50, NULL);
            break;
        case ID_TAIL_30:
            startChase = true;
            KillTimer(hWnd, 2);
            CreateChase = 30;
            SetTimer(hWnd, 2, 50, NULL);
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Draw Board       
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                Board[i][j].left = 0 + i * BLOCK_SIZE;
                Board[i][j].right = BLOCK_SIZE + i * BLOCK_SIZE;
                Board[i][j].top = 0 + j * BLOCK_SIZE;
                Board[i][j].bottom = BLOCK_SIZE + j * BLOCK_SIZE;

                Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);
            }
        }

        hBrush = CreateSolidBrush(RGB(r, g, b));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        if (shape == CIRCLE) 
        {
            Ellipse(hdc, my_Pos_x * BLOCK_SIZE, my_Pos_y * BLOCK_SIZE,
                (my_Pos_x + 1) * BLOCK_SIZE, (my_Pos_y + 1) * BLOCK_SIZE);
        }
        else if (shape == RECTANGLE) 
        {
            Rectangle(hdc, my_Pos_x * BLOCK_SIZE, my_Pos_y * BLOCK_SIZE,
                (my_Pos_x + 1) * BLOCK_SIZE, (my_Pos_y + 1) * BLOCK_SIZE);
        }

        if (startChase) {
            for (int i = 0; i < CreateChase; ++i) {

                hBrush = CreateSolidBrush(RGB(255, 0, 0));
                oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                Ellipse(hdc, (TailEl[i].x * BLOCK_SIZE) - TailEl[i].scale, (TailEl[i].y * BLOCK_SIZE) - TailEl[i].scale,
                    ((TailEl[i].x + 1) * BLOCK_SIZE) + TailEl[i].scale, ((TailEl[i].y + 1) * BLOCK_SIZE) + TailEl[i].scale);

                SelectObject(hdc, oldBrush);
                DeleteObject(hBrush);
            }
        }



        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);

        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
