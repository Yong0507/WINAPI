#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <vector>
#include <algorithm>
#include "resource.h"
#include "resource1.h"


#define Window_Size_X 1000
#define Window_Size_Y 1000
#define BLOCK_SIZE 100

#define NOT_USING 1
#define USING 2


using namespace std;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Game - NumTris";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int ROW = 7;
int COLUMN = 9;


// 보드 크기        
int Type = 0;
int RC_Type = 2;

int rate;
int preview_rate;
bool AtUp = false;
class Numbers {

private:
    int numbers;
    RECT board;
    bool status;
    int MoveAnim, AnimFrame;

    bool isFall;
    bool isCheck;

    HBITMAP hbit;

public:

    Numbers(int r) : numbers(2), board({ (rand()% (ROW- RC_Type))  , 0, ((rand()% (ROW - RC_Type)) + 1) , 1 }), status(false), MoveAnim(0), AnimFrame(10), isFall(false), isCheck(false)
    {
   
        if (r >= 0 && r < 25) {
            numbers = 2;
 
        }

        else if (r >= 25 && r < 45) {
            numbers = 4;

        }

        else if (r >= 45 && r < 60) {
            numbers = 8;

        }

        else if (r >= 60 && r < 75) {
            numbers = 16;

        }

        else if (r>= 75 && r < 85) {
            numbers = 32;

        }

        else if (r>= 85 && r < 95) {
            numbers = 64;

        }

        else if (r >= 95 && r < 100) {
            numbers = 128;
        }
    }

    Numbers(int _numbers, RECT _rect, bool _status, int _red, int _green, int _blue, int _moveAnim, int _moveFrame)
    {
        numbers = _numbers;
        board = _rect;
        status = _status;
        MoveAnim = _moveAnim;
        AnimFrame = _moveFrame;
    }

    void setNum(int n) { numbers = n; }
    int getNum() { return numbers; }

    void setRect(RECT r) { board = r; }
    RECT getRect() { return board; }

    void setStatus(bool s) { status = s; }
    bool getStatus() { return status; }


    void setMoveAnim(int anim) { MoveAnim = anim; }
    int getMoveAnim() { return MoveAnim; }

    void setAnimFrame(int frame) { AnimFrame = frame; }
    int getAnimFrame() { return AnimFrame; }

    void setisFall(bool s) { isFall = s; }
    bool getisFall() { return isFall; }

    void setisCheck(bool s) { isCheck = s; }
    bool getisCheck() { return isCheck; }



    // 도형은 정수 단위의 인덱스 --> 도형이 떨어질 때 한번에 1칸씩 한번에 움직이기 때문에 떨어지는 애니메이션이 딱딱하다
    // 인덱스 단위로 1칸이 떨어지기 전에 애니메이션을 주도록 하자
    void DownAnim()
    {
        MoveAnim += BLOCK_SIZE / AnimFrame;
    }

    // 도형이 인덱스 단위로 떨어지도록 하는 것. 
    void DownRect()
    {
        board.bottom += 1;
        board.top += 1;
    }

    // 바닥에 닿으면 고정시키기
    void FixRect()
    {
        board.top = COLUMN - 1 - RC_Type ;
        board.bottom = COLUMN -  RC_Type ;
    }

    // 도형 좌우로 움직이기
    void MoveRect(int dir)
    {
        board.left += dir;
        board.right += dir;
    }

    ~Numbers() {};
};
bool find_Box(vector<Numbers>& v, Numbers& n, int arr[7][9]);
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
    WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
    //비트맵
    HDC FinalDC;
    HDC BackDC;
    HDC MemDC;

    static HBITMAP hfbitmap, oldhfbitmap;
    static HBITMAP hBitmap;
    HBITMAP oldBitmap;
    static HBITMAP backBit = NULL;

    PAINTSTRUCT ps;
    HDC hdc;
    static HBRUSH hBrush, oldBrush;
    HPEN hPen, oldPen;

    // 보드판
    static RECT Board[7][9];
    static int height, width;

    // 보드안에 숫자가 있는지 확인용
    static int Check[7][9];

    // 숫자 2,4,8,16 보드 관리할 벡터
    static vector<Numbers> numbers;
    numbers.reserve(100);



    // 타이머 속도
    static int speed = 0;
    static bool speed_type; // true 일때 speed false 면 PrepareSpeed

    // 떨어지기 전 타이머 속도
    static int PrepareSpeed = 600;

    // 벡터 크기
    static int v_size;

    //Timer2 변수

    static bool FristCheck;
    static bool SecondCheck;
    static bool errorCheck ;
    static int c;

    static int GoalNumber = 0;

    static bool pause;
    static bool GameStart;
    static bool Select_Type;

    // 메뉴
    static bool Is_Start = false;

    switch (uMsg) {

    case WM_CREATE:

        srand((unsigned)time(NULL));
        hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        rate = rand() % 100;
        preview_rate = rand() % 100;
        for (int i = 0; i < ROW - RC_Type; i++) {
            for (int j = 0; j < COLUMN - RC_Type; j++) {
                Board[i][j].left = 0 + i * BLOCK_SIZE;
                Board[i][j].right = 100 + i * BLOCK_SIZE;
                Board[i][j].top = 0 + j * BLOCK_SIZE;
                Board[i][j].bottom = 100 + j * BLOCK_SIZE;
                //Check[i][j] = NOT_USING;
            }
        }


        // 처음에 벡터는 비어있으니까 하나만 넣어놓고 시작한다
        numbers.push_back(Numbers(rate));
        v_size = numbers.size() - 1;
        break;

    case WM_TIMER:
        switch (wParam) {
        case 1:
            KillTimer(hWnd, 2);
            if ((numbers[v_size].getRect()).top == 1) {

                // 맨윗줄은 준비칸이고, 준비칸 지나면 속도증가.
                numbers[v_size].setisFall(true);
                KillTimer(hWnd, 1);
                SetTimer(hWnd, 1, speed, NULL);
                speed_type = true;

            }

            if ((numbers[v_size].getRect()).top+1 < COLUMN - RC_Type) {
                numbers[v_size].DownAnim();

                if (numbers[v_size].getMoveAnim() == BLOCK_SIZE) {
                    numbers[v_size].setMoveAnim(0);
                    numbers[v_size].DownRect();

                    if (Check[numbers[v_size].getRect().left][numbers[v_size].getRect().top + 1] == USING)
                    {

                        if (find_Box(numbers, numbers[v_size], Check))
                        {
                            v_size = numbers.size() - 1;
                            for(int i = 0 ; i < numbers.size(); ++i)
                                if (numbers[i].getNum() == GoalNumber) {
                                    KillTimer(hWnd, 1);
                                    if (MessageBox(hWnd, L"목표 점수 완료 게임을 종료합니다", L"종료", MB_OK) == IDOK)
                                        SendMessage(hWnd, WM_CLOSE, 0, 0);
                                }
                            if (AtUp) {
                                AtUp = false;

                                KillTimer(hWnd, 1);
                                SetTimer(hWnd, 2, 5, NULL);

                            }


                        }
                        else {
       
                            Check[numbers[v_size].getRect().left][numbers[v_size].getRect().top] = USING;
                            if (numbers[v_size].getRect().top == 1) {
                                KillTimer(hWnd, 1);
                                if (MessageBox(hWnd, L"You Failed!!!!!!!!!!!!!!!", L"종료", MB_OK) == IDOK)
                                    SendMessage(hWnd, WM_CLOSE, 0, 0);
                            }
                           
                        }
                        
                        rate = preview_rate;
                        preview_rate = rand() % 100;
                        numbers.push_back(Numbers(rate));
                        v_size = numbers.size() - 1;

                        // 다시 준비상태 타이머로 돌려준다.
                        KillTimer(hWnd, 1);
                        SetTimer(hWnd, 2, 5, NULL);

                    }
                }
            }
            // 바닥에 닿는 시점에 수행해야 할 것들 
            // 1. 바닥 고정
            // 2. 2차원 배열에 상태값 주기
            // 3. 새로운 도형 push_back
            else {
                if (find_Box(numbers, numbers[v_size], Check))
                {
                    v_size = numbers.size() - 1;
                    for (int i = 0; i < numbers.size(); ++i)
                        if (numbers[i].getNum() == GoalNumber) {
                            KillTimer(hWnd, 1);
                            if (MessageBox(hWnd, L"목표 점수 완료 게임을 종료합니다", L"종료", MB_OK) == IDOK)
                                SendMessage(hWnd, WM_CLOSE, 0, 0);
                        }
                    if (AtUp) {
                        AtUp = false;
                        KillTimer(hWnd, 1);
                        SetTimer(hWnd, 2, 5, NULL);
                    }
                }
                else {
                    Check[numbers[v_size].getRect().left][numbers[v_size].getRect().top] = USING;


                    numbers[v_size].FixRect();  // 바닥 고정
                }

                KillTimer(hWnd, 1);
                SetTimer(hWnd, 2, 5, NULL);
                rate = preview_rate;
                preview_rate = rand() % 100;
                numbers.push_back(Numbers(rate));
                v_size = numbers.size() - 1;
            }

            break;

        case 2:
            c = 0;
            for (int i = 0; i < numbers.size(); ++i) {
                
                if (Check[numbers[i].getRect().left][numbers[i].getRect().top + 1] == NOT_USING && numbers[i].getRect().top != 0) {
                    numbers[i].DownAnim();
                    if (numbers[i].getMoveAnim() == BLOCK_SIZE) {
                        numbers[i].setMoveAnim(0);
                        numbers[i].DownRect();

                        if (find_Box(numbers, numbers[i], Check))
                        {
                            v_size = numbers.size() - 1;
                            for (int i = 0; i < numbers.size(); ++i)
                                if (numbers[i].getNum() == GoalNumber) {
                                    KillTimer(hWnd, 1);
                                    if (MessageBox(hWnd, L"목표 점수 완료 게임을 종료합니다", L"종료", MB_OK) == IDOK)
                                        SendMessage(hWnd, WM_CLOSE, 0, 0);
                                }
                            if (!AtUp) {
                                AtUp = false;
                            }
                        }
                        else {
                            Check[numbers[i].getRect().left][numbers[i].getRect().top - 1] = NOT_USING;
                            Check[numbers[i].getRect().left][numbers[i].getRect().top] = USING;
                        }


                    }
                }
                else {
                    c++;
                    if (c == numbers.size()) {
                        SetTimer(hWnd, 1, PrepareSpeed, NULL);
                        speed_type = false;
                        KillTimer(hWnd, 2);
                    }
                }

            }
 
            break;
        }


        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_GAMESTART_START:

            if (!GameStart) {
                if (GoalNumber == 0) {
                    MessageBox(hWnd, L"목표 점수를 설정해 주세요", L"오류", MB_OK);
                }
                else if (!Select_Type) {
                    MessageBox(hWnd, L"보드의 칸수를 설정해 주세요", L"오류", MB_OK);
                }
                else if (speed == 0) {
                    MessageBox(hWnd, L"속도를 설정해 주세요", L"오류", MB_OK);
                }
                else {
                    SetTimer(hWnd, 1, PrepareSpeed, NULL);
                    speed_type = false;
                    GameStart = true;
                    Is_Start = true;
                }
            }
            break;

        case ID_GAMESTART_PAUSE:
            pause = true;
            if (pause)
                KillTimer(hWnd, 1);
            break;

        case ID_GAMESTART_RESUME:
            if (Is_Start) {
                pause = false;
                if (!pause) {
                    if (speed_type)
                        SetTimer(hWnd, 1, speed, NULL);
                    else if (!speed_type)
                        SetTimer(hWnd, 1, PrepareSpeed, NULL);
                }
            }
            break;
        
        case ID_GOALSCORE_512:
            if (!Is_Start) {
                GoalNumber = 512;
                MessageBox(hWnd, L"목표 점수 512", L"알림", MB_OK);
            }
            break;
        case ID_GOALSCORE_1024:
            if (!Is_Start) {
                GoalNumber = 1024;
                MessageBox(hWnd, L"목표 점수 1024", L"알림", MB_OK);
            }
            break;
        case ID_GOALSCORE_2048:
            if (!Is_Start) {
                GoalNumber = 2048;
                MessageBox(hWnd, L"목표 점수 2048", L"알림", MB_OK);
            }
            break;
        case ID_GOALSCORE_4096:
            if (!Is_Start) {
                GoalNumber = 4096;
                MessageBox(hWnd, L"목표 점수 4096", L"알림", MB_OK);
            }
            break;
        case ID_GOALSCORE_8192:
            if (!Is_Start) {
                GoalNumber = 8192;
                MessageBox(hWnd, L"목표 점수 8192", L"알림", MB_OK);
            }
            break;
        case ID_BOARD_5X6:
            if (!Is_Start) {
                Type = 0;
                RC_Type = 2;
                Select_Type = true;
                MessageBox(hWnd, L"보드 칸 : 5x6 ", L"알림", MB_OK);
            }
            break;
        case ID_BOARD_6X8:
            if (!Is_Start) {
                Type = 1;
                RC_Type = 1;
                Select_Type = true;
                MessageBox(hWnd, L"보드 칸 : 6x8 ", L"알림", MB_OK);
            }
            break;
        case ID_BOARD_7X9:
            if (!Is_Start) {
                Type = 2;
                RC_Type = 0;
                Select_Type = true;
                MessageBox(hWnd, L"보드 칸 : 7x9 ", L"알림", MB_OK);
            }
            break;
        case ID_SPEED_NORMAL:
            if (!Is_Start) {
                PrepareSpeed = 600;
                speed = 10;
                MessageBox(hWnd, L"보통 속도", L"알림", MB_OK);
            }
            break;
        case ID_SPEED_SLOW:
            if (!Is_Start) {
                PrepareSpeed = 1000;
                speed = 20;
                MessageBox(hWnd, L"느린 속도", L"알림", MB_OK);
            }
            break;
        case ID_SPEED_FAST:
            if (!Is_Start) {
                PrepareSpeed = 100;
                speed = 2;
                MessageBox(hWnd, L"빠른 속도", L"알림", MB_OK);

            }
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    case WM_LBUTTONDOWN:
        if (Is_Start)
        {
            errorCheck = false;

            for (int i = 0; i < ROW - RC_Type; ++i) {
                for (int j = 0; j < COLUMN - 1 - RC_Type; ++j) {

                    if (Check[i][j] == USING) {
                        for (int c = 0; c < numbers.size(); ++c) {
                            if (numbers[c].getRect().left == i && numbers[c].getRect().top == j) {
                                errorCheck = true;
                            }

                        }
                        if (errorCheck == false) {
                            Check[i][j] = NOT_USING;
                        }
                        else errorCheck = false;
                    }
                }
            }

            if (numbers[v_size].getisFall() == false)
            {
                numbers[v_size].setisFall(true);
                KillTimer(hWnd, 1);
                SetTimer(hWnd, 1, speed, NULL);
                speed_type = true;
            }
            break;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;        
    case WM_KEYDOWN:
        switch (wParam) {
                
        case VK_LEFT:

            if (numbers[v_size].getisFall() == false)
                if(numbers[v_size].getRect().left != 0)
                    numbers[v_size].MoveRect(-1);

            break;

        case VK_RIGHT:
            if (numbers[v_size].getisFall() == false)
                if (numbers[v_size].getRect().left != ROW-RC_Type -1)
                    numbers[v_size].MoveRect(1);
            break;

        case VK_SPACE:
            // 떨어지기 전엔 느린 속도, space 누르면 아래로 확 내려가는 작업 
            if (Is_Start)
            {
                errorCheck = false;

                for (int i = 0; i < ROW - RC_Type; ++i) {
                    for (int j = 0; j < COLUMN - 1 - RC_Type; ++j) {

                        if (Check[i][j] == USING) {
                            for (int c = 0; c < numbers.size(); ++c) {
                                if (numbers[c].getRect().left == i && numbers[c].getRect().top == j) {
                                    errorCheck = true;
                                }

                            }
                            if (errorCheck == false) {
                                Check[i][j] = NOT_USING;
                            }
                            else errorCheck = false;
                        }
                    }
                }

                if (numbers[v_size].getisFall() == false)
                {
                    numbers[v_size].setisFall(true);
                    KillTimer(hWnd, 1);
                    SetTimer(hWnd, 1, speed, NULL);
                    speed_type = true;
                }
            }
            break;
        }

        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_PAINT:


        hdc = BeginPaint(hWnd, &ps);
        
        FinalDC = CreateCompatibleDC(hdc);
        BackDC = CreateCompatibleDC(FinalDC);   // 더블 버퍼링 용 hdc < - back <- rectangle
        MemDC = CreateCompatibleDC(BackDC);   // 더블 버퍼링 용  back <- mdc  <- 비트맵 이미지용
        

        //이미지 넣을때는 DC 하나를 만든다음에 그림 을 넣고 내가 보일 화면에 복사를 한다.

        hfbitmap = CreateCompatibleBitmap(hdc, 1000, 1000);   
        oldhfbitmap = (HBITMAP)SelectObject(FinalDC, hfbitmap); 


        backBit = CreateCompatibleBitmap(FinalDC, 800, 1000);  
        oldBitmap = (HBITMAP)SelectObject(BackDC, backBit); 
        
        // 배경화면
        hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
        SelectObject(MemDC, hBitmap);   
        StretchBlt(FinalDC, 0, 0, 1000, 1000, MemDC, 0, 0, 1024, 1024, SRCCOPY);

        // 미리보기 블럭
        if (preview_rate >= 0 && preview_rate < 25)
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
        else if (preview_rate >= 25 && preview_rate < 45)
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
        else if (preview_rate >= 45 && preview_rate < 60)
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
        else if (preview_rate >= 60 && preview_rate < 75)
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
        else if (preview_rate >= 75 && preview_rate < 85)
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
        else if (preview_rate >= 85 && preview_rate < 95)
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));   
        else if (preview_rate >= 95 && preview_rate < 100) 
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7)); 
        SelectObject(MemDC, hBitmap);
        BitBlt(FinalDC, 700 + Type * 50, 100 - Type * 50,100, 100, MemDC, 0, 0, SRCCOPY);
        DeleteObject(SelectObject(MemDC, hBitmap));

        // 타일 화면
        hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
        SelectObject(MemDC, hBitmap);  
        StretchBlt(BackDC, 0, 0, 500 + Type* 100, 700+ Type * 100, MemDC, 0, 0, 1024, 1024, SRCCOPY);
        DeleteObject(SelectObject(MemDC, hBitmap));

        //맨위 타일들
        for (int i = 0; i < ROW - RC_Type; ++i) {
            hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
            SelectObject(MemDC, hBitmap);   
            BitBlt(BackDC, 0 + i * 100, 0, 100, 100, MemDC, 0, 0, SRCCOPY);
            DeleteObject(SelectObject(MemDC, hBitmap));

        }

        // 블럭
        for (int i = 0; i < numbers.size(); ++i) {
           
            //BitBlt(BackDC, numbers[i].getRect().left* BLOCK_SIZE, numbers[i].getRect().top* BLOCK_SIZE,50,50,)
            if (numbers[i].getNum() == 2) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
            }
            if (numbers[i].getNum() == 4) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
            }
            if (numbers[i].getNum() == 8) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));

            }
            if (numbers[i].getNum() == 16) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));

            }
            if (numbers[i].getNum() == 32) {
               hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));

            }
            if (numbers[i].getNum() == 64) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
            }
            if (numbers[i].getNum() == 128) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
            }
            if (numbers[i].getNum() == 256) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
            }
            if (numbers[i].getNum() == 512) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
            }
            if (numbers[i].getNum() == 1024) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
            }
            if (numbers[i].getNum() == 2048) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
            }
            if (numbers[i].getNum() == 4096) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
            }
            if (numbers[i].getNum() == 8192) {
                hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
            }

            SelectObject(MemDC, hBitmap);   //MemDC에 그릴 비트맵 선택

            BitBlt(BackDC, (numbers[i].getRect()).left* BLOCK_SIZE, (numbers[i].getRect()).top* BLOCK_SIZE + numbers[i].getMoveAnim(), 100, 100, MemDC,0,0, SRCCOPY);      
            DeleteObject(SelectObject(MemDC, hBitmap));

        }
            DeleteObject(MemDC);


        BitBlt(FinalDC, 200 - Type * 50, 100 - Type * 50 , 500+ Type * 100, 700 + Type * 100, BackDC, 0, 0, SRCCOPY);

        BitBlt(hdc, 0, 0, 1000, 1000, FinalDC, 0, 0, SRCCOPY);
 
        DeleteObject(SelectObject(FinalDC, oldhfbitmap));

        DeleteObject(SelectObject(BackDC, oldBitmap));

        DeleteDC(BackDC);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// 현재 블록 떨어졌을 때 삭제하기.
bool find_Box(vector<Numbers>& v, Numbers& n, int arr[7][9])
{
    int FindNum = n.getNum();   // 검사할 숫자기준점을 받아온다.
    bool check = false;         // return 값

    bool LR_Check = false;      
    bool TB_Check = false;

    bool L_Check = false;
    bool R_Check = false;


    int Tempx, Tempy;


    for (int i = 0; i < v.size(); ++i)
    {

        if (v[i].getNum() == FindNum)
        {
            if ((v[i].getRect().left - 1 == n.getRect().left || v[i].getRect().left + 1 == n.getRect().left) && v[i].getRect().top == n.getRect().top)   // 왼쪽 or 오른쪽에 있음
            {
                v[i].setisCheck(true);      //erase 하기 위한 Check 멤버 변수를 true로 변경해준다.

                if (v[i].getRect().left + 1 == n.getRect().left)
                    L_Check = true;
                if (v[i].getRect().left - 1 == n.getRect().left)
                    R_Check = true;

                arr[n.getRect().left][n.getRect().top] = USING;           // 내가 착지한 곳을 사용중이라고 Check2차원 배열에 값을 바꿔준다. 밑에서 erase를 해주기 때문에, 인덱스 번호 달라져서 여기서 했음.
                arr[v[i].getRect().left][v[i].getRect().top] = NOT_USING; // 삭제할 값에 있는 check 2차원 배열 값을  NOT_USING으로 바꿔준다.
                
                if (arr[v[i].getRect().left][v[i].getRect().top - 1] == USING) {    // 좌우 삭제후, 좌우 삭제 한 곳 위에 박스 들이 있으면 한칸씩 떨궈야 하기 때문에 NOT_USING으로 바꾸고 AtUP ture
                    arr[v[i].getRect().left][v[i].getRect().top - 1] = NOT_USING;   
                    AtUp = true;    // 전역 bool 변수 이다. 이걸 통해서 Timer2 에서 이 함수를 재귀적으로 호출 한다.
                }            
                else AtUp = true;  //재귀적으로 호출할 필요가 없다고 판단. false로 바꿔준다.


                if (arr[v[i].getRect().left][v[i].getRect().top + 1] == USING) {    
                   // AtUp = true;
                }

                Tempx = v[i].getRect().left;        // 2개 이상 삭제 될때 여기서 검사했던 값을 미리 잠깐 받아온다. 두개 이상 검사시 좌우 -> 상하 순서면 상관없지만
                Tempy = v[i].getRect().top;         //  상하 -> 좌우 순으로 검사를 하게 되면 처리 안되는 부분이 있기 때문에 잠깐 인덱스값을 빌려와서 밑에 LR , TB bool 값을 통해 처리 단계에서 사용한다.


                LR_Check = true;        // 좌우 중 삭제할 것이 있다고 알리기 위한 bool --> setNum 연산에 사용된다.
                check = true;           // 전체적으로 삭제할 것이 있엇다고 알리기 위한 bool .. return 값으로 쓰인다.
            }
            else if( (v[i].getRect().top == n.getRect().top+1) && v[i].getRect().left == n.getRect().left)   // 위 or 아래에 있음
            {
                v[i].setisCheck(true);      //erase 하기 위한 Check 멤버 변수를 true로 변경해준다.
                AtUp = true;                // 위에서 떨어져서 합쳐진건 무조건 다시 밑으로 내려가야되기 때문에 어떤조건에도 true를 준다.
                TB_Check = true;            //  위아래 중 삭제할 것이 있었다고 알리기 위한 bool --> setNum 연산에 사용된다.
                check = true;               // 전체적으로 삭제할 것이 있엇다고 알리기 위한 bool .. return 값으로 쓰인다.

                arr[v[i].getRect().left][v[i].getRect().top] = NOT_USING;       //삭제할 값에 있는 check 2차원 배열 값을 NOT_USING으로 바꿔준다.
                //arr[v[i].getRect().left][v[i].getRect().top+1] = NOT_USING;
            }
        }

    }
    if (LR_Check && TB_Check) {
        n.setNum(n.getNum() * 3 + n.getNum());          //  2개 이상 setNum
        arr[Tempx][Tempy] = NOT_USING;                  //  상하 -> 좌우 순으로 검사했을때, 충돌 검사한 블럭이 제자리에 있기 되기 때문에, 그걸 풀어주기 위해서 사용했다.
        arr[n.getRect().left][n.getRect().top + 1] = NOT_USING; 
        arr[n.getRect().left][n.getRect().top - 1] = NOT_USING;
        arr[n.getRect().left][n.getRect().top] = NOT_USING;


        AtUp = true;            // 위 for문에서 연산 과정에서 위를 체크하고 마지막으로 아래를 체크하면, 밑으로 안내려가기 때문에 강제적으로 AtUP = true를 줘서 내 블록이 내려가게 만들어준다.

    }
    ///// 여기는 경우의 수 있으면 연산처리하고 블록 숫자 바꿔주는 과정인데, 설명하기 좀 힘듬.. ///
    else if (LR_Check || TB_Check)
    {
        if (LR_Check) {
            if (L_Check && R_Check)
                n.setNum(n.getNum() * 2);
            else if (arr[n.getRect().left][n.getRect().top + 1] == USING) {
                for (int i = 0; i < v.size(); ++i) {
                    if (v[i].getRect().left == n.getRect().left && n.getRect().top + 1 == v[i].getRect().top)
                        if (n.getNum() * 2 == v[i].getNum()) {
                            v[i].setisCheck(true);
                            arr[n.getRect().left][n.getRect().top + 1] = NOT_USING;
                            n.setNum(n.getNum() * 2);
                        }
                }
            }
            else if(L_Check && arr[n.getRect().left+1][n.getRect().top] == USING)
                for (int i = 0; i < v.size(); ++i) {
                    if (v[i].getRect().left - 1 == n.getRect().left && n.getRect().top == v[i].getRect().top)
                        if (n.getNum() * 2 == v[i].getNum()) {
                            v[i].setisCheck(true);
                            arr[n.getRect().left + 1][n.getRect().top] = NOT_USING;
                            n.setNum(n.getNum() * 2);
                        }
                }
            else if (R_Check && arr[n.getRect().left - 1][n.getRect().top] == USING)
                for (int i = 0; i < v.size(); ++i) {
                    if (v[i].getRect().left + 1 == n.getRect().left && n.getRect().top == v[i].getRect().top)
                        if (n.getNum() * 2 == v[i].getNum()) {
                            v[i].setisCheck(true);
                            arr[n.getRect().left - 1][n.getRect().top] = NOT_USING;
                            n.setNum(n.getNum() * 2);
                        }
                }

        }
        n.setNum(n.getNum() * 2);
    }

    
    //벡터 이레이즈

    for (std::vector<Numbers>::iterator iter = v.begin(); iter != v.end(); ) {
        if ((*iter).getisCheck())
            iter = v.erase(iter);
        else
            ++iter;
    }


    if (check) 
        return true;
    else 
        return false;

}
