//#include <windows.h>
//#include <tchar.h>
//#include <stdlib.h>
//#define MAX_STR 80
//
//HINSTANCE g_hInst;
//LPCTSTR lpszClass = L"Window Class Name";
//LPCTSTR lpszWindowName = L"Window Programming 2-4";
//LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
//{
//    HWND hWnd;
//    MSG Message;
//    WNDCLASSEX WndClass;
//    g_hInst = hInstance;
//    WndClass.cbSize = sizeof(WndClass);
//    WndClass.style = CS_HREDRAW | CS_VREDRAW;
//    WndClass.lpfnWndProc = (WNDPROC)WndProc;
//    WndClass.cbClsExtra = 0;
//    WndClass.cbWndExtra = 0;
//    WndClass.hInstance = hInstance;
//    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
//    WndClass.hbrBackground =
//        (HBRUSH)GetStockObject(WHITE_BRUSH);
//    WndClass.lpszMenuName = NULL;
//    WndClass.lpszClassName = lpszClass;
//    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//    RegisterClassEx(&WndClass);
//    hWnd = CreateWindow
//    (lpszClass, lpszWindowName,
//        WS_OVERLAPPEDWINDOW,
//        0, 0, 800, 600,
//        NULL, (HMENU)NULL,
//        hInstance, NULL);
//    ShowWindow(hWnd, nCmdShow);
//    UpdateWindow(hWnd);
//    while (GetMessage(&Message, 0, 0, 0)) {
//        TranslateMessage(&Message);
//        DispatchMessage(&Message);
//    }
//    return Message.wParam;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    static SIZE size;
//    static TCHAR str[10][81];
//    static int count;
//    static int R_count;
//
//    static int Line;
//    static int Line_Draw;
//    static int cx;
//
//    static bool Return = false;
//
//    RECT r = { 5,15,0,0 };
//    PAINTSTRUCT ps;
//    HDC hDC;
//    // 메시지 처리하기
//    switch (uMsg) {
//    case WM_CREATE:
//        CreateCaret(hWnd, NULL, 3, 15);
//        ShowCaret(hWnd);
//        count = 0;
//        break;
//    case WM_CHAR:
//        hDC = GetDC(hWnd);
//
//
//        if (wParam == VK_RETURN) {
//            if (Line == 9) {
//                R_count = 0;
//                Line = 0;
//                Return = true;
//                cx = 0;
//            }
//            else {
//                if (Return) {
//
//                    R_count = 0;
//                }
//                else count = 0;
//
//
//                Line++;
//                if (Line_Draw != 9) Line_Draw++;
//            }
//        }
//        else if (wParam == VK_BACK) {
//            if (count == 0) {
//                if (Line == 0) break;
//
//                Line--;
//                if (Return)R_count = lstrlen(str[Line]);
//                count = lstrlen(str[Line]);
//            }
//
//            if (Return) {
//                R_count--;
//                str[Line][R_count] = NULL;
//            }
//            else {
//                count--;
//                str[Line][count] = NULL;
//            }
//        }
//        else {
//
//            if (Return) {
//                str[Line][R_count++] = wParam;
//            }
//            else
//            {
//                str[Line][count] = wParam;
//                count++;
//                str[Line][count] = NULL;
//
//                if (count > MAX_STR)
//                {
//                    str[Line][MAX_STR] = NULL;
//                    Line++;
//                    if (Line_Draw != 9) Line_Draw++;
//                    count = 0;
//                }
//            }
//        }
//
//        if (!Return) {
//            GetTextExtentPoint32(hDC, str[Line], lstrlen(str[Line]), &size);
//            cx = size.cx;
//        }
//        else {
//            GetTextExtentPoint32(hDC, str[Line], R_count, &size);
//
//            cx = size.cx;
//        }
//
//        InvalidateRect(hWnd, NULL, TRUE);
//        break;
//    case WM_PAINT:
//        hDC = BeginPaint(hWnd, &ps);
//
//        for (int i = 0; i < Line_Draw + 1; ++i) {
//            TextOut(hDC, 0, i * 15, str[i], lstrlen(str[i]));
//        }
//
//        SetCaretPos(cx, Line * 15);
//        EndPaint(hWnd, &ps);
//        break;
//    case WM_DESTROY:
//        HideCaret(hWnd);
//        DestroyCaret();
//        PostQuitMessage(0);
//        break;
//    }
//    return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}