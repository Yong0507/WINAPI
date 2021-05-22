#include <Windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <math.h>
using namespace std;
default_random_engine dre;
uniform_int_distribution<> uid{ 0,25 };

int Window_Size_X = 800;
int Window_Size_Y = 600;
HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 3-2";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

// 두 점 사이 길이
float LengthPts(int x1, int y1, int x2, int y2)
{
    return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

BOOL InCircle(int x1, int y1, int x2, int y2)
{
    if (LengthPts(x1, y1, x2, y2) < 50)
        return TRUE;
    else
        return FALSE;
}


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
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&WndClass);

    hWnd = CreateWindow
    (lpszClass,
        lpszWindowName,
        (WS_OVERLAPPEDWINDOW),
        0,
        0,
        Window_Size_X,
        Window_Size_Y,
        NULL,
        (HMENU)NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }
    return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    COLORREF text_color;
    COLORREF bk_color;
    HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;
    static RECT play = { 100,100,600,500 };
    static RECT stone[3][10];
    static int state[3][10];
    static RECT bar = { 300,460,400,470 };
    static RECT ball = { 340,440,360,460 };
    static RECT SCORE = { 100,500,150,600 };
    static TCHAR score[100];
    static int mx, my;
    static BOOL Selection;
    static int x, y;
    static int speed;
    static int Rc;
    static int color_0;
    static int color_1;
    static int vec;
    static int count;
    static int location;
    static int doll;
    static int pri;
    static int score_a;
    static int score_b;
    static int score_c;
    static int Gover;
    static int Down_c;

    static int color[3][3];
    static bool isColor[3];
    switch (iMessage)
    {
    case WM_CREATE:
        mx = 0;
        my = 0;
        x = 0;
        y = 0;
        speed = 20;
        Rc = 9;
        color_0 = 0;
        color_1 = 0;
        count = 0;
        vec = 0;
        doll = 0;
        location = 0;
        pri = 0;
        score_a = 0;
        score_b = 0;
        count = 0;
        Gover = 0;

        // 벽돌 가로 40  세로 20
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 10; ++j) {
                stone[i][j].left = 150 + j * 40;
                stone[i][j].top = 100 + i * 20;
                stone[i][j].right = 190 + j * 40;
                stone[i][j].bottom = 120 + i * 20;

            }
        }
        break;
    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);

        //게임 창 생성
        hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
        oldPen = (HPEN)SelectObject(hdc, hPen);
        Rectangle(hdc, play.left, play.top, play.right, play.bottom);

        //1번 줄 벽돌 생성 (가로 40 . 세로 20)

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 10; ++j) {

                hBrush = CreateSolidBrush(RGB(0, 0, 255));
                oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                Rectangle(hdc, stone[i][j].left, stone[i][j].top, stone[i][j].right, stone[i][j].bottom);

                SelectObject(hdc, oldBrush);
                DeleteObject(hBrush);

            }
        }
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (state[i][j] == 1) {
                    for (int k = 0; k < 10; ++k) {

                        hBrush = CreateSolidBrush(RGB(color[i][0], color[i][1], color[i][2]));

                        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                        Rectangle(hdc, stone[i][k].left, stone[i][k].top, stone[i][k].right, stone[i][k].bottom);

                        SelectObject(hdc, oldBrush);
                        DeleteObject(hBrush);
                    }
                }
            }
        }
        TCHAR str1[10];
        wsprintf(str1, TEXT("%d"), Down_c);
        TextOut(hdc, 10, 100, str1, lstrlen(str1));
        //바 생성
        hBrush = CreateSolidBrush(RGB(200, 0, 230));
        oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        Rectangle(hdc, bar.left, bar.top, bar.right, bar.bottom);
        SelectObject(hdc, oldBrush);
        DeleteObject(hBrush);

        //공 생성 20,20

        Ellipse(hdc, ball.left + x, ball.top + y, ball.right + x, ball.bottom + y);

        SelectObject(hdc, oldPen);
        DeleteObject(hPen);


        //
        if (pri % 2 == 1) {
            wsprintf(score, TEXT("변색된 벽돌: %d    제거된 벽돌: %d"), (score_a + score_b + score_c) - Down_c, Down_c);
            TextOut(hdc, SCORE.left, SCORE.top, score, lstrlen(score));
        }
        if (Gover == 1) {
            TextOut(hdc, SCORE.left + 150, SCORE.top, L" GAME OVER <RESET = n/N>", 24);
        }

        EndPaint(hWnd, &ps);

        break;
    case WM_CHAR:
        hdc = GetDC(hWnd);
        // 공튀기기 시작
        if (wParam == 's' || wParam == 'S')
        {
            SetTimer(hWnd, 1, speed, NULL);
            SetTimer(hWnd, 5, 50, NULL);
            SetTimer(hWnd, 7, 200, NULL);

        }
        // 종료
        else if (wParam == 'q' || wParam == 'Q') {
            SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
        // 멈춤, 다시시작 (스코어 표)
        else if (wParam == 'p' || wParam == 'P') {
            if (pri % 2 == 0) {
                pri++;
                KillTimer(hWnd, location);
                KillTimer(hWnd, doll);
                KillTimer(hWnd, 7);
            }
            else if (pri % 2 == 1) {
                pri++;
                SetTimer(hWnd, location, speed, NULL);
                SetTimer(hWnd, doll, 50, NULL);
                SetTimer(hWnd, 7, 200, NULL);

            }
        }

        // 리셋
        else if (wParam == 'n' || wParam == 'N') {
            KillTimer(hWnd, location);
            KillTimer(hWnd, doll);
            Gover = 0;
            mx = 0;
            my = 0;
            x = 0;
            y = 0;
            speed = 20;
            Rc = 9;
            color_0 = 0;
            color_1 = 0;
            count = 0;
            vec = 0;
            doll = 0;
            location = 0;
            pri = 0;
            score_a = 0;
            score_b = 0;
            count = 0;
            Gover = 0;
            stone[0][0] = { 150 ,100,190,120 };
            stone[1][0] = { 150 ,120,190,140 };
            stone[2][0] = { 150 ,120,190,140 };

            for (int i = 0; i < 3; ++i)isColor[i] = false;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 10; ++j) {
                    stone[i][j].left = 150 + j * 40;
                    stone[i][j].top = 100 + i * 20;
                    stone[i][j].right = 190 + j * 40;
                    stone[i][j].bottom = 120 + i * 20;
                    state[i][j] = 0;
                }
            }
        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_KEYDOWN:
        hdc = GetDC(hWnd);
        // 공 속도 증가
        if (wParam == VK_ADD) {
            if (speed > 0) {
                KillTimer(hWnd, location);
                speed--;
                SetTimer(hWnd, location, speed, NULL);
            }
        }
        // 공 속도 감소
        else if (wParam == VK_SUBTRACT) {
            if (speed <= 30) {
                KillTimer(hWnd, location);
                speed++;
                SetTimer(hWnd, location, speed, NULL);
            }
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_TIMER:
        switch (wParam) {


        case 1:
            location = 1;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 10; j++) {
                    if (ball.left + 10 + x > stone[i][j].left && ball.left + 10 + x < stone[i][j].right && ball.top + 10 + y > stone[i][j].top && ball.top + 10 + y < stone[i][j].bottom) {
                        state[i][j] = 1;
                        if (!isColor[i]) {
                            isColor[i] = true;
                            for (int j = 0; j < 3; ++j) {
                                color[i][j] = uid(dre) * 10;
                            }
                        }
                        Down_c++;

                        if (vec == 0)
                        {
                            vec = 1;
                        }
                        else if (vec == 1)
                        {
                            vec = 0;
                        }
                        SetTimer(hWnd, 3, speed, NULL);
                        KillTimer(hWnd, 1);

                    }
                }
            }
            if (ball.left + x == play.left)
            {
                SetTimer(hWnd, 2, speed, NULL);
                KillTimer(hWnd, 1);
            }
            if (ball.right + x == play.right)
            {
                SetTimer(hWnd, 3, speed, NULL);
                KillTimer(hWnd, 1);
            }
            else if (ball.bottom + y == play.bottom)
            {
                x = 0;
                y = 0;
            }
            else {
                if (vec == 0) {
                    x -= 4;
                    y -= 2;

                }
                else if (vec == 1) {
                    x += 4;
                    y -= 2;

                }
            }
            break;
            //왼쪽 벽
        case 2:
            location = 2;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 10; j++) {
                    if (ball.left + 10 + x > stone[i][j].left && ball.left + 10 + x < stone[i][j].right && ball.top + 10 + y > stone[i][j].top && ball.top + 10 + y < stone[i][j].bottom) {
                        state[i][j] = 1;
                        Down_c++;
                        if (!isColor[i]) {
                            isColor[i] = true;
                            for (int j = 0; j < 3; ++j) {
                                color[i][j] = uid(dre) * 10;
                            }
                        }


                        if (i == 0) {
                            color_0 = 200;
                            score_a = 10;
                        }
                        if (i == 1) {
                            color_1 = 200;;
                            score_b = 10;
                        }
                        if (i == 2) {
                            score_c = 10;
                        }
                        SetTimer(hWnd, 3, speed, NULL);
                        KillTimer(hWnd, 2);

                    }
                }
            }
            if (ball.bottom + y >= bar.top && ball.left + x >= bar.left && ball.right + x <= bar.right) {

                SetTimer(hWnd, 1, speed, NULL);
                KillTimer(hWnd, 2);
            }
            if (ball.top + y == play.top)
            {
                SetTimer(hWnd, 3, speed, NULL);
                KillTimer(hWnd, 2);
            }
            if (ball.right + x == play.right) {
                if (vec == 0)
                {
                    vec = 1;
                }
                else if (vec == 1)
                {
                    vec = 0;
                }
                SetTimer(hWnd, 4, speed, NULL);
                KillTimer(hWnd, 2);
            }
            else if (ball.bottom + y == play.bottom)
            {
                x = 0;
                y = 0;
            }
            else {
                if (vec == 0) {
                    x += 4;
                    y -= 2;

                }
                else if (vec == 1) {
                    x += 4;
                    y += 2;

                }
            }
            break;
            //위쪽 벽
        case 3:
            location = 3;
            if (ball.bottom + y >= bar.top && ball.left + x >= bar.left && ball.right + x <= bar.right) {
                if (vec == 0)
                {
                    vec = 1;
                }
                else if (vec == 1)
                {
                    vec = 0;
                }

                SetTimer(hWnd, 1, speed, NULL);
                KillTimer(hWnd, 3);
            }
            else if (ball.right + x == play.right)
            {

                SetTimer(hWnd, 4, speed, NULL);
                KillTimer(hWnd, 3);
            }
            else if (ball.left + x == play.left)
            {

                SetTimer(hWnd, 2, speed, NULL);
                KillTimer(hWnd, 3);
            }
            else if (ball.bottom + y == play.bottom)
            {
                x = 0;
                y = 0;
            }
            else {
                if (vec == 0) {
                    x += 4;
                    y += 2;
                }
                if (vec == 1) {
                    x -= 4;
                    y += 2;
                }
            }
            break;

        case 4:
            location = 4;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 10; j++) {
                    if (ball.left + 10 + x > stone[i][j].left && ball.left + 10 + x < stone[i][j].right && ball.top + 10 + y > stone[i][j].top && ball.top + 10 + y < stone[i][j].bottom) {
                        state[i][j] = 1;
                        if (!isColor[i]) {
                            isColor[i] = true;
                            for (int j = 0; j < 3; ++j) {
                                color[i][j] = uid(dre) * 10;
                            }
                        }
                        Down_c++;


                        if (i == 0) {
                            score_a = 10;
                        }
                        if (i == 1) {
                            score_b = 10;
                        }
                        if (i == 2) {
                            score_c = 10;
                        }
                        SetTimer(hWnd, 3, speed, NULL);
                        KillTimer(hWnd, 4);

                    }
                }
            }
            if (ball.bottom + y >= bar.top && ball.left + x >= bar.left && ball.right + x <= bar.right) {

                SetTimer(hWnd, 1, speed, NULL);
                KillTimer(hWnd, 4);
            }
            if (ball.top + y == play.top)
            {
                SetTimer(hWnd, 3, speed, NULL);
                KillTimer(hWnd, 4);
            }
            if (ball.left + x == play.left)
            {
                if (vec == 0)
                {
                    vec = 1;
                }
                else if (vec == 1)
                {
                    vec = 0;
                }
                SetTimer(hWnd, 2, speed, NULL);
                KillTimer(hWnd, 4);
            }
            else if (ball.bottom + y == play.bottom)
            {
                x = 0;
                y = 0;
            }
            else {
                if (vec == 0) {
                    x -= 4;
                    y += 2;

                }
                else if (vec == 1) {
                    x -= 4;
                    y -= 2;

                }


            }

            break;
            // 벽돌 움직임 좌
        case 5:
            doll = 5;
            if (stone[0][0].left == play.left + 20)
            {
                SetTimer(hWnd, 6, 50, NULL);
                KillTimer(hWnd, 5);
            }
            else {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 10; j++) {
                        if (state[i][j] == 0) {
                            stone[i][j].left -= 2;
                            stone[i][j].right -= 2;
                        }
                    }
                }
            }
            break;
            // 벽돌 움직임 우
        case 6:
            doll = 6;
            if (stone[0][9].right == play.right - 20)
            {
                SetTimer(hWnd, 5, 50, NULL);
                KillTimer(hWnd, 6);
            }
            else {
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 10; j++) {
                        if (state[i][j] == 0) {
                            stone[i][j].left += 2;
                            stone[i][j].right += 2;
                        }
                    }
                }
            }
            break;
            //벽돌 떨어짐
        case 7:
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 10; ++j) {
                    if (state[i][j] == 1) {
                        stone[i][j].top += 20;
                        stone[i][j].bottom += 20;

                    }
                }
            }
            break;

        }

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case WM_LBUTTONDOWN:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);

        if (mx >= bar.left && mx <= bar.right && my >= bar.top && my <= bar.bottom) {
            Selection = TRUE;
        }
        InvalidateRect(hWnd, NULL, TRUE);
        break;
        //바 클릭 해제
    case WM_LBUTTONUP:
        Selection = FALSE;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
        //바 움직이기
    case WM_MOUSEMOVE:
        mx = LOWORD(lParam);
        my = HIWORD(lParam);
        if (Selection)
        {
            if (mx - 50 >= 100 && mx + 50 <= 600) {
                bar.left = mx - 50;
                bar.right = mx + 50;
            }
            InvalidateRect(hWnd, NULL, TRUE);
            break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
        KillTimer(hWnd, 3);
        KillTimer(hWnd, 4);
        KillTimer(hWnd, 5);
        KillTimer(hWnd, 6);
        PostQuitMessage(0);
        break;
        }
    }
    return DefWindowProc(hWnd, iMessage, wParam, lParam);
}