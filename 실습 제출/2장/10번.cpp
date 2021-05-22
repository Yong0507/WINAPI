#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
using namespace std;

default_random_engine dre;
uniform_int_distribution<> uid{ 0,25 };

#define Window_Size_X 1200
#define Window_Size_Y 1200
#define BLOCK_SIZE 20

#define CIRCLE      0
#define TRIANGLE    1 
#define RECTANGLE   2

#define CHANGE_SCALE    4
#define DOWN            5
#define UP              6
#define RIGHT           7
#define LEFT            8

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 2-9";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

struct Shape {
    int pos_x, pos_y;
    int r, g, b;
    int input_Shape;
    int input_Move;
    bool check;
};

struct Color {
    int r, g, b;
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
    static HPEN hPen, oldPen;

    static RECT Board[50][50];
    static POINT tri_Point[3];
    static int board_Num = 30;
    static int input_Key;

    static int rand_rgb[3][3] = { 0, };
    static POINT tri_Pos[3] = { 0, };

    static POINT rand_Circle_Pos;
    static POINT rand_Rectangle_Pos;
    static POINT rand_Triangle_Pos;

    static Shape Shapes[20];
    static int count = 0;
    static int temp_count = 0;

    static int number;

    static int change_Color = 0;
    static Color temp_Color[20] = { 0, };

    switch (uMsg) {

    case WM_CREATE:
        srand((unsigned)time(NULL));
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
            Shapes[temp_count].pos_y -= 1;
            if (Shapes[temp_count].pos_y < 0)
                Shapes[temp_count].pos_y = board_Num - 1;

            break;

        case VK_DOWN:
            Shapes[temp_count].pos_y += 1;
            if (Shapes[temp_count].pos_y > board_Num - 1)
                Shapes[temp_count].pos_y = 0;
            break;

        case VK_RIGHT:
            Shapes[temp_count].pos_x += 1;
            if (Shapes[temp_count].pos_x > board_Num - 1)
                Shapes[temp_count].pos_x = 0;
            break;

        case VK_LEFT:
            Shapes[temp_count].pos_x -= 1;
            if (Shapes[temp_count].pos_x < 0)
                Shapes[temp_count].pos_x = board_Num - 1;
            break;

        case VK_DELETE:
            for (int i = temp_count; i < count; ++i) {
                Shapes[i] = Shapes[i + 1];
            }
            count--;
            break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_CHAR:

        switch (wParam)
        {
        case 's':
        case 'S':
            board_Num = 30;
            for (int i = 0; i < count; ++i) {
                if (Shapes[i].pos_x > 29)Shapes[i].pos_x = 29;
                if (Shapes[i].pos_y > 29)Shapes[i].pos_y = 29;
            }
            break;

        case 'm':
        case 'M':
            board_Num = 40;
            for (int i = 0; i < count; ++i) {
                if (Shapes[i].pos_x > 39)Shapes[i].pos_x = 39;
                if (Shapes[i].pos_y > 39)Shapes[i].pos_y = 39;
            }
            break;

        case 'l':
        case 'L':
            board_Num = 50;
            break;

        case 'e':
        case 'E':
            if (count == 10) {
                for (int i = 0; i < 9; ++i) {
                    Shapes[i] = Shapes[i + 1];
                    temp_Color[i] = temp_Color[i + 1];
                }
                Shapes[9].pos_x = rand() % board_Num;
                Shapes[9].pos_y = rand() % board_Num;
                Shapes[9].r = temp_Color[9].r = rand() % 256;
                Shapes[9].g = temp_Color[9].g = rand() % 256;
                Shapes[9].b = temp_Color[9].b = rand() % 256;
                Shapes[9].input_Shape = CIRCLE;
            }
            else {
                Shapes[count].pos_x = rand() % board_Num;
                Shapes[count].pos_y = rand() % board_Num;
                Shapes[count].r = temp_Color[count].r = rand() % 256;
                Shapes[count].g = temp_Color[count].g = rand() % 256;
                Shapes[count].b = temp_Color[count].b = rand() % 256;
                Shapes[count].input_Shape = CIRCLE;
            }
            if (count != 10)count++;
            break;

        case 't':
        case 'T':
            if (count == 10) {
                for (int i = 0; i < 9; ++i) {
                    Shapes[i] = Shapes[i + 1];
                    temp_Color[i] = temp_Color[i + 1];
                }
                Shapes[9].pos_x = rand() % board_Num;
                Shapes[9].pos_y = rand() % board_Num;
                Shapes[9].r = temp_Color[9].r = rand() % 256;
                Shapes[9].g = temp_Color[9].g = rand() % 256;
                Shapes[9].b = temp_Color[9].b = rand() % 256;
                Shapes[9].input_Shape = TRIANGLE;
            }
            else {
                Shapes[count].pos_x = rand() % board_Num;
                Shapes[count].pos_y = rand() % board_Num;
                Shapes[count].r = temp_Color[count].r = rand() % 256;
                Shapes[count].g = temp_Color[count].g = rand() % 256;
                Shapes[count].b = temp_Color[count].b = rand() % 256;
                Shapes[count].input_Shape = TRIANGLE;
            }
            if (count != 10)count++;
            break;

        case 'r':
        case 'R':
            if (count == 10) {
                for (int i = 0; i < 9; ++i) {
                    Shapes[i] = Shapes[i + 1];
                    temp_Color[i] = temp_Color[i + 1];
                }
                Shapes[9].pos_x = rand() % board_Num;
                Shapes[9].pos_y = rand() % board_Num;
                Shapes[9].r = temp_Color[9].r = rand() % 256;
                Shapes[9].g = temp_Color[9].g = rand() % 256;
                Shapes[9].b = temp_Color[9].b = rand() % 256;
                Shapes[9].input_Shape = RECTANGLE;
            }
            else {
                Shapes[count].pos_x = rand() % board_Num;
                Shapes[count].pos_y = rand() % board_Num;
                Shapes[count].r = temp_Color[count].r = rand() % 256;
                Shapes[count].g = temp_Color[count].g = rand() % 256;
                Shapes[count].b = temp_Color[count].b = rand() % 256;
                Shapes[count].input_Shape = RECTANGLE;
            }
            if (count != 10)count++;
            break;
        case 'c':
        case 'C':
            change_Color++;
            break;
        case 'p':
        case 'P':
            count = 0;
            number = 0;
            break;

        case '1':
            number = 1;
            break;
        case '2':
            number = 2;
            break;
        case '3':
            number = 3;
            break;
        case '4':
            number = 4;
            break;
        case '5':
            number = 5;
            break;
        case '6':
            number = 6;
            break;
        case '7':
            number = 7;
            break;
        case '8':
            number = 8;
            break;
        case '9':
            number = 9;
            break;
        case '0':
            number = 10;
            break;


        case 'q':
        case 'Q':
            exit(1);
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Draw Board       
        for (int i = 0; i < board_Num; i++) {
            for (int j = 0; j < board_Num; j++) {
                Board[i][j].left = 0 + i * BLOCK_SIZE;
                Board[i][j].right = BLOCK_SIZE + i * BLOCK_SIZE;
                Board[i][j].top = 0 + j * BLOCK_SIZE;
                Board[i][j].bottom = BLOCK_SIZE + j * BLOCK_SIZE;
                Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);
            }
        }



        for (int i = 0; i < count; ++i) {

            if (i == number - 1)
            {
                hPen = CreatePen(PS_DOT, 3, RGB(255, 0, 0));
                oldPen = (HPEN)SelectObject(hdc, hPen);
                temp_count = i;
            }

            if (Shapes[i].input_Shape == CIRCLE)
            {
                if (change_Color % 2 == 1)
                {
                    Shapes[i].r = 255;
                    Shapes[i].g = 0;
                    Shapes[i].b = 0;
                }

                else if (change_Color % 2 == 0)
                {
                    Shapes[i].r = temp_Color[i].r;
                    Shapes[i].g = temp_Color[i].g;
                    Shapes[i].b = temp_Color[i].b;
                }

                hBrush = CreateSolidBrush(RGB(Shapes[i].r, Shapes[i].g, Shapes[i].b));


                oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                Ellipse(hdc, Shapes[i].pos_x * BLOCK_SIZE, Shapes[i].pos_y * BLOCK_SIZE,
                    (Shapes[i].pos_x + 1) * BLOCK_SIZE, (Shapes[i].pos_y + 1) * BLOCK_SIZE);
                SelectObject(hdc, oldBrush);
                DeleteObject(hBrush);
            }

            else if (Shapes[i].input_Shape == TRIANGLE)
            {
                if (change_Color % 2 == 1)
                {
                    Shapes[i].r = 0;
                    Shapes[i].g = 255;
                    Shapes[i].b = 0;
                }

                else if (change_Color % 2 == 0)
                {
                    Shapes[i].r = temp_Color[i].r;
                    Shapes[i].g = temp_Color[i].g;
                    Shapes[i].b = temp_Color[i].b;
                }


                hBrush = CreateSolidBrush(RGB(Shapes[i].r, Shapes[i].g, Shapes[i].b));
                oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                tri_Point[0].x = (Board[Shapes[i].pos_x][Shapes[i].pos_y].right + Board[Shapes[i].pos_x][Shapes[i].pos_x].left) / 2;
                tri_Point[0].y = Board[Shapes[i].pos_x][Shapes[i].pos_y].top;
                tri_Point[1].x = Board[Shapes[i].pos_x][Shapes[i].pos_y].right;
                tri_Point[1].y = Board[Shapes[i].pos_x][Shapes[i].pos_y].bottom;
                tri_Point[2].x = Board[Shapes[i].pos_x][Shapes[i].pos_y].left;
                tri_Point[2].y = Board[Shapes[i].pos_x][Shapes[i].pos_y].bottom;
                Polygon(hdc, tri_Point, 3);
                SelectObject(hdc, oldBrush);
                DeleteObject(hBrush);

            }


            else if (Shapes[i].input_Shape == RECTANGLE)
            {
                if (change_Color % 2 == 1)
                {
                    Shapes[i].r = 0;
                    Shapes[i].g = 0;
                    Shapes[i].b = 255;
                }

                else if (change_Color % 2 == 0)
                {
                    Shapes[i].r = temp_Color[i].r;
                    Shapes[i].g = temp_Color[i].g;
                    Shapes[i].b = temp_Color[i].b;
                }

                hBrush = CreateSolidBrush(RGB(Shapes[i].r, Shapes[i].g, Shapes[i].b));
                oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                Rectangle(hdc, Shapes[i].pos_x * BLOCK_SIZE, Shapes[i].pos_y * BLOCK_SIZE,
                    (Shapes[i].pos_x + 1) * BLOCK_SIZE, (Shapes[i].pos_y + 1) * BLOCK_SIZE);

                SelectObject(hdc, oldBrush);
                DeleteObject(hBrush);
            }

            if (i == number - 1)
            {
                SelectObject(hdc, oldPen);
                DeleteObject(hPen);
            }
        }

        TCHAR str1[1000];
        wsprintf(str1, TEXT("%d,       Shapes[10].input_Shape: %d"), count, Shapes[10].input_Shape);
        TextOut(hdc, 700, 100, str1, lstrlen(str1));
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// 1. 10개 그리면 나머지 삭제
// 2. 도형이 범위 벗어날 때 예외처리
// 3. Del 기능
