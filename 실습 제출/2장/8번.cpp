#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>

#define Window_Size_X 1000
#define Window_Size_Y 1000
#define SIZE 50

#define RED_BLOCK       1
#define CHANGE_GREEN    2
#define CHANGE_BROWN    3
#define CHANGE_SCALE    4
#define DOWN            5
#define UP              6
#define RIGHT           7
#define LEFT            8

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 2-8";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

struct Player{
    POINT pos;
    int r;
    int g;
    int b;
    int scale;
    bool isScale;
};

void Check_Block(int Check[][15], Player* player, int dir);

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

    static RECT Board[15][15];
    static int Check[15][15];
    static Player player[2];
    static bool p1_Used = true;
    static bool p2_Used = false;

    switch (uMsg) {

    case WM_CREATE:
        player[0] = { 0,0,255,255,0,0,false };
        player[1] = { 14,14,0,255,255,0,false };
        break;

    case WM_CHAR:

        switch (wParam)
        {
        case 's':
        case 'S':            
            if (p1_Used)
            {
                player[0].pos.y += 1;
                Check_Block(Check, &player[0], DOWN);
                p2_Used = true;
                p1_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;

        case 'w':
        case 'W':
            if (p1_Used) 
            {
                player[0].pos.y -= 1;
                Check_Block(Check, &player[0], UP);
                p2_Used = true;
                p1_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;

        case 'd':
        case 'D':
            if (p1_Used)
            {
                player[0].pos.x += 1;
                Check_Block(Check, &player[0], RIGHT);
                p2_Used = true;
                p1_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;

        case 'a':
        case 'A':
            if (p1_Used)
            {
                player[0].pos.x -= 1;
                Check_Block(Check, &player[0], LEFT);
                p2_Used = true;
                p1_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;

        case 'r':
        case 'R':
            player[0] = { 0,0,255,255,0,0,false };
            player[1] = { 14,14,0,255,255,0,false };
            break;

        case 'q':
        case 'Q':
            exit(1);

        }
        

        InvalidateRect(hWnd, NULL, TRUE);        
        break;
                
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_DOWN:
            if (p2_Used) 
            {
                player[1].pos.y += 1;
                Check_Block(Check, &player[1], DOWN);
                p1_Used = true;
                p2_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;

        case VK_UP:
            if (p2_Used)
            {
                player[1].pos.y -= 1;
                Check_Block(Check, &player[1], UP);
                p1_Used = true;
                p2_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;

        case VK_RIGHT:
            if (p2_Used)
            {
                player[1].pos.x += 1;
                Check_Block(Check, &player[1], RIGHT);
                p1_Used = true;
                p2_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;

        case VK_LEFT:
            if (p2_Used)
            {
                player[1].pos.x -= 1;
                Check_Block(Check, &player[1], LEFT);
                p1_Used = true;
                p2_Used = false;
            }
            //else
                //MessageBox(hWnd, L"Error !! ", L"에러", MB_ICONWARNING);
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Draw 15x15 Board       
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                Board[i][j].left = 0 + i * 50;
                Board[i][j].right = 50 + i * 50;
                Board[i][j].top = 0 + j * 50;
                Board[i][j].bottom = 50 + j * 50;

                if (j % 5 == 2 && i % 10 == 3)
                {
                    Check[i][j] = RED_BLOCK;

                    hBrush = CreateSolidBrush(RGB(255, 0, 0));
                    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                    Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);

                    SelectObject(hdc, oldBrush);
                    DeleteObject(hBrush);
                }

                else if (j % 7== 1 && i % 6 == 1)
                {
                    Check[i][j] = CHANGE_GREEN;

                    hBrush = CreateSolidBrush(RGB(0, 255, 0));
                    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                    Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);

                    SelectObject(hdc, oldBrush);
                    DeleteObject(hBrush);
                }                                

                else if (j % 6 == 1 && i % 8 == 2)
                {
                    Check[i][j] = CHANGE_SCALE;

                    hBrush = CreateSolidBrush(RGB(0, 0, 255));
                    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                    Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);

                    SelectObject(hdc, oldBrush);
                    DeleteObject(hBrush);
                }

                else if (j % 4 == 3 && i % 8 == 1) 
                {
                    Check[i][j] = CHANGE_BROWN;

                    hBrush = CreateSolidBrush(RGB(200, 100, 50));
                    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                    Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);

                    SelectObject(hdc, oldBrush);
                    DeleteObject(hBrush);                
                }

                else
                {
                    Check[i][j] = 5;

                    Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);
                }
            }
        }

        // Player 1
        hBrush = CreateSolidBrush(RGB(player[0].r, player[0].g, player[0].b));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        Ellipse(hdc, player[0].pos.x * SIZE - player[0].scale, player[0].pos.y * SIZE - player[0].scale,
            (player[0].pos.x + 1) * SIZE + player[0].scale, (player[0].pos.y + 1) * SIZE + player[0].scale);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);

        TCHAR str1[10];
        wsprintf(str1, TEXT("P_1"));
        TextOut(hdc, player[0].pos.x * SIZE, player[0].pos.y * SIZE + 10, str1, lstrlen(str1));

        // Player 2
        hBrush = CreateSolidBrush(RGB(player[1].r, player[1].g, player[1].b));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        Ellipse(hdc, player[1].pos.x* SIZE - player[1].scale, player[1].pos.y* SIZE - player[1].scale,
            (player[1].pos.x + 1)* SIZE + player[1].scale, (player[1].pos.y + 1)* SIZE + player[1].scale);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);

        TCHAR str2[10];
        wsprintf(str2, TEXT("P_2"));
        TextOut(hdc, player[1].pos.x* SIZE, player[1].pos.y* SIZE + 10, str2, lstrlen(str2));


        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Check_Block(int Check[][15], Player *player, int dir)
{
    // 1. 색깔 바꾸기
    if (Check[player->pos.x][player->pos.y] == CHANGE_GREEN)
    {
        player->r = 0;
        player->g = 255;
        player->b = 0;
    }

    if (Check[player->pos.x][player->pos.y] == CHANGE_BROWN)
    {
        player->r = 200;
        player->g = 100;
        player->b = 50;
    }

    // 2. 축소 및 확대
    if (Check[player->pos.x][player->pos.y] == CHANGE_SCALE)
    {
        player->isScale = !(player->isScale);

        if(player->isScale)
            player->scale += 10;
        else 
            player->scale -= 10;        
    }


    // 3. 벽돌 못 지나가게 하기 
    // 4. 가장자리 도착하면 반대쪽으로 나타나게 하기
    switch (dir)
    {
    case DOWN:
        if (Check[player->pos.x][player->pos.y] == RED_BLOCK)
            player->pos.y -= 1;
        if (player->pos.y > 14)
            player->pos.y = 0;        
    case UP:
        if (Check[player->pos.x][player->pos.y] == RED_BLOCK)
            player->pos.y += 1;
        if (player->pos.y < 0)
            player->pos.y = 14;
    case RIGHT:
        if (Check[player->pos.x][player->pos.y] == RED_BLOCK)
            player->pos.x -= 1;
        if (player->pos.x > 14)
            player->pos.x = 0;
    case LEFT:
        if (Check[player->pos.x][player->pos.y] == RED_BLOCK)
            player->pos.x += 1;
        if (player->pos.x < 0)
            player->pos.x = 14;
    }
}

