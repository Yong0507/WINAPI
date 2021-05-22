#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <atlimage.h>
#include <string.h>

#include "Image.h"
#include "Wall.h"
#include "State.h"
#include "Define.h"


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Window Programming 5-7";
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

void LoadImage();
bool CollisionHelper(RECT, RECT);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hdc, memdc1, memdc2;
    HBITMAP hBitmap1, hBitmap2;
    HBRUSH hBrush, oldBrush;

    // Player 위치, 애니메이션, 크기, 칸수 등등
    static int p_x, p_y;
    static int p_anim;
    static int p_dir;
    static int p_imageCount;
    static int jump_y;

    // Monster 위치, 애니메이션, 크기, 칸수 등등
    static int m_x[MONSTER_AMOUNT];
    static int m_y[MONSTER_AMOUNT];
    static int m_anim;
    static int m_imageCount;
    static int m_dir[MONSTER_AMOUNT] = { -1,-1,-1 };
    static int m_spawn_time;
    static int m_count;

    // Bullet 위치, 방향, 개수
    static int b_x[BULLET_AMOUNT];
    static int b_y[BULLET_AMOUNT];
    static int b_dir[BULLET_AMOUNT];
    static int bullet_count;
    static bool is_bullet;

    // 충돌 박스 
    static RECT m_rect[MONSTER_AMOUNT];
    static RECT b_rect[BULLET_AMOUNT];
    static RECT p_rect;

    // jump
    static int jumpForce;

    // direction
    static int dir;

    // scroll
    static int scroll_top_x = 1;
    static int scroll_bottom_x = 1;
    switch (uMsg) {

    case WM_CREATE:
        srand((unsigned)time(NULL));
        LoadImage();

        p_x = 500;
        p_y = 720;

        jump_y = p_y;

        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            m_y[i] = 700;
        }

        player = frog_idle;

        p_state = PLAYER::IDLE;
        m_state = MONSTER::IDLE;

        SetTimer(hWnd, 1, 100, NULL);
        SetTimer(hWnd, 2, 100, NULL);
        SetTimer(hWnd, 4, 100, NULL);
        SetTimer(hWnd, 5, 100, NULL);
        SetTimer(hWnd, 6, 100, NULL);

        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                Board[i][j].left = 0 + i * BLOCK_SIZE;
                Board[i][j].right = 30 + i * BLOCK_SIZE;
                Board[i][j].top = 0 + j * BLOCK_SIZE;
                Board[i][j].bottom = 30 + j * BLOCK_SIZE;
            }
        }

        break;

    case WM_TIMER:
        switch (wParam) {

            // 1번 타이머 - Player 애니메이션
        case 1:
            p_anim += P_IMAGE_SIZE;
            if (p_anim == P_IMAGE_SIZE * (p_imageCount - 1))
                p_anim = 0;
            break;

            // 2번 타이머 - Monster 애니메이션
        case 2:
            m_anim += M_IMAGE_SIZE;
            if (m_anim == M_IMAGE_SIZE * (m_imageCount - 1))
                m_anim = 0;
            break;

            // 3번 타이머 - Bullet 애니메이션
        case 3:
            for (int i = 0; i < bullet_count; ++i) {
                b_x[i] += (20 * b_dir[i]);
            }
            break;

            // 4번 타이머 - Monster 움직임
        case 4:
            for (int i = 0; i < MONSTER_AMOUNT; ++i) {
                m_x[i] += 10 * m_dir[i];
                if (m_x[i] == -100)
                    m_dir[i] = 0;
                if (m_x[i] < 0) {
                    //m_x[0] = 10;
                    m_dir[i] = 1;
                }
                if (m_x[i] > 900)
                    m_dir[i] = -1;
            }
            break;

            // 5번 타이머 - 횡 스크롤
        case 5:
            scroll_top_x += 3;
            scroll_bottom_x += 6;

            if (scroll_top_x > Window_Size_X)
                scroll_top_x = 0;
            if (scroll_bottom_x > Window_Size_X)
                scroll_bottom_x = 0;
            break;

            // 6번 타이머 - 몬스터 일정시간 간격으로 생성
        case 6:
            m_spawn_time++;
            if (m_spawn_time > 30)
            {
                m_spawn_time = 0;
                if (m_count < MONSTER_AMOUNT) {
                    m_x[m_count] = rand() % 1100;
                    m_count++;
                }
            }
            break;

            // 7번 타이머 - JUMP
        case 7:
            jumpForce++;
            if (jumpForce > 40)
                jumpForce -= 4;

            break;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'j':
        case 'J':
            SetTimer(hWnd, 7, 10, NULL);
            p_state = PLAYER::JUMP;
            break;
        case 'q':
        case 'Q':
            exit(1);
            break;
        }
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        //case VK_UP:
        //    p_y -= 3;

        //    p_state = PLAYER::MOVE;
        //    break;
        //case VK_DOWN:
        //    p_y += 3;

        //    p_state = PLAYER::MOVE;
        //    break;
        case VK_LEFT:
            p_anim = 0; // p_anim을 초기화 해주지 않으면 좌,우 변경 시 애니메이션이 어긋나게 됨
            p_dir = P_DIR_LEFT;
            p_x -= 3;

            p_state = PLAYER::MOVE;
            break;
        case VK_RIGHT:
            p_anim = 0;
            p_dir = P_DIR_RIGHT;
            p_x += 3;

            p_state = PLAYER::MOVE;
            break;
        
        case VK_SPACE:
            p_anim = 0;
            p_state = PLAYER::ATTACK;
            SetTimer(hWnd, 3, 100, NULL);
            // 스페이스 누르는 순간 플레이어가 어느 방향을 바라보는지 판단하고 플레이어보다 좀 더 앞선 위치에 총알을 그린다
            if (p_dir == P_DIR_LEFT) {
                b_x[bullet_count] = p_x - 15;
                b_dir[bullet_count] = -1;
            }

            if (p_dir == P_DIR_RIGHT) {
                b_x[bullet_count] = p_x + 15;
                b_dir[bullet_count] = 1;

            }

            b_y[bullet_count] = p_y + 15;

            is_bullet = true;

            if (bullet_count > BULLET_AMOUNT)
                bullet_count = 0;

            bullet_count++;

            break;

        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_KEYUP:
        p_state = PLAYER::IDLE;
        break;

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);

        memdc1 = CreateCompatibleDC(hdc);
        hBitmap1 = CreateCompatibleBitmap(hdc, Window_Size_X, Window_Size_Y);
        SelectObject(memdc1, hBitmap1);

        // 1. 배경 그리기

        // 전체적인 배경 속도 일정하게 하려면 밑에 처럼하면 됨
        //bg1.Draw(memdc1, -Window_Size_X + scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
        //bg2.Draw(memdc1, scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);


        bg1.Draw(memdc1, -Window_Size_X + scroll_top_x, 0, Window_Size_X, Window_Size_Y / 2, 0, 0, bg_width, bg_height / 2);
        bg2.Draw(memdc1, scroll_top_x, 0, Window_Size_X, Window_Size_Y / 2, 0, 0, bg_width, bg_height / 2);

        bg1.Draw(memdc1, -Window_Size_X + scroll_bottom_x, Window_Size_Y / 2, Window_Size_X, Window_Size_Y, 0, bg_height / 2, bg_width, bg_height);
        bg2.Draw(memdc1, scroll_bottom_x, Window_Size_Y / 2, Window_Size_X, Window_Size_Y, 0, bg_height / 2, bg_width, bg_height);

        // 2. 발판 그리기
        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                if (Check[i][j] == WALL)
                {
                    wall.Draw(memdc1, Board[i][j].left, Board[i][j].top, wall_width, wall_height);
                }
            }
        }

        // 3. 몬스터 그리기
        switch (m_state)
        {
        case MONSTER::IDLE:
            m_imageCount = 11;
            monster = monster_idle;
            for (int i = 0; i < m_count; ++i) {
                monster.Draw(memdc1, m_x[i], m_y[i], 44, 42, m_anim, 0, 44, 42);
            }
            break;
        case MONSTER::DEAD:
            //monster = monster_idle;
            //monster.Draw(memdc1, m_x[0], m_y[0], 44, 42, m_anim, 0, 44, 42);           
            //monster.Draw(memdc1, m_x[1], m_y[1], 44, 42, m_anim, 0, 44, 42);
            //monster.Draw(memdc1, m_x[2], m_y[2], 44, 42, m_anim, 0, 44, 42);
            break;
        }

        // 4. 플레이어 그리기
        switch (p_state)
        {
        case PLAYER::IDLE:
            p_imageCount = 11;
            player = frog_idle;
            player.Draw(memdc1, p_x, p_y, 32, 32, p_anim, p_dir, 32, 32);
            break;
        case PLAYER::MOVE:
            p_imageCount = 12;
            player = frog_move;
            player.Draw(memdc1, p_x, p_y, 32, 32, p_anim, p_dir, 32, 32);
            break;
        case PLAYER::JUMP:
            player = frog_jump;
            player.Draw(memdc1, p_x, p_y - jumpForce, 32, 32, 0, p_dir, 32, 32);
            break;
        case PLAYER::FALL:
            player = frog_fall;
            player.Draw(memdc1, p_x, p_y, 32, 32, 0, p_dir, 32, 32);
            break;
        case PLAYER::ATTACK:
            p_imageCount = 7;
            player = frog_attack;
            player.Draw(memdc1, p_x, p_y, 32, 32, p_anim, 0, 32, 32);
            break;
        case PLAYER::DEAD:
            break;
        }

        // 5. 총알 그리기
        if (is_bullet)
        {
            for (int i = 0; i < bullet_count; ++i) {
                bullet.Draw(memdc1, b_x[i] + 10, b_y[i], 16, 16, 0, 0, 16, 16);

            }
        }

        // 6. 충돌처리를 위한 RECT 범위 설정

        // 6-1. 총알 rect 범위 설정
        for (int i = 0; i < bullet_count; ++i) {
            b_rect[i].left = b_x[i] + 10;
            b_rect[i].right = b_x[i] + 26;
            b_rect[i].top = b_y[i];
            b_rect[i].bottom = b_y[i] + 16;
        }

        // 6-2. 몬스터 rect 범위 설정
        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            m_rect[i].left = m_x[i];
            m_rect[i].right = m_x[i] + 44;
            m_rect[i].top = m_y[i];
            m_rect[i].bottom = m_y[i] + 42;
        }

        // 6-3. 플레이어 rect 범위 설정
        p_rect.left = p_x;
        p_rect.right = p_x + 32;
        p_rect.top = p_y;
        p_rect.bottom = p_y + 32;


        // 7. 충돌처리

        // 7-1. 총알 <-> 몬스터 충돌처리
        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            for (int j = 0; j < BULLET_AMOUNT; ++j) {
                if (CollisionHelper(m_rect[i], b_rect[j]))
                {
                    //effect.Draw(memdc1, m_x[i], m_y[i], 65, 65, 0, 0, 65, 65);

                    m_x[i] = -100;
                    m_y[i] = -100;
                    b_x[j] = -100;
                    b_y[j] = -100;
                }
            }
        }

        // 7-2. 플레이어 <-> 몬스터 충돌처리
        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            if (CollisionHelper(m_rect[i], p_rect))
            {
                effect.Draw(memdc1, p_rect.left, p_rect.top, 65, 65, 0, 0, 65, 65);

            }
        }



        //// RECT 테스트

        //for (int i = 0; i < bullet_count; ++i) {
        //    Rectangle(memdc1, b_rect[i].left, b_rect[i].top, b_rect[i].right, b_rect[i].bottom);
        //}

        //for (int i = 0; i < MONSTER_AMOUNT; ++i) {
        //    Rectangle(memdc1, m_rect[i].left, m_rect[i].top, m_rect[i].right, m_rect[i].bottom);
        //}

        //Rectangle(memdc1, p_rect.left, p_rect.top, p_rect.right, p_rect.bottom);



        BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);

        DeleteObject(SelectObject(memdc1, hBitmap1));
        DeleteDC(memdc1);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void LoadImage()
{
    bg1.Load(L"background.png");
    bg2.Load(L"background.png");
    bg_width = bg1.GetWidth();
    bg_height = bg1.GetHeight();

    wall.Load(L"wall.png");
    wall_width = wall.GetWidth();
    wall_height = wall.GetHeight();

    monster_idle.Load(L"monster_idle.png");
    monster_dead.Load(L"monster_dead.png");

    frog_idle.Load(L"frog_idle.png");
    frog_move.Load(L"frog_move.png");
    frog_jump.Load(L"frog_jump.png");
    frog_fall.Load(L"frog_fall.png");
    frog_attack.Load(L"frog_attack.png");

    bullet.Load(L"bullet.png");

    effect.Load(L"effect.png");
}

bool CollisionHelper(RECT r1, RECT r2)
{
    if (r1.left > r2.right) return false;
    if (r1.top > r2.bottom) return false;
    if (r1.right < r2.left) return false;
    if (r1.bottom < r2.top) return false;

    return true;
}