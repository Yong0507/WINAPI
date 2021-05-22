//#include <iostream>
//#include <windows.h>
//#include <tchar.h>
//#include <stdlib.h>
//#include <random>
//#include <time.h>
//using namespace std;
//
//struct Stemp
//{
//    int s;
//    int l;
//    int t;
//    int r;
//    int b;
//    int line;
//
//    int sr;
//    int sg;
//    int sb;
//
//    int lr;
//    int lg;
//    int lb;
//};
//
//default_random_engine dre;
//uniform_int_distribution<> uid{ 0,25 };
//#define Window_Size_X 800
//#define Window_Size_Y 600
//#define BLOCK_SIZE 20
//
//#define DRAW_CIRCLE      0
//#define DRAW_TRIANGLE    1 
//#define DRAW_RECTANGLE   2
//
//#define CHANGE_SCALE    4
//#define DOWN            5
//#define UP              6
//#define RIGHT           7
//#define LEFT            8
//
//HINSTANCE g_hInst;
//LPCTSTR lpszClass = L"Window Class Name";
//LPCTSTR lpszWindowName = L"Window Programming 2-9";
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
//        0, 0, Window_Size_X, Window_Size_Y,
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
//
//
//void Draw(Stemp, HDC);
//int rgb[6] = {};
//
//HBRUSH hBrush, oldBrush;
//HPEN hPen, oldPen;
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//
//    PAINTSTRUCT ps;
//    HDC hdc;
//
//
//    // 문자열 -> 숫자 변환 변수들//
//    static TCHAR str[100];
//    static char c_temp[100];
//    static int count;
//    static int temp[6];
//    static int temp_num;
//    static int num;
//    static int c_count = 0;
//
//
//    static bool isShape = false;
//
//    // 저장
//
//    static Stemp shapes[10];
//    static int shape_count;
//    static int prior[6];
//    static int prior_temp[6];
//    static int prior_rgb[6];
//    static int prior_rgb_temp[6];
//
//    static bool isTemp;
//    switch (uMsg) {
//
//    case WM_CREATE:
//        for (int i = 0; i < 3; ++i) {
//            rgb[i] = 255;
//        }
//        break;
//
//    case WM_CHAR:
//
//        if (wParam == VK_RETURN) {
//            if (shape_count > 9) {
//                MessageBox(hWnd, L" 도형 개수 초과! ", L"Error Message", MB_ICONWARNING);
//                break;
//            }
//            // 저장해 놓기
//            for (int i = 0; i < 6; ++i) {
//                prior_temp[i] = temp[i];
//                prior_rgb_temp[i] = rgb[i];
//            }
//            for (int i = 0; i < count + 1; ++i) {
//                if ((str[i] == ' ' || str[i] == NULL) && str[i + 1] != ' ') {
//                    for (int j = i - num; j < i; ++j) {
//                        c_temp[c_count] = str[j];
//                        c_count++;
//                    }
//                    c_count = 0;
//                    temp[temp_num] = atoi(c_temp);
//                    temp_num++;
//
//                    num = 0;
//                    for (int i = 0; i < sizeof(c_temp) / sizeof(char); ++i) {
//                        c_temp[i] = NULL;
//                    }
//                }
//                num++;
//            }
//            if (temp_num != 6) {
//                MessageBox(hWnd, L" 입력값은 6개 입니다.", L"Error Message", MB_ICONWARNING);
//                for (int i = 0; i < lstrlen(str); ++i) {
//                    str[i] = NULL;
//                }
//                temp_num = 0;
//                count = 0;
//                num = 0;
//                break;
//            }
//            if (temp[0] > 4 || temp[0] < 1) {
//                MessageBox(hWnd, L" 일치하는 도형이 없습니다 ", L"Error Message", MB_ICONWARNING);
//                for (int i = 0; i < lstrlen(str); ++i) {
//                    str[i] = NULL;
//                }
//                temp_num = 0;
//                count = 0;
//                num = 0;
//                break;
//            }
//            shapes[shape_count].s = temp[0];
//            shapes[shape_count].l = temp[1];
//            shapes[shape_count].t = temp[2];
//            shapes[shape_count].r = temp[3];
//            shapes[shape_count].b = temp[4];
//            shapes[shape_count].line = temp[5];
//
//
//            isShape = true;
//            // 색
//            for (int i = 0; i < 6; ++i) {
//                rgb[i] = uid(dre) * 10;
//            }
//            shapes[shape_count].sr = uid(dre) * 10;
//            shapes[shape_count].sg = uid(dre) * 10;
//            shapes[shape_count].sb = uid(dre) * 10;
//            shapes[shape_count].lr = uid(dre) * 10;
//            shapes[shape_count].lg = uid(dre) * 10;
//            shapes[shape_count].lb = uid(dre) * 10;
//
//            for (int i = 0; i < lstrlen(str); ++i) {
//                str[i] = NULL;
//            }
//            temp_num = 0;
//            num = 0;
//            count = 0;
//            shape_count++;
//
//        }
//        else if (wParam == VK_BACK) {
//            if (count == 0) break;
//            count--;
//            str[count] = NULL;
//        }
//
//        else if (wParam == '+') {
//            if (!isShape) {
//                MessageBox(hWnd, L" 화면에 도형이 없습니다 ", L"Error Message", MB_ICONWARNING);
//                break;
//            }
//            if (shapes[shape_count - 1].line == 10) {
//                shapes[shape_count - 1].l -= 3;
//                shapes[shape_count - 1].t -= 3;
//                shapes[shape_count - 1].r += 3;
//                shapes[shape_count - 1].b += 3;
//
//            }
//            else shapes[shape_count - 1].line++;
//
//        }
//        else if (wParam == '-') {
//            if (!isShape) {
//                MessageBox(hWnd, L" 화면에 도형이 없습니다 ", L"Error Message", MB_ICONWARNING);
//                break;
//            }
//            if (shapes[shape_count - 1].line == 1) {
//                shapes[shape_count - 1].l += 3;
//                shapes[shape_count - 1].t += 3;
//                shapes[shape_count - 1].r -= 3;
//                shapes[shape_count - 1].b -= 3;
//
//            }
//            else shapes[shape_count - 1].line--;
//        }
//        else if (wParam == 'k') {
//            if (!isShape) {
//                MessageBox(hWnd, L" 화면에 도형이 없습니다 ", L"Error Message", MB_ICONWARNING);
//                break;
//            }
//
//            shapes[shape_count - 1].sr = uid(dre) * 10;
//            shapes[shape_count - 1].sg = uid(dre) * 10;
//            shapes[shape_count - 1].sb = uid(dre) * 10;
//            shapes[shape_count - 1].lr = uid(dre) * 10;
//            shapes[shape_count - 1].lg = uid(dre) * 10;
//            shapes[shape_count - 1].lb = uid(dre) * 10;
//
//        }
//        else if (wParam == 'p') {
//            if (!isShape || shape_count <= 1) {
//                MessageBox(hWnd, L" 도형이 없습니다 ", L"Error Message", MB_ICONWARNING);
//                break;
//            }
//            if (shape_count > 1) {
//                shape_count--;
//                /* isTemp = true;
//                 for (int i = 0; i < 6; ++i) {
//                     prior[i] = temp[i];
//                     prior_rgb[i] = rgb[i];
//
//                     temp[i] = prior_temp[i];
//                     rgb[i] = prior_rgb[i];
//                 }*/
//            }
//        }
//        else if (wParam == 'n') {
//            if (!isShape || shape_count > 10) {
//                MessageBox(hWnd, L" 도형이 없습니다 ", L"Error Message", MB_ICONWARNING);
//                break;
//            }
//            else {
//                shape_count++;
//            }
//            /*          if (isTemp) {
//                          for (int i = 0; i < 6; ++i) {
//                              temp[i] = prior[i];
//                              rgb[i] = prior[i];
//                          }
//                      }*/
//
//        }
//        else {
//            str[count] = wParam;
//            count++;
//            str[count] = NULL;
//        }
//        InvalidateRect(hWnd, NULL, TRUE);
//        break;
//    case WM_KEYDOWN:
//        switch (wParam)
//        {
//        case VK_LEFT:
//            shapes[shape_count - 1].l -= 10;
//            shapes[shape_count - 1].r -= 10;
//            break;
//        case VK_RIGHT:
//            shapes[shape_count - 1].l += 10;
//            shapes[shape_count - 1].r += 10;
//            break;
//        case VK_UP:
//            shapes[shape_count - 1].t -= 10;
//            shapes[shape_count - 1].b -= 10;
//
//            break;
//        case VK_DOWN:
//            shapes[shape_count - 1].t += 10;
//            shapes[shape_count - 1].b += 10;
//
//            break;
//        }
//        InvalidateRect(hWnd, NULL, TRUE);
//
//        break;
//
//    case WM_PAINT:
//        hdc = BeginPaint(hWnd, &ps);
//
//        Rectangle(hdc, 15, 510, 500, 530);
//        TextOut(hdc, 25, 512, str, lstrlen(str));
//        TCHAR str1[1000];
//        for (int i = 0; i < 6; ++i) {
//            wsprintf(str1, TEXT("%d"), temp[i]);
//            TextOut(hdc, 100, 40 + i * 20, str1, lstrlen(str1));
//        }
//
//        Draw(shapes[shape_count - 1], hdc);
//
//        EndPaint(hWnd, &ps);
//        break;
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    }
//
//    return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}
//
//void Draw(Stemp shape, HDC hdc) {
//    int type = shape.s;
//    int L = shape.l;
//    int T = shape.t;
//    int R = shape.r;
//    int B = shape.b;
//    int Line = shape.line;
//
//    hPen = CreatePen(PS_SOLID, Line, RGB(shape.lr, shape.lg, shape.lb));
//    oldPen = (HPEN)SelectObject(hdc, hPen);
//    hBrush = CreateSolidBrush(RGB(shape.sr, shape.sg, shape.sb));
//    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//
//    if (type == 1) {
//
//        MoveToEx(hdc, L, T, NULL);
//        LineTo(hdc, R, B);
//    }
//    if (type == 2) {
//        POINT tri[6] = { {(L + R) / 2,T},{L,B},{R,B} };
//        Polygon(hdc, tri, 3);
//    }
//    if (type == 3) {
//
//        Rectangle(hdc, L, T, R, B);
//
//    }
//    if (type == 4) {
//
//
//        Ellipse(hdc, L, T, R, B);
//
//    }
//    SelectObject(hdc, oldPen);
//    DeleteObject(hPen);
//    SelectObject(hdc, oldBrush);
//    DeleteObject(hBrush);
//}
//
//void Draw(int shape[], HDC hdc) {
//    int type = shape[0];
//    int L = shape[1];
//    int T = shape[2];
//    int R = shape[3];
//    int B = shape[4];
//    int Line = shape[5];
//
//    hPen = CreatePen(PS_SOLID, Line, RGB(rgb[3], rgb[4], rgb[5]));
//    oldPen = (HPEN)SelectObject(hdc, hPen);
//    hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
//    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//
//    if (type == 1) {
//
//        MoveToEx(hdc, L, T, NULL);
//        LineTo(hdc, R, B);
//    }
//    if (type == 2) {
//        POINT tri[6] = { {(L + R) / 2,T},{L,B},{R,B} };
//        Polygon(hdc, tri, 3);
//    }
//    if (type == 3) {
//
//        Rectangle(hdc, L, T, R, B);
//
//    }
//    if (type == 4) {
//        hPen = CreatePen(PS_SOLID, Line, RGB(rgb[3], rgb[4], rgb[5]));
//        oldPen = (HPEN)SelectObject(hdc, hPen);
//        hBrush = CreateSolidBrush(RGB(rgb[0], rgb[1], rgb[2]));
//        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
//
//        Ellipse(hdc, L, T, R, B);
//
//    }
//    SelectObject(hdc, oldPen);
//    DeleteObject(hPen);
//    SelectObject(hdc, oldBrush);
//    DeleteObject(hBrush);
//}