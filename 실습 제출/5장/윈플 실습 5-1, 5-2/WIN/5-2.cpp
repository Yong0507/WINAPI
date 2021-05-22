//#include <iostream>
//#include <windows.h>
//#include <tchar.h>
//#include <stdlib.h>
//#include <random>
//#include <time.h>
//#include "Resource.h"
//#include "Resource_Bit.h"
//
//#define Window_Size_X 915
//#define Window_Size_Y 940
//#define BLOCK_SIZE 20
//
//HINSTANCE g_hInst;
//LPCTSTR lpszClass = L"Window Class Name";
//LPCTSTR lpszWindowName = L"Window Programming 5-2";
//LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
//
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
//    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    PAINTSTRUCT ps;
//    static HDC hdc;
//  
//    //BitMAp
//    HDC BackDC;
//    HDC MemDC;
//    HBRUSH hBrush, oldBrush;
//    HPEN hPen, oldPen;
//
//    static HBITMAP hBitmap;
//    HBITMAP oldBitmap;
//    static HBITMAP backBit = NULL;
//
//    static BITMAP bmp;
//
//    static int hBitx;
//    static int hBity;
//
//    static RECT crt;
//
//    //게임 틀
//    static RECT Parts[5][5];
//    static RECT Veiw[5][5];
//    static int MoveX[5][5];
//    static int MoveY[5][5];
//
//    // 무작위 설정 관련
//    static int TempX , TempY;
//    static int rand_i, rand_j;
//  
//
//    //움직임 관련
//    static int BlankX, BlankY;
//    static int Move_Index_X, Move_Index_Y;
//
//    //사진 나누기 관련
//    static int Block_Parts_Size;
//    static int Block_Image_Size;
//
//    //풀스크린
//    static bool FullSceen;
//
//    //그림 선택 관련
//    static bool Image1;
//    static bool Image2;
//
//    //게임 시작
//    static bool GameStart;
//
//    //마우스 관련
//    static int mx, my;
//    static int oldx, oldy;
//    static bool isDrag;
//
//    //반전 그림
//    static bool Image_NOT;
//    switch (uMsg) {
//
//    case WM_CREATE:
//        Image1 = false;
//        Image2 = false;
//
//        srand(unsigned(time(NULL)));
//
//        hBitx = 900;
//        hBity = 900;
//        break;
//    case WM_CHAR:
//
//        switch (wParam)
//        {
//            //게임시작
//        case 's':
//        case 'S':
//            if (!Image1 && !Image2)
//                MessageBox(hWnd, L"그림을 선택하지 않았습니다.", L"오류", MB_OK);
//            else if (Block_Parts_Size == 0)
//                MessageBox(hWnd, L"그림 나누기를 설정하지 않았습니다.", L"오류", MB_OK);
//            else if (GameStart)
//                MessageBox(hWnd, L"이미 게임이 진행 중입니다.", L"오류", MB_OK);
//            else
//                GameStart = true;
//            break;
//
//            // 전체 그림 보기
//        case 'f':
//        case 'F':
//            FullSceen = !FullSceen;
//            break;
//
//            //종료
//        case 'q':
//        case 'Q':
//            if (MessageBox(hWnd, L"종료하시겠습니까?", L"종료", MB_OKCANCEL) == IDOK)
//                SendMessage(hWnd, WM_CLOSE, 0, 0);
//            break;
//        }
//        break;
//        InvalidateRect(hWnd, NULL, TRUE);
//
//        break;
//    case WM_TIMER:
//        switch (wParam)
//        {
//
//            //오른쪽 
//        case 1:
//
//            if (Veiw[Move_Index_X][Move_Index_Y].left + Block_Image_Size == BlankX && Veiw[Move_Index_X][Move_Index_Y].top == BlankY) {
//                if (MoveX[Move_Index_X][Move_Index_Y] == Block_Image_Size) {
//                    MoveX[Move_Index_X][Move_Index_Y] = 0;
//                    Veiw[Move_Index_X][Move_Index_Y].left += Block_Image_Size;
//                    BlankX -= Block_Image_Size;
//                    KillTimer(hWnd, 1);
//                }
//                else MoveX[Move_Index_X][Move_Index_Y] += 15;
//            }
//
//            break;
//
//            //왼쪽
//        case 2:
//
//            if (Veiw[Move_Index_X][Move_Index_Y].left - Block_Image_Size == BlankX && Veiw[Move_Index_X][Move_Index_Y].top == BlankY) {
//                if (MoveX[Move_Index_X][Move_Index_Y] == -Block_Image_Size) {
//                    Veiw[Move_Index_X][Move_Index_Y].left -= Block_Image_Size;
//                    MoveX[Move_Index_X][Move_Index_Y] = 0;
//                    BlankX += Block_Image_Size;
//                    KillTimer(hWnd, 2);
//                }
//                else MoveX[Move_Index_X][Move_Index_Y] -= 15;
//            }
//
//            break;
//
//        case 3:
//
//            if (Veiw[Move_Index_X][Move_Index_Y].left == BlankX && Veiw[Move_Index_X][Move_Index_Y].top - Block_Image_Size == BlankY) {
//                if (MoveY[Move_Index_X][Move_Index_Y] == -Block_Image_Size) {
//                    Veiw[Move_Index_X][Move_Index_Y].top -= Block_Image_Size;
//                    MoveY[Move_Index_X][Move_Index_Y] = 0;
//                    BlankY += Block_Image_Size;
//                    KillTimer(hWnd, 3);
//                }
//                else MoveY[Move_Index_X][Move_Index_Y] -= 15;
//            }
//
//            break;
//
//        case 4:
//
//            if (Veiw[Move_Index_X][Move_Index_Y].left == BlankX && Veiw[Move_Index_X][Move_Index_Y].top + Block_Image_Size == BlankY) {
//                if (MoveY[Move_Index_X][Move_Index_Y] == Block_Image_Size) {
//                    Veiw[Move_Index_X][Move_Index_Y].top += Block_Image_Size;
//                    MoveY[Move_Index_X][Move_Index_Y] = 0;
//                    BlankY -= Block_Image_Size;
//                    KillTimer(hWnd, 4);
//                }
//                else MoveY[Move_Index_X][Move_Index_Y] += 15;
//            }
//
//            break;
//        }
//        InvalidateRect(hWnd, NULL, FALSE);
//
//        break;
//
//    case WM_KEYDOWN:
//        switch (wParam)
//        {
//            //그림 바꾸기
//        case '1':
//            Image1 = true;
//            Image2 = false;
//            break;
//
//            //그림 바꾸기
//        case '2':
//            Image2 = true;
//            Image1 = false;
//            break;
//
//            // 그림 나누기 3x3
//        case '3':
//            Block_Parts_Size = 3;
//            Block_Image_Size = 900 / 3;
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    Veiw[i][j].left = 0 + i * Block_Image_Size;
//                    Veiw[i][j].top = 0 + j * Block_Image_Size;
//
//                    Parts[i][j].left = 0 + i * Block_Image_Size;
//                    Parts[i][j].top = 0 + j * Block_Image_Size;
//                }
//            }
//
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    TempX = Parts[i][j].left;
//                    TempY = Parts[i][j].top;
//
//                    rand_i = rand() % Block_Parts_Size;
//                    rand_j = rand() % Block_Parts_Size;
//
//                    Parts[i][j].left = Parts[rand_i][rand_j].left;
//                    Parts[i][j].top = Parts[rand_i][rand_j].top;
//
//                    Parts[rand_i][rand_j].left = TempX;
//                    Parts[rand_i][rand_j].top = TempY;
//                }
//            }
//
//            BlankX = Veiw[rand_i][rand_j].left;
//            BlankY = Veiw[rand_i][rand_j].top;
//            Veiw[rand_i][rand_j].left = 9000;
//            Veiw[rand_i][rand_j].top = 9000;
//            break;
//
//
//            // 그림 나누기 4x4
//        case '4':
//            Block_Parts_Size = 4;
//            Block_Image_Size = 900 / 4;
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    Veiw[i][j].left = 0 + i * Block_Image_Size;
//                    Veiw[i][j].top = 0 + j * Block_Image_Size;
//
//                    Parts[i][j].left = 0 + i * Block_Image_Size;
//                    Parts[i][j].top = 0 + j * Block_Image_Size;
//                }
//            }
//
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    TempX = Parts[i][j].left;
//                    TempY = Parts[i][j].top;
//
//                    rand_i = rand() % Block_Parts_Size;
//                    rand_j = rand() % Block_Parts_Size;
//
//                    Parts[i][j].left = Parts[rand_i][rand_j].left;
//                    Parts[i][j].top = Parts[rand_i][rand_j].top;
//
//                    Parts[rand_i][rand_j].left = TempX;
//                    Parts[rand_i][rand_j].top = TempY;
//                }
//            }
//
//            BlankX = Veiw[rand_i][rand_j].left;
//            BlankY = Veiw[rand_i][rand_j].top;
//            Veiw[rand_i][rand_j].left = 9000;
//            Veiw[rand_i][rand_j].top = 9000;
//            break;
//
//
//            // 그림 나누기 5x5
//        case '5':
//            Block_Parts_Size = 5;
//            Block_Image_Size = 900 / 5;
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    Veiw[i][j].left = 0 + i * Block_Image_Size;
//                    Veiw[i][j].top = 0 + j * Block_Image_Size;
//
//                    Parts[i][j].left = 0 + i * Block_Image_Size;
//                    Parts[i][j].top = 0 + j * Block_Image_Size;
//                }
//            }
//
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    TempX = Parts[i][j].left;
//                    TempY = Parts[i][j].top;
//
//                    rand_i = rand() % Block_Parts_Size;
//                    rand_j = rand() % Block_Parts_Size;
//
//                    Parts[i][j].left = Parts[rand_i][rand_j].left;
//                    Parts[i][j].top = Parts[rand_i][rand_j].top;
//
//                    Parts[rand_i][rand_j].left = TempX;
//                    Parts[rand_i][rand_j].top = TempY;
//                }
//            }
//
//            BlankX = Veiw[rand_i][rand_j].left;
//            BlankY = Veiw[rand_i][rand_j].top;
//            Veiw[rand_i][rand_j].left = 9000;
//            Veiw[rand_i][rand_j].top = 9000;
//            break;
//
//            //키보드로 움직이기 부분 확인 편하게 넣어놨음.
//        case VK_RIGHT:
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    if (Veiw[i][j].left + Block_Image_Size == BlankX && Veiw[i][j].top == BlankY) {
//                        Move_Index_X = i;
//                        Move_Index_Y = j;
//                        SetTimer(hWnd, 1, 10, NULL);
//
//                    }
//                }
//            }
//            break;
//
//        case VK_LEFT:
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    if (Veiw[i][j].left - Block_Image_Size == BlankX && Veiw[i][j].top == BlankY) {
//                        Move_Index_X = i;
//                        Move_Index_Y = j;
//                        SetTimer(hWnd, 2, 10, NULL);
//
//                    }
//                }
//            }
//            break;
//
//        case VK_UP:
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    if (Veiw[i][j].left == BlankX && Veiw[i][j].top - Block_Image_Size == BlankY) {
//                        Move_Index_X = i;
//                        Move_Index_Y = j;
//                        SetTimer(hWnd, 3, 10, NULL);
//
//                    }
//                }
//            }
//            break;
//        case VK_DOWN:
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    if (Veiw[i][j].left == BlankX && Veiw[i][j].top + Block_Image_Size == BlankY) {
//                        Move_Index_X = i;
//                        Move_Index_Y = j;
//                        SetTimer(hWnd, 4, 10, NULL);
//
//                    }
//                }
//            }
//            break;
//        }
//        InvalidateRect(hWnd, NULL, FALSE);
//        break;
//
//    case WM_COMMAND:
//        switch (LOWORD(wParam)) {
//        case 40001:
//            Image1 = true;
//            Image2 = false;
//            break;
//        case 40002:
//            Image2 = true;
//            Image1 = false;
//            break;
//
//            //3x3변경
//        case 40003:
//            Block_Parts_Size = 3;
//            Block_Image_Size = 900 / 3;
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    Veiw[i][j].left = 0 + i * Block_Image_Size;
//                    Veiw[i][j].top = 0 + j * Block_Image_Size;
//
//                    Parts[i][j].left = 0 + i * Block_Image_Size;
//                    Parts[i][j].top = 0 + j * Block_Image_Size;
//                }
//            }
//
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    TempX = Parts[i][j].left;
//                    TempY = Parts[i][j].top;
//
//                    rand_i = rand() % Block_Parts_Size;
//                    rand_j = rand() % Block_Parts_Size;
//
//                    Parts[i][j].left = Parts[rand_i][rand_j].left;
//                    Parts[i][j].top = Parts[rand_i][rand_j].top;
//
//                    Parts[rand_i][rand_j].left = TempX;
//                    Parts[rand_i][rand_j].top = TempY;
//                }
//            }
//
//            BlankX = Veiw[rand_i][rand_j].left;
//            BlankY = Veiw[rand_i][rand_j].top;
//            Veiw[rand_i][rand_j].left = 9000;
//            Veiw[rand_i][rand_j].top = 9000;
//            break;
//
//            //4x4 변경
//        case 40004:
//            Block_Parts_Size = 4;
//            Block_Image_Size = 900 / 4;
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    Veiw[i][j].left = 0 + i * Block_Image_Size;
//                    Veiw[i][j].top = 0 + j * Block_Image_Size;
//
//                    Parts[i][j].left = 0 + i * Block_Image_Size;
//                    Parts[i][j].top = 0 + j * Block_Image_Size;
//                }
//            }
//
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    TempX = Parts[i][j].left;
//                    TempY = Parts[i][j].top;
//
//                    rand_i = rand() % Block_Parts_Size;
//                    rand_j = rand() % Block_Parts_Size;
//
//                    Parts[i][j].left = Parts[rand_i][rand_j].left;
//                    Parts[i][j].top = Parts[rand_i][rand_j].top;
//
//                    Parts[rand_i][rand_j].left = TempX;
//                    Parts[rand_i][rand_j].top = TempY;
//                }
//            }
//
//            BlankX = Veiw[rand_i][rand_j].left;
//            BlankY = Veiw[rand_i][rand_j].top;
//            Veiw[rand_i][rand_j].left = 9000;
//            Veiw[rand_i][rand_j].top = 9000;
//            break;
//
//            // 5x5 변경
//        case 40005:
//            Block_Parts_Size = 5;
//            Block_Image_Size = 900 / 5;
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    Veiw[i][j].left = 0 + i * Block_Image_Size;
//                    Veiw[i][j].top = 0 + j * Block_Image_Size;
//
//                    Parts[i][j].left = 0 + i * Block_Image_Size;
//                    Parts[i][j].top = 0 + j * Block_Image_Size;
//                }
//            }
//
//
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    TempX = Parts[i][j].left;
//                    TempY = Parts[i][j].top;
//
//                    rand_i = rand() % Block_Parts_Size;
//                    rand_j = rand() % Block_Parts_Size;
//
//                    Parts[i][j].left = Parts[rand_i][rand_j].left;
//                    Parts[i][j].top = Parts[rand_i][rand_j].top;
//
//                    Parts[rand_i][rand_j].left = TempX;
//                    Parts[rand_i][rand_j].top = TempY;
//                }
//            }
//
//            BlankX = Veiw[rand_i][rand_j].left;
//            BlankY = Veiw[rand_i][rand_j].top;
//            Veiw[rand_i][rand_j].left = 9000;
//            Veiw[rand_i][rand_j].top = 9000;
//            break;
//        
//            //게임시작
//        case 40006:
//            if (!Image1 && !Image2)
//                MessageBox(hWnd, L"그림을 선택하지 않았습니다.", L"오류", MB_OK);
//            else if (Block_Parts_Size == 0)
//                MessageBox(hWnd, L"그림 나누기를 설정하지 않았습니다.", L"오류", MB_OK);
//            else if (GameStart)
//                MessageBox(hWnd, L"이미 게임이 진행 중입니다.", L"오류", MB_OK);
//            else
//                GameStart = true;
//            break;
//            // 전체 그림 보기
//        case 40007:
//            FullSceen = !FullSceen;
//            break;
//
//            //반전 그림
//        case 40008:
//            Image_NOT = !Image_NOT;
//            break;
//
//            //종료
//        case 40009:
//            if (MessageBox(hWnd, L"종료하시겠습니까?", L"종료", MB_OKCANCEL) == IDOK)
//                SendMessage(hWnd, WM_CLOSE, 0, 0);
//            break;
//        }
//        InvalidateRect(hWnd, NULL, FALSE);
//
//        break;
//
//    case WM_LBUTTONDOWN:
//        mx = LOWORD(lParam);
//        my = HIWORD(lParam);
//        isDrag = true;
//        InvalidateRect(hWnd, NULL, FALSE);
//        break;
//    case WM_MOUSEMOVE:
//        if (isDrag) {
//            oldx = LOWORD(lParam);
//            oldy = HIWORD(lParam);
//            
//            // 오른쪽 이동
//            if (mx + 50 < oldx)
//            {
//                for (int i = 0; i < Block_Parts_Size; ++i) {
//                    for (int j = 0; j < Block_Parts_Size; ++j) {
//                        if (Veiw[i][j].left + Block_Image_Size == BlankX && Veiw[i][j].top == BlankY) {
//                            Move_Index_X = i;
//                            Move_Index_Y = j;
//                            SetTimer(hWnd, 1, 10, NULL);
//
//                        }
//                    }
//                }
//            }
//
//            // 왼쪽 이동
//            else if (mx - 50 > oldx)
//            {
//                for (int i = 0; i < Block_Parts_Size; ++i) {
//                    for (int j = 0; j < Block_Parts_Size; ++j) {
//                        if (Veiw[i][j].left - Block_Image_Size == BlankX && Veiw[i][j].top == BlankY) {
//                            Move_Index_X = i;
//                            Move_Index_Y = j;
//                            SetTimer(hWnd, 2, 10, NULL);
//
//                        }
//                    }
//                }
//            }
//
//            // 위 이동
//            else if (my - 50 > oldy)
//            {
//                for (int i = 0; i < Block_Parts_Size; ++i) {
//                    for (int j = 0; j < Block_Parts_Size; ++j) {
//                        if (Veiw[i][j].left == BlankX && Veiw[i][j].top - Block_Image_Size == BlankY) {
//                            Move_Index_X = i;
//                            Move_Index_Y = j;
//                            SetTimer(hWnd, 3, 10, NULL);
//
//                        }
//                    }
//                }
//            }
//
//            // 아래 이동
//            else if (my + 50 < oldy)
//            {
//                for (int i = 0; i < Block_Parts_Size; ++i) {
//                    for (int j = 0; j < Block_Parts_Size; ++j) {
//                        if (Veiw[i][j].left == BlankX && Veiw[i][j].top + Block_Image_Size == BlankY) {
//                            Move_Index_X = i;
//                            Move_Index_Y = j;
//                            SetTimer(hWnd, 4, 10, NULL);
//
//                        }
//                    }
//                }
//            }
//            InvalidateRect(hWnd, NULL, FALSE);
//        }
//        
//        break;
//
//    case WM_LBUTTONUP:
//        isDrag = false;
//        break;
//    case WM_PAINT:
//        hdc = BeginPaint(hWnd, &ps);
//        GetClientRect(hWnd, &crt);  // 그릴 영역 crt에 받아오기
//
//        BackDC = CreateCompatibleDC(hdc);   // 더블 버퍼링 용 hdc < - back <- rectangle
//        MemDC = CreateCompatibleDC(BackDC);   // 더블 버퍼링 용  back <- mdc  <- 비트맵 이미지용
//        //이미지 넣을때는 DC 하나를 만든다음에 그림 을 넣고 내가 보일 화면에 복사를 한다.
//
//        backBit = CreateCompatibleBitmap(hdc, 950, 950);   //도화지 준비
//
//        SetBkMode(BackDC, WHITE_BRUSH);
//        oldBitmap = (HBITMAP)SelectObject(BackDC, backBit); //도화지 세팅
//
//        FillRect(BackDC, &crt, GetSysColorBrush(COLOR_WINDOW)); //그릴 영역 하얀색 할려고 ㅇㅇ
//        ////////////////////////////////////////////////////
//
//
//        if (Image1) {
//            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
//            SelectObject(MemDC, hBitmap);   //MemDC에 루피 넣어 줬어요.
//        }
//        if (Image2) {
//            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
//            SelectObject(MemDC, hBitmap);   //MemDC에 루피 넣어 줬어요.
//        }
//
//        if (GameStart) {
//            //게임 시작 후 출력
//            for (int i = 0; i < Block_Parts_Size; ++i) {
//                for (int j = 0; j < Block_Parts_Size; ++j) {
//                    if(Image_NOT)
//                        BitBlt(BackDC, Veiw[i][j].left + MoveX[i][j], Veiw[i][j].top + MoveY[i][j], Block_Image_Size, Block_Image_Size, MemDC, Parts[i][j].left, Parts[i][j].top, NOTSRCCOPY);
//                    else
//                        BitBlt(BackDC, Veiw[i][j].left + MoveX[i][j], Veiw[i][j].top + MoveY[i][j], Block_Image_Size, Block_Image_Size, MemDC, Parts[i][j].left, Parts[i][j].top, SRCCOPY);
//                }
//            }
//        }
//        else {
//            //게임 시작 전 출력
//            BitBlt(BackDC, 0, 0, 900, 900, MemDC, 0, 0, SRCCOPY);
//        }
//
//        // 전체그림 출력
//        if(FullSceen)
//            if (Image_NOT)
//                BitBlt(BackDC, 0, 0, 900, 900, MemDC, 0, 0, NOTSRCCOPY);
//            else
//                BitBlt(BackDC,0,0,900, 900, MemDC, 0 ,0, SRCCOPY);
//
//        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
//        oldBrush = (HBRUSH)SelectObject(BackDC, hBrush);
//        hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
//        oldPen = (HPEN)SelectObject(BackDC, hPen);
//
//        SelectObject(BackDC, oldPen);
//        DeleteObject(hPen);
//        SelectObject(BackDC, oldBrush);
//        DeleteObject(hBrush);
//
//        
//
//
//        ////////////////////////////////////////////////////
//
//        BitBlt(hdc, 0, 0, 950, 950, BackDC, 0, 0, SRCCOPY);        
//        
//    
//
//        DeleteObject(SelectObject(MemDC, hBitmap));
//        DeleteObject(MemDC);
//        DeleteObject(SelectObject(BackDC, oldBitmap));
//        DeleteDC(BackDC);
//
//
//        //back -> hdc
//        // 결론적으로 한번만 그리는걸로하자.
//
//        EndPaint(hWnd, &ps);
//
//        break;
//
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        break;
//    }
//    return DefWindowProc(hWnd, uMsg, wParam, lParam);
//}