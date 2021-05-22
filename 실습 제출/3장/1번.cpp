#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <cmath>
using namespace std;

default_random_engine dre;
uniform_int_distribution<> uid{ 0,25 };

#define Window_Size_X 1200
#define Window_Size_Y 1200
#define BLOCK_SIZE 20
#define LAST_BLOCK 39

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 2-9";
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


struct Chase {
    int x;
    int y;

    int xdir = 1;
    int ydir = 1;

    int count;

    int scale;
    int scale_count;

    bool contact;
    int contactNum = 10;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    HBRUSH hBrush, oldBrush;
    static HPEN hPen, oldPen;

    static RECT Board[40][40];
    static bool BoardState[40][40];

    static int my_Pos_x = 1;
    static int my_Pos_y = 0;

    static int change_Dir_x = 1;
    static int change_Dir_y = 1;

    static bool Left_Right = false;
    static bool Up_Down = false;
    static bool Down = false;
    static bool UP = false;
    static bool LEFT = false;
    static bool RIGHT = false;

    static int speed = 50;

    static int myPosDir = 0;

    static int CreateChaseTime;
    static int CreateChase;
    static Chase TailEl[5];
    static Chase chase[5] = {};
    static int Chase_count = 0;

    //마우스
    static int mx;
    static int my;
    static bool mL, mR, mT, mB;
    static int tempMx, tempMy;

    static bool ClickS;
    static int clickSpeed;
    switch (uMsg) {

    case WM_CREATE:

        srand(unsigned(time(NULL)));
        for (int i = 0; i < 5; ++i) {
            TailEl[i].x = 20;
            TailEl[i].y = 20;
        }
        TailEl[4].y = 28;

        break;

    case WM_TIMER:
        switch (wParam)
        {

            // 1 왼 2 오 3 위 4 아래
        case 1:
            CreateChaseTime++;
            if (CreateChaseTime % 20 == 0 && CreateChase != 5) {
                CreateChase++;
            }

            for (int i = 0; i < CreateChase; ++i) {

                if (!TailEl[i].contact) {
                    if (i == 0) {
                        if (TailEl[i].x == 0) TailEl[i].xdir = 1;
                        else if (TailEl[i].x == 39)  TailEl[i].xdir = -1;
                        TailEl[i].x += TailEl[i].xdir;
                    }
                    if (i == 1) {
                        if (TailEl[i].count % 2 == 0)
                            TailEl[i].x += TailEl[i].xdir;
                        else TailEl[i].y += TailEl[i].ydir;
                        if (TailEl[i].x == 0) {
                            TailEl[i].xdir = 1;
                            if (rand() % 2 == 0)TailEl[i].ydir = 1;
                            else TailEl[i].ydir = -1;
                            TailEl[i].count++;
                        }
                        if (TailEl[i].x == 39) {
                            TailEl[i].xdir = -1;
                            if (rand() % 2 == 0)TailEl[i].ydir = 1;
                            else TailEl[i].ydir = -1;
                            TailEl[i].count++;
                        }
                        if (TailEl[i].y == 0) {
                            TailEl[i].ydir = 1;
                            if (rand() % 2 == 0)TailEl[i].xdir = 1;
                            else TailEl[i].xdir = -1;
                            TailEl[i].count++;
                        }
                        if (TailEl[i].y == 39) {
                            TailEl[i].ydir = -1;
                            if (rand() % 2 == 0)TailEl[i].xdir = 1;
                            else TailEl[i].xdir = -1;
                            TailEl[i].count++;
                        }
                    }
                    if (i == 2) {
                        TailEl[i].x += TailEl[i].xdir;
                        TailEl[i].y += TailEl[i].ydir;
                        if (TailEl[i].x == 39 || TailEl[i].y == 39) {
                            TailEl[i].xdir = -1;
                            TailEl[i].ydir = -1;
                        }
                        else if (TailEl[i].y == 0 || TailEl[i].x == 0) {
                            TailEl[i].xdir = 1;
                            TailEl[i].ydir = 1;
                        }
                    }
                    if (i == 3) {
                        if (TailEl[i].y == 0) TailEl[i].ydir = 1;
                        else if (TailEl[i].y == 39)  TailEl[i].ydir = -1;
                        TailEl[i].y += TailEl[i].ydir;
                    }
                    if (i == 4) {
                        if (TailEl[i].x == 39) TailEl[i].x = 0;
                        TailEl[i].x += 1;
                    }

                    for (int j = 0; j < CreateChase; ++j) {
                        if (i != j) {
                            if (TailEl[i].x == TailEl[j].x && TailEl[i].y == TailEl[j].y) {
                                TailEl[i].scale_count = 4;
                                TailEl[j].scale_count = 4;
                            }
                        }
                    }
                    if (TailEl[i].scale_count != 0) {
                        TailEl[i].scale = 4;
                        TailEl[i].scale_count--;
                    }
                    else TailEl[i].scale = 0;

                    if (TailEl[i].x == my_Pos_x && TailEl[i].y == my_Pos_y) {
                        TailEl[i].contact = true;
                    }
                }



                // 꼬리원 연결 bool
                for (int i = 0; i < 5; ++i) {
                    if (TailEl[i].contact && TailEl[i].contactNum == 10) {
                        for (int j = 0; j < 5; ++j) {
                            if (chase[j].contact == false) {
                                chase[j].contact = true;
                                TailEl[i].contactNum = j;
                                TailEl[i].scale = 0;
                                TailEl[i].scale_count = 0;
                                break;
                            }
                        }
                    }

                    else if (TailEl[i].contact) {
                        TailEl[i].x = chase[TailEl[i].contactNum].x;
                        TailEl[i].y = chase[TailEl[i].contactNum].y;

                    }
                }

            }


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


            //for (int i = 0; i < 40; ++i) {
            //    for (int j = 0; j < 40; ++j) {
            //        if (BoardState[i][j]) {
            //            // 꼬리 충돌
            //            for (int c = 0; i < 5; ++c) {
            //                Board[]
            //            }
            //            // 주인공 충돌
            //        }
            //    }
            //}

            if (Chase_count == 5) {

                chase[4].x = chase[3].x;
                chase[4].y = chase[3].y;

                chase[3].x = chase[2].x;
                chase[3].y = chase[2].y;

                chase[2].x = chase[1].x;
                chase[2].y = chase[1].y;

                chase[1].x = chase[0].x;
                chase[1].y = chase[0].y;

                chase[0].x = my_Pos_x;
                chase[0].y = my_Pos_y;
            }
            else {
                chase[Chase_count].x = my_Pos_x;
                chase[Chase_count].y = my_Pos_y;
                Chase_count++;
            }


            break;
        case 2:
            clickSpeed++;
            if (clickSpeed == 15) {
                clickSpeed = 0;
                KillTimer(hWnd, 2);

            }
            // 꼬리원 연결 bool
            for (int i = 0; i < 5; ++i) {
                if (TailEl[i].contact && TailEl[i].contactNum == 10) {
                    for (int j = 0; j < 5; ++j) {
                        if (chase[j].contact == false) {
                            chase[j].contact = true;
                            TailEl[i].contactNum = j;
                            TailEl[i].scale = 0;
                            TailEl[i].scale_count = 0;
                            break;
                        }
                    }
                }

                else if (TailEl[i].contact) {
                    TailEl[i].x = chase[TailEl[i].contactNum].x;
                    TailEl[i].y = chase[TailEl[i].contactNum].y;

                }
            }

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
            if (Chase_count == 5) {

                chase[4].x = chase[3].x;
                chase[4].y = chase[3].y;

                chase[3].x = chase[2].x;
                chase[3].y = chase[2].y;

                chase[2].x = chase[1].x;
                chase[2].y = chase[1].y;

                chase[1].x = chase[0].x;
                chase[1].y = chase[0].y;

                chase[0].x = my_Pos_x;
                chase[0].y = my_Pos_y;
            }
            else {
                chase[Chase_count].x = my_Pos_x;
                chase[Chase_count].y = my_Pos_y;
                Chase_count++;
            }
            break;
        }

        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_RBUTTONDOWN:

        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 40; ++j) {
                if (!BoardState[i][j] && (mx > Board[i][j].left && mx < Board[i][j].right
                    && my > Board[i][j].top && my < Board[i][j].bottom))
                {
                    BoardState[i][j] = true;
                }
            }
        }
        break;
    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        /*
        if (mx > my_Pos_x)
        {
            tempMx = abs(mx - my_Pos_x);
            mR = true;
        }
        if (mx < my_Pos_x)
        {
            tempMx = abs(my_Pos_x - mx);
            mL = true;
        }
        if (my > my_Pos_y)
        {
            tempMy = abs(my - my_Pos_y);
            mB = true;
        }
        if (my < my_Pos_y)
        {
            tempMy = abs(my_Pos_y - my);
            mT = true;
        }
        if (tempMx > tempMy) {
                mT = false;
                mB = false;

        }
        else if (tempMx < tempMy) {

                mL = false;
                mR = false;

        }

        if (mL) {
            Left_Right = true;
            Up_Down = false;
            LEFT = true;
            if (change_Dir_x == 1)
                change_Dir_x *= -1;
            else
                change_Dir_x *= 1;
        }
        if (mR) {
            Left_Right = true;
            Up_Down = false;
            RIGHT = true;
            if (change_Dir_x == -1)
                change_Dir_x *= -1;
            else
                change_Dir_x *= 1;
        }
        if (mT) {
            Up_Down = true;
            Left_Right = false;
            UP = true;
            if (change_Dir_y == 1)
                change_Dir_y = -1;

        }
        if (mB) {
            Up_Down = true;
            Left_Right = false;
            Down = true;
            if (change_Dir_y == -1)
                change_Dir_y = 1;
        }
        mL = false; mR = false; mT = false; mB = false;
        */
        // 원클릭 스피드
        if (mx > my_Pos_x * 20 && mx < (my_Pos_x + 1) * 20
            && my > my_Pos_y * 20 && my < (my_Pos_y + 1) * 20) {
            SetTimer(hWnd, 2, 25, NULL);
        }
        // 꼬리원 자르기
        for (int i = 0; i < 5; ++i) {
            if (chase[i].contact && (mx > chase[i].x * 20 && mx < (chase[i].x + 1) * 20
                && my >chase[i].y * 20 && my < (chase[i].y + 1) * 20)) {
                for (int j = i; j < 5; ++j) {

                    for (int c = 0; c < 5; ++c) {
                        if (TailEl[c].contactNum == j) {
                            TailEl[c].contact = false;
                            TailEl[c].contactNum = 10;
                        }
                    }
                    chase[j].contact = false;

                }
            }
        }
        // 장애물 생성

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

    case WM_CHAR:

        switch (wParam)
        {
        case 's':
        case 'S':

            Left_Right = true;
            Up_Down = false;

            SetTimer(hWnd, 1, speed, NULL);
            break;

        case '+':
            speed -= 30;
            break;
        case '-':
            speed += 30;
            break;
        case 'j':
        case 'J':
            if (Up_Down) {

            }
            else if (Left_Right) {

            }
            break;

        case't':
        case'T':

            break;

        case'q':
        case'Q':

            break;
        }
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

                if (BoardState[i][j])
                {
                    hBrush = CreateSolidBrush(RGB(0, 0, 0));
                    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                    Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);

                    SelectObject(hdc, oldBrush);
                    DeleteObject(hBrush);
                }
                else {
                    Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);
                }

            }
        }

        hBrush = CreateSolidBrush(RGB(0, 255, 0));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Ellipse(hdc, my_Pos_x * 20, my_Pos_y * 20,
            (my_Pos_x + 1) * 20, (my_Pos_y + 1) * 20);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);

        for (int i = 0; i < CreateChase; ++i) {

            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

            Ellipse(hdc, (TailEl[i].x * 20) - TailEl[i].scale, (TailEl[i].y * 20) - TailEl[i].scale, ((TailEl[i].x + 1) * 20) + TailEl[i].scale, ((TailEl[i].y + 1) * 20) + TailEl[i].scale);

            SelectObject(hdc, oldBrush);
            DeleteObject(hBrush);

        }


        TCHAR str[100];
        wsprintf(str, TEXT("x : %d, y : %d ,  count : %d"), my_Pos_x, my_Pos_y, Chase_count);
        TextOut(hdc, 1000, 200, str, lstrlen(str));

        TCHAR str1[1000];
        for (int i = 0; i < 5; ++i) {
            wsprintf(str1, TEXT("x : %d, y : %d"), chase[i].contact, chase[i].y);
            TextOut(hdc, 1000, 400 + i * 30, str1, lstrlen(str1));
        }
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}