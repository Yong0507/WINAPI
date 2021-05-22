#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>

#define Window_Size_X 1000
#define Window_Size_Y 1000
#define BLOCK_SIZE 250

#define RED         1
#define YELLOW      2
#define BLUE        3

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 3-4";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

struct SIGNAL {
    int x, y;
    int status;
    int r, g, b;

    bool isRedBlue;
};

struct UD_CAR {
    RECT rect;
};

struct LR_CAR {
    RECT rect;
};

double LengthPts(int x1, int y1, int x2, int y2);
BOOL InCircle(int x, int y, int x2, int y2);


void DrawSignal(HDC hdc, HBRUSH hBrush, HBRUSH oldBrush, HPEN hPen, HPEN oldPen, SIGNAL signal);
void UD_CarPos1(UD_CAR* car, RECT* Board);
void UD_CarPos2(UD_CAR* car, RECT* Board);
void LR_CarPos1(LR_CAR* car, RECT* Board);
void LR_CarPos2(LR_CAR* car, RECT* Board);

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
    static HBRUSH hBrush, oldBrush;
    static HPEN hPen, oldPen;

    // 보드판 및 자동차
    static RECT Board[3][3];
    static UD_CAR UD_Cars[2];
    static LR_CAR LR_Cars[2];
    static int Board_left;
    static int Board_top;
    static int Board_right;
    static int Board_bottom;
    static int car_speed;

    static POINT left_top;
    static POINT right_bottom;

    // 자동차가 보드판 끝에 닿았을 때 처리
    static bool is_LR_Crushed;
    static bool is_UD_Crushed;
    static RECT temp_Car;
    static int LtoR_Length;
    static int RtoL_Length;
    static int UtoD_Length;
    static int DtoU_Length;

    // 신호등 클릭
    static int mx, my;
    static SIGNAL LR_Signals[3];
    static SIGNAL UD_Signals[3];
    static SIGNAL Change_Signals[2];

    // 신호등 변경을 위한 count
    static int count_input;
    static int count_click;

    switch (uMsg) {

    case WM_CREATE:
        srand((unsigned)time(NULL));

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                Board[i][j].left = 100 + i * BLOCK_SIZE;
                Board[i][j].right = 350 + i * BLOCK_SIZE;
                Board[i][j].top = 100 + j * BLOCK_SIZE;
                Board[i][j].bottom = 350 + j * BLOCK_SIZE;
            }
        }

        // 보드판 기준으로 자동차의 rect값 설정
        UD_CarPos1(&UD_Cars[0], &Board[1][0]);
        UD_CarPos2(&UD_Cars[1], &Board[1][2]);
        LR_CarPos1(&LR_Cars[0], &Board[0][1]);
        LR_CarPos2(&LR_Cars[1], &Board[2][1]);

        // 보드판의 좌측상단과 우측하단 값 정의
        Board_left = 100;
        Board_top = 100;
        Board_right = 850;
        Board_bottom = 850;


        // 자동차 보드판과의 충돌 판단
        is_LR_Crushed = false;
        is_UD_Crushed = false;

        // 신호등 클릭을 위해 신호등 값 구조체에 넣기
      
        LR_Signals[0].x = Board[2][0].left + 10;
        LR_Signals[1].x = Board[2][0].left + 50;
        LR_Signals[2].x = Board[2][0].left + 90;

        for (int i = 0; i < 3; ++i) {
            LR_Signals[i].y = Board[2][0].top + 10;
            LR_Signals[i].status = 0;
        }

        LR_Signals[0].r = 255;
        LR_Signals[0].g = 0;
        LR_Signals[0].b = 0;

        LR_Signals[1].r = 255;
        LR_Signals[1].g = 255;
        LR_Signals[1].b = 0;

        LR_Signals[2].r = 0;
        LR_Signals[2].g = 0;
        LR_Signals[2].b = 255;


        UD_Signals[0].x = Board[2][2].left + 10;
        UD_Signals[1].x = Board[2][2].left + 50;
        UD_Signals[2].x = Board[2][2].left + 90;

        for (int i = 0; i < 3; ++i) {
            UD_Signals[i].y = Board[2][2].top + 10;
            UD_Signals[i].status = 0;
        }

        UD_Signals[0].r = 255;
        UD_Signals[0].g = 0;
        UD_Signals[0].b = 0;

        UD_Signals[1].r = 255;
        UD_Signals[1].g = 255;
        UD_Signals[1].b = 0;

        UD_Signals[2].r = 0;
        UD_Signals[2].g = 0;
        UD_Signals[2].b = 255;
        
        Change_Signals[0].x = Board[2][0].left + 140;
        Change_Signals[0].y = Board[2][0].top + 10;
        Change_Signals[0].status = 0;

        Change_Signals[1].x = Board[2][2].left + 140;
        Change_Signals[1].y = Board[2][2].top + 10;
        Change_Signals[1].status = 0;

        LR_Signals[0].status = RED;
        LR_Signals[1].status = 0;
        LR_Signals[2].status = 0;

        UD_Signals[0].status = 0;
        UD_Signals[1].status = 0;
        UD_Signals[2].status = BLUE;

        car_speed = 30;

        break;
    
    case WM_TIMER:
        switch (wParam) {
        
        case 1:
            
            // 좌 --> 우
            if (LR_Cars[0].rect.right > Board_right) {
                is_LR_Crushed = true;
                LtoR_Length = LR_Cars[0].rect.right - Board_right;

                if (LtoR_Length > 100) {
                    LtoR_Length = 0;
                    is_LR_Crushed = false;
                }
            }

            if (LR_Cars[0].rect.left > Board_right) {
                LR_Cars[0].rect.left = Board_left;
                LR_Cars[0].rect.right = Board_left + 100;
            }

            LR_Cars[0].rect.left += 10;
            LR_Cars[0].rect.right += 10;            


            // 우 --> 좌
            if (LR_Cars[1].rect.left < Board_left) {
                is_LR_Crushed = true;
                RtoL_Length = (LR_Cars[1].rect.left - 100) * -1;
                
                if (RtoL_Length > 100) {
                    RtoL_Length = 0;
                    is_LR_Crushed = false;
                }
            }

            if (LR_Cars[1].rect.right < Board_left) {
                LR_Cars[1].rect.left = Board_right - 100;
                LR_Cars[1].rect.right = Board_right;
            }

            LR_Cars[1].rect.left -= 10;
            LR_Cars[1].rect.right -= 10;

            break;

        case 2:

            // 상 --> 하
            if (UD_Cars[0].rect.bottom > Board_bottom) {
                is_UD_Crushed = true;
                UtoD_Length = UD_Cars[0].rect.bottom - Board_bottom;

                if (UtoD_Length > 100) {
                    UtoD_Length = 0;
                    is_UD_Crushed = false;
                }
            }

            if (UD_Cars[0].rect.top > Board_bottom) {
                UD_Cars[0].rect.top = Board_top;
                UD_Cars[0].rect.bottom = Board_top + 100;
            }

            UD_Cars[0].rect.bottom += 10;
            UD_Cars[0].rect.top += 10;


            // 하 --> 상
            if (UD_Cars[1].rect.top < Board_top) {
                is_UD_Crushed = true;
                DtoU_Length = (UD_Cars[1].rect.top - 100) * -1;

                if (DtoU_Length > 100) {
                    DtoU_Length = 0;
                    is_UD_Crushed = false;
                }
            }

            if (UD_Cars[1].rect.bottom < Board_top) {
                UD_Cars[1].rect.top = Board_bottom - 100;
                UD_Cars[1].rect.bottom = Board_bottom;
            }

            UD_Cars[1].rect.bottom -= 10;
            UD_Cars[1].rect.top -= 10;

            break;

        case 3:                                               
            if (count_input < 3) {
                LR_Signals[0].status = RED;
                LR_Signals[1].status = 0;
                LR_Signals[2].status = 0;

                UD_Signals[0].status = 0;
                UD_Signals[1].status = 0;
                UD_Signals[2].status = BLUE;
            }

            if (count_input >= 3 && count_input < 6) {
                LR_Signals[0].status = 0;
                LR_Signals[1].status = YELLOW;
                LR_Signals[2].status = 0;

                UD_Signals[0].status = 0;
                UD_Signals[1].status = YELLOW;
                UD_Signals[2].status = 0;

            }
            if (count_input >= 6 && count_input < 9) {
                LR_Signals[0].status = 0;
                LR_Signals[1].status = 0;
                LR_Signals[2].status = BLUE;

                UD_Signals[0].status = RED;
                UD_Signals[1].status = 0;
                UD_Signals[2].status = 0;
            }
            if (count_input == 9)
                count_input = 0;
            else {
                count_input++;
            }                           
            break;

        case 4:
                count_click++;

                if (Change_Signals[0].isRedBlue == true) {
                    if (count_click < 3) {
                        LR_Signals[0].status = RED;
                        LR_Signals[1].status = 0;
                        LR_Signals[2].status = 0;

                        UD_Signals[0].status = 0;
                        UD_Signals[1].status = 0;
                        UD_Signals[2].status = BLUE;
                    }

                    if (count_click >= 3 && count_click < 6) {
                        LR_Signals[0].status = 0;
                        LR_Signals[1].status = YELLOW;
                        LR_Signals[2].status = 0;

                        UD_Signals[0].status = 0;
                        UD_Signals[1].status = YELLOW;
                        UD_Signals[2].status = 0;

                    }
                    if (count_click >= 6 && count_click < 9) {
                        LR_Signals[0].status = 0;
                        LR_Signals[1].status = 0;
                        LR_Signals[2].status = BLUE;

                        UD_Signals[0].status = RED;
                        UD_Signals[1].status = 0;
                        UD_Signals[2].status = 0;
                        count_click = 0;
                        Change_Signals[0].isRedBlue = false;
                        KillTimer(hWnd, 4);
                    }
                }


                else if (Change_Signals[0].isRedBlue == false)
                {
                    if (count_click < 3) {
                        LR_Signals[0].status = 0;
                        LR_Signals[1].status = 0;
                        LR_Signals[2].status = BLUE;

                        UD_Signals[0].status = RED;
                        UD_Signals[1].status = 0;
                        UD_Signals[2].status = 0;
                    }

                    if (count_click >= 3 && count_click < 6) {
                        LR_Signals[0].status = 0;
                        LR_Signals[1].status = YELLOW;
                        LR_Signals[2].status = 0;

                        UD_Signals[0].status = 0;
                        UD_Signals[1].status = YELLOW;
                        UD_Signals[2].status = 0;

                    }
                    if (count_click >= 6 && count_click < 9) {
                        LR_Signals[0].status = RED;
                        LR_Signals[1].status = 0;
                        LR_Signals[2].status = 0;

                        UD_Signals[0].status = 0;
                        UD_Signals[1].status = 0;
                        UD_Signals[2].status = BLUE;
                        count_click = 0;
                        Change_Signals[0].isRedBlue = true;
                        KillTimer(hWnd, 4);
                    }
                }
                break;
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_CHAR:
        switch (wParam) {

        case 'a':
            SetTimer(hWnd, 3, 300, NULL);
            break;

        case 'A':
            KillTimer(hWnd, 3);
            break;
        
        case '+':
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            car_speed -= 5;
            SetTimer(hWnd, 1, car_speed, NULL);
            SetTimer(hWnd, 2, car_speed, NULL);
            break;

        case '-':
            KillTimer(hWnd, 1);
            KillTimer(hWnd, 2);
            car_speed += 5;
            SetTimer(hWnd, 1, car_speed, NULL);
            SetTimer(hWnd, 2, car_speed, NULL);
            break;

        case 'q':
        case 'Q':
            exit(1);            
        }
        break;

    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        //for (int i = 0; i < 3; ++i) {
        //    if (InCircle(LR_Signals[i].x + 15, LR_Signals[i].y + 15, mx, my))
        //        LR_Signals[i].status = i + 1;

        //    if (InCircle(UD_Signals[i].x + 15, UD_Signals[i].y + 15, mx, my))
        //        UD_Signals[i].status = i + 1;
        //}

        for (int i = 0; i < 2; ++i) {
            if (InCircle(Change_Signals[i].x + 15, Change_Signals[i].y + 15, mx, my)) {
                Change_Signals[i].status = i + 1;

                if (LR_Signals[0].status == RED) 
                    Change_Signals[i].isRedBlue = true;
                else if(LR_Signals[2].status == BLUE)
                    Change_Signals[i].isRedBlue = false;                
            }
        }

        if (Change_Signals[0].status == 1 || Change_Signals[1].status == 2)
            SetTimer(hWnd, 4, 100, NULL);



        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_LBUTTONUP:
        //for (int i = 0; i < 3; ++i) {
        //    LR_Signals[i].status = 0;
        //    UD_Signals[i].status = 0;
        //}

        //for(int i=0; i<2; ++i)
        //    Change_Signals[i].status = 0;;

        InvalidateRect(hWnd, NULL, TRUE);
        break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Draw 3x3 Board
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                Rectangle(hdc, Board[i][j].left, Board[i][j].top, Board[i][j].right, Board[i][j].bottom);
            }
        }
        
        // Draw 신호등
        Rectangle(hdc, Board[2][0].left, Board[2][0].top, Board[2][0].left + 130, Board[2][0].top + 50);
        Rectangle(hdc, Board[2][2].left, Board[2][2].top, Board[2][2].left + 130, Board[2][2].top + 50);

        for (int i = 0; i < 3; ++i) {
            DrawSignal(hdc, hBrush, oldBrush, hPen, oldPen, LR_Signals[i]);
            DrawSignal(hdc, hBrush, oldBrush, hPen, oldPen, UD_Signals[i]);
        }

        // Draw 자동차
        hBrush = CreateSolidBrush(RGB(0, 180, 230));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
         
        Rectangle(hdc, UD_Cars[0].rect.left, UD_Cars[0].rect.top, UD_Cars[0].rect.right, UD_Cars[0].rect.bottom - UtoD_Length);
        Rectangle(hdc, UD_Cars[1].rect.left, UD_Cars[1].rect.top + DtoU_Length, UD_Cars[1].rect.right, UD_Cars[1].rect.bottom);

        Rectangle(hdc, LR_Cars[0].rect.left, LR_Cars[0].rect.top, LR_Cars[0].rect.right - LtoR_Length, LR_Cars[0].rect.bottom);
        Rectangle(hdc, LR_Cars[1].rect.left + RtoL_Length, LR_Cars[1].rect.top, LR_Cars[1].rect.right, LR_Cars[1].rect.bottom);

        // Draw 충돌 범위 기반 자동차
        if (is_LR_Crushed) {
            Rectangle(hdc, Board_left, LR_Cars[0].rect.top, Board_left + LtoR_Length, LR_Cars[0].rect.bottom);
            Rectangle(hdc, Board_right - RtoL_Length, LR_Cars[1].rect.top, Board_right, LR_Cars[1].rect.bottom);
        }

        if (is_UD_Crushed) {
            Rectangle(hdc, UD_Cars[0].rect.left, Board_top, UD_Cars[0].rect.right, Board_top + UtoD_Length);
            Rectangle(hdc, UD_Cars[1].rect.left, Board_bottom - DtoU_Length, UD_Cars[1].rect.right, Board_bottom);
        }

        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);
        
        // 신호 바꾸기 버튼
        DrawSignal(hdc, hBrush, oldBrush, hPen, oldPen, Change_Signals[0]);
        DrawSignal(hdc, hBrush, oldBrush, hPen, oldPen, Change_Signals[1]);

        for (int i = 0; i < 3; ++i) {
            if (LR_Signals[i].status == BLUE)
                SetTimer(hWnd, 1, car_speed, NULL);
            else if (LR_Signals[i].status == RED)
                KillTimer(hWnd, 1);
        }

        for (int i = 0; i < 3; ++i) {
            if (UD_Signals[i].status == BLUE)
                SetTimer(hWnd, 2, car_speed, NULL);
            else if (UD_Signals[i].status == RED)
                KillTimer(hWnd, 2);
        }

        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void DrawSignal(HDC hdc, HBRUSH hBrush, HBRUSH oldBrush, HPEN hPen, HPEN oldPen, SIGNAL signal)
{
    if (signal.status != 0) {
        hPen = CreatePen(PS_DOT, 5, RGB(100, 150, 150));
        oldPen = (HPEN)SelectObject(hdc, hPen);
    }

    hBrush = CreateSolidBrush(RGB(signal.r, signal.g, signal.b));
    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Ellipse(hdc, signal.x, signal.y, signal.x + 30, signal.y + 30);

    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);

    if (signal.status != 0) {
        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
    }
}

void UD_CarPos1(UD_CAR* car, RECT *Board)
{
    car->rect.left = Board->left + 20;
    car->rect.right = Board->left + 60;
    car->rect.top = Board->top;
    car->rect.bottom = Board->top + 100;
}

void UD_CarPos2(UD_CAR* car, RECT* Board)
{
    car->rect.left = Board->right - 60;
    car->rect.right = Board->right - 20;
    car->rect.top = Board->bottom - 100;
    car->rect.bottom = Board->bottom;
}
void LR_CarPos1(LR_CAR* car, RECT* Board)
{
    car->rect.left = Board->left;
    car->rect.right = Board->left + 100;
    car->rect.top = Board->bottom - 60;
    car->rect.bottom = Board->bottom - 20;
}

void LR_CarPos2(LR_CAR* car, RECT* Board)
{
    car->rect.left = Board->right - 100;
    car->rect.right = Board->right;
    car->rect.top = Board->top + 20;
    car->rect.bottom = Board->top + 60;
}

double LengthPts(int x1, int y1, int x2, int y2) 
{
    return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x, int y, int x2, int y2)
{
    if (LengthPts(x, y, x2, y2) < 15)
        return TRUE;
    else
        return FALSE;
}