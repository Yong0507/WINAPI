#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#define MAX_STR 80

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 2-4";
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
        0, 0, 800, 600,
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
    static SIZE size;
    static SIZE Csize;
    static TCHAR str[10][81];
    static int count[10] = { 0, };
    static int R_count[10] = { 0, };

    static int Line;
    static int Line_Draw;
    static int cx;

    static bool Cmove = false;
    static int CposX;
    static int CposY;

    static bool Return = false;
    static bool Upper = false;
    static bool Insert = false;
    static bool Back = false;

    static bool UPC = false;
    static bool DOWNC = false;

    static int currentC = 0;
    RECT r = { 5,15,0,0 };
    PAINTSTRUCT ps;
    HDC hDC;
    // 메시지 처리하기
    switch (uMsg) {
    case WM_CREATE:
        CreateCaret(hWnd, NULL, 3, 15);
        ShowCaret(hWnd);
        break;
    case WM_CHAR:
        hDC = GetDC(hWnd);

        if (wParam == VK_ESCAPE) {

            // 초기화
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < sizeof(str[Line]) / sizeof(TCHAR); ++j) {
                    str[i][j] = NULL;
                }
            }

            Line = 0;

            for (int i = 0; i < 10; ++i) {
                count[i] = 0;
                R_count[i] = 0;
            }
            cx = 0;
        }

        // 엔터
        else if (wParam == VK_RETURN) {
            Cmove = false;
            if (Line == 9) {
                for (int i = 0; i < 10; ++i) {

                    R_count[i] = 0;
                }

                Line = 0;
                Return = true;
                cx = 0;
            }
            else {
                if (Return) {
                    count[Line] = R_count[Line];
                    R_count[Line] = 0;

                }

                Line++;
                R_count[Line] = 0;
                if (Line_Draw != 9) Line_Draw++;
            }
        }

        // 백스페이스
        else if (wParam == VK_BACK) {

            if (count[Line] == 0) {
                if (Line == 0) break;

                Line--;
            }

            else if (Return) {
                if (count[Line] == 0) {
                    if (Line == 0) break;

                    Line--;
                }
                if (R_count[Line] == 1 && count[Line - 1] > 0 && Line > 0) {
                    //Back = true;
                    R_count[Line] = '!';
                }
                if (R_count[Line] != count[Line] && !Back) {
                    for (int i = R_count[Line]; i < count[Line]; ++i) {
                        str[Line][i] = str[Line][i + 1];
                    }
                    //R_count[Line]--;
                    count[Line] --;
                    //str[Line][count[Line]] = NULL;
                }
                else {

                    R_count[Line] --;
                    count[Line]--;
                    str[Line][R_count[Line]] = NULL;
                }
            }
            else {
                if (CposX != count[Line]) {
                    for (int i = CposX; i < count[Line]; ++i) {
                        str[Line][i] = str[Line][i + 1];

                    }
                    {
                        count[Line]--;
                        str[Line][count[Line]] = NULL;
                    }
                }
                else {
                    Cmove = false;
                    count[Line]--;
                    str[Line][count[Line]] = NULL;
                }
            }
        }

        // TAB 기능 띄워쓰기 4개
        else if (wParam == VK_TAB) {

            for (int i = 0; i < 4; ++i) {

                if (Return) {
                    str[Line][R_count[Line]++] = ' ';
                }
                else
                {
                    str[Line][count[Line]] = ' ';
                    count[Line]++;
                    str[Line][count[Line]] = NULL;

                    if (count[Line] > MAX_STR)
                    {
                        str[Line][MAX_STR] = NULL;
                        Line++;
                        if (Line_Draw != 9) Line_Draw++;
                        count[Line] = 0;
                    }
                }
            }
        }

        // 문자 입력
        else {

            if (Return) {
                if (CposX == R_count[Line]) Cmove = false;
                if (Cmove) {
                    str[Line][CposX] = wParam;

                    if (Upper) {
                        if (str[Line][CposX] >= 'a' && str[Line][CposX] <= 'z') str[Line][CposX] -= 32;
                    }

                    CposX++;
                }
                str[Line][R_count[Line]] = wParam;

                if (Upper) {
                    if (str[Line][R_count[Line]] >= 'a' && str[Line][R_count[Line]] <= 'z') str[Line][R_count[Line]] -= 32;
                }

                if (R_count[Line] == count[Line]) count[Line]++;
                R_count[Line]++;


            }
            else
            {
                if (CposX == count[Line]) Cmove = false;
                if (Cmove) {

                    if (Insert) {
                        str[Line][count[Line]] = '0';
                        str[Line][count[Line] + 1] = NULL;
                        for (int i = count[Line]; i > CposX - 1; --i) {
                            str[Line][i] = str[Line][i - 1];
                        }
                        str[Line][CposX] = wParam;
                        count[Line]++;

                        if (Upper) {
                            if (str[Line][CposX] >= 'a' && str[Line][CposX] <= 'z') str[Line][CposX] -= 32;
                        }
                    }
                    else {
                        str[Line][CposX] = wParam;

                        if (Upper) {
                            if (str[Line][CposX] >= 'a' && str[Line][CposX] <= 'z') str[Line][CposX] -= 32;
                        }
                    }

                    CposX++;
                }
                else
                {
                    str[Line][count[Line]] = wParam;

                    if (Upper) {
                        if (str[Line][count[Line]] >= 'a' && str[Line][count[Line]] <= 'z') str[Line][count[Line]] -= 32;
                    }

                    count[Line]++;
                    str[Line][count[Line]] = NULL;
                }

                if (count[Line] > MAX_STR)
                {
                    str[Line][MAX_STR] = NULL;
                    Line++;
                    if (Line_Draw != 9) Line_Draw++;
                    count[Line] = 0;
                }
            }


        }

        if (Return) 
        {
            GetTextExtentPoint32(hDC, str[Line], R_count[Line], &size);
            cx = size.cx;
            if (!Cmove) CposX = R_count[Line];
            else {
                GetTextExtentPoint32(hDC, str[Line], CposX, &size);
                cx = size.cx;
            }

            if (Back) {
                GetTextExtentPoint32(hDC, str[Line], count[Line], &size);
                cx = size.cx;
            }
        }

        else {

            GetTextExtentPoint32(hDC, str[Line], count[Line], &size);
            cx = size.cx;
            if (!Cmove) CposX = count[Line];
            else {
                GetTextExtentPoint32(hDC, str[Line], CposX, &size);
                cx = size.cx;
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;



    case WM_KEYDOWN:
        hDC = GetDC(hWnd);

        if (wParam == VK_F1) {
            Upper = !Upper;
        }
        if (wParam == VK_INSERT) {
            Insert = !Insert;

        }
        if (wParam == VK_HOME) {
            Insert = false;

            Line = 0;

            Return = true;
            GetTextExtentPoint32(hDC, str[Line], R_count[Line], &size);
            cx = size.cx;




        }

        if (wParam == VK_DELETE) {

        }

        if (wParam == VK_LEFT) {
            Cmove = true;
            CposX--;
            GetTextExtentPoint32(hDC, str[Line], CposX, &size);
            cx = size.cx;
        }

        else if (wParam == VK_RIGHT) {
            if (str[Line][CposX] != NULL) {
                Cmove = true;
                CposX++;
                GetTextExtentPoint32(hDC, str[Line], CposX, &size);
                cx = size.cx;
            }
        }

        else if (wParam == VK_UP) {
            if (Line_Draw > 0) {

                if (CposX > count[Line - 1]) {
                    CposX = count[Line - 1];
                    Line--;
                    GetTextExtentPoint32(hDC, str[Line], CposX, &size);
                    cx = size.cx;
                }
                else {
                    Line--;

                    GetTextExtentPoint32(hDC, str[Line], CposX, &size);
                    cx = size.cx;
                    Cmove = true;
                }
            }
        }

        else if (wParam == VK_DOWN) {
            if (Line_Draw != 0 && Line_Draw != 9) {
                //Cmove = true;
                if (CposX > count[Line + 1]) {
                    CposX = count[Line + 1];
                    Line++;
                    GetTextExtentPoint32(hDC, str[Line], CposX, &size);
                    cx = size.cx;
                }
                else {
                    Line++;

                    GetTextExtentPoint32(hDC, str[Line], CposX, &size);
                    cx = size.cx;
                    Cmove = true;
                }
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);



        TCHAR s[100];

        TCHAR s1[100];

        TCHAR c[100];

        for (int i = 0; i < Line_Draw + 1; ++i) {
            TextOut(hDC, 0, i * 15, str[i], lstrlen(str[i]));
        }
        
        SetCaretPos(cx, Line * 15);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        HideCaret(hWnd);
        DestroyCaret();
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}