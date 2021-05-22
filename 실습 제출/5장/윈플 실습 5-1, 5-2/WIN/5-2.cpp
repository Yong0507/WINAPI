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
//    //���� Ʋ
//    static RECT Parts[5][5];
//    static RECT Veiw[5][5];
//    static int MoveX[5][5];
//    static int MoveY[5][5];
//
//    // ������ ���� ����
//    static int TempX , TempY;
//    static int rand_i, rand_j;
//  
//
//    //������ ����
//    static int BlankX, BlankY;
//    static int Move_Index_X, Move_Index_Y;
//
//    //���� ������ ����
//    static int Block_Parts_Size;
//    static int Block_Image_Size;
//
//    //Ǯ��ũ��
//    static bool FullSceen;
//
//    //�׸� ���� ����
//    static bool Image1;
//    static bool Image2;
//
//    //���� ����
//    static bool GameStart;
//
//    //���콺 ����
//    static int mx, my;
//    static int oldx, oldy;
//    static bool isDrag;
//
//    //���� �׸�
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
//            //���ӽ���
//        case 's':
//        case 'S':
//            if (!Image1 && !Image2)
//                MessageBox(hWnd, L"�׸��� �������� �ʾҽ��ϴ�.", L"����", MB_OK);
//            else if (Block_Parts_Size == 0)
//                MessageBox(hWnd, L"�׸� �����⸦ �������� �ʾҽ��ϴ�.", L"����", MB_OK);
//            else if (GameStart)
//                MessageBox(hWnd, L"�̹� ������ ���� ���Դϴ�.", L"����", MB_OK);
//            else
//                GameStart = true;
//            break;
//
//            // ��ü �׸� ����
//        case 'f':
//        case 'F':
//            FullSceen = !FullSceen;
//            break;
//
//            //����
//        case 'q':
//        case 'Q':
//            if (MessageBox(hWnd, L"�����Ͻðڽ��ϱ�?", L"����", MB_OKCANCEL) == IDOK)
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
//            //������ 
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
//            //����
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
//            //�׸� �ٲٱ�
//        case '1':
//            Image1 = true;
//            Image2 = false;
//            break;
//
//            //�׸� �ٲٱ�
//        case '2':
//            Image2 = true;
//            Image1 = false;
//            break;
//
//            // �׸� ������ 3x3
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
//            // �׸� ������ 4x4
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
//            // �׸� ������ 5x5
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
//            //Ű����� �����̱� �κ� Ȯ�� ���ϰ� �־����.
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
//            //3x3����
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
//            //4x4 ����
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
//            // 5x5 ����
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
//            //���ӽ���
//        case 40006:
//            if (!Image1 && !Image2)
//                MessageBox(hWnd, L"�׸��� �������� �ʾҽ��ϴ�.", L"����", MB_OK);
//            else if (Block_Parts_Size == 0)
//                MessageBox(hWnd, L"�׸� �����⸦ �������� �ʾҽ��ϴ�.", L"����", MB_OK);
//            else if (GameStart)
//                MessageBox(hWnd, L"�̹� ������ ���� ���Դϴ�.", L"����", MB_OK);
//            else
//                GameStart = true;
//            break;
//            // ��ü �׸� ����
//        case 40007:
//            FullSceen = !FullSceen;
//            break;
//
//            //���� �׸�
//        case 40008:
//            Image_NOT = !Image_NOT;
//            break;
//
//            //����
//        case 40009:
//            if (MessageBox(hWnd, L"�����Ͻðڽ��ϱ�?", L"����", MB_OKCANCEL) == IDOK)
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
//            // ������ �̵�
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
//            // ���� �̵�
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
//            // �� �̵�
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
//            // �Ʒ� �̵�
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
//        GetClientRect(hWnd, &crt);  // �׸� ���� crt�� �޾ƿ���
//
//        BackDC = CreateCompatibleDC(hdc);   // ���� ���۸� �� hdc < - back <- rectangle
//        MemDC = CreateCompatibleDC(BackDC);   // ���� ���۸� ��  back <- mdc  <- ��Ʈ�� �̹�����
//        //�̹��� �������� DC �ϳ��� ��������� �׸� �� �ְ� ���� ���� ȭ�鿡 ���縦 �Ѵ�.
//
//        backBit = CreateCompatibleBitmap(hdc, 950, 950);   //��ȭ�� �غ�
//
//        SetBkMode(BackDC, WHITE_BRUSH);
//        oldBitmap = (HBITMAP)SelectObject(BackDC, backBit); //��ȭ�� ����
//
//        FillRect(BackDC, &crt, GetSysColorBrush(COLOR_WINDOW)); //�׸� ���� �Ͼ�� �ҷ��� ����
//        ////////////////////////////////////////////////////
//
//
//        if (Image1) {
//            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
//            SelectObject(MemDC, hBitmap);   //MemDC�� ���� �־� ����.
//        }
//        if (Image2) {
//            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
//            SelectObject(MemDC, hBitmap);   //MemDC�� ���� �־� ����.
//        }
//
//        if (GameStart) {
//            //���� ���� �� ���
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
//            //���� ���� �� ���
//            BitBlt(BackDC, 0, 0, 900, 900, MemDC, 0, 0, SRCCOPY);
//        }
//
//        // ��ü�׸� ���
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
//        // ��������� �ѹ��� �׸��°ɷ�����.
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