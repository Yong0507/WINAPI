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
LPCTSTR lpszWindowName = L"Term Project";
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

    // Player ��ġ, �ִϸ��̼�, ũ��, ĭ�� ���
    static int p_x, p_y;
    static int p_anim;
    static int p_dir;
    static int p_imageCount;

    // Monster ��ġ, �ִϸ��̼�, ũ��, ĭ�� ���
    static int m_x[MONSTER_AMOUNT];
    static int m_y[MONSTER_AMOUNT];
    static int m_anim;
    static int m_imageCount;
    static int m_dir[MONSTER_AMOUNT] = { -1,-1,-1 };

    // Bullet ��ġ, ����, ����
    static int b_x[BULLET_AMOUNT];
    static int b_y[BULLET_AMOUNT];
    static int b_dir[BULLET_AMOUNT];
    static int bullet_count;
    static bool is_bullet;

    // �浹 �ڽ� 
    static RECT m_rect[MONSTER_AMOUNT];
    static RECT b_rect[BULLET_AMOUNT];
    static RECT p_rect;
    static RECT temp_rect;

    // �浹 ����
    static bool mb_isCollide;
    static int mb_collide_x;
    static int mb_collide_y;

    // BackGround for Scroll 
    static int scroll_x;

    

    // jump
    static int jumpForce;

    // direction
    static int dir;

    switch (uMsg) {

    case WM_CREATE:
        LoadImage();

        p_x = 500;
        p_y = 500;
        m_x[0] = 300;
        m_x[1] = 300 + 44 * 1;
        m_x[2] = 300 + 44 * 2;

        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            m_y[i] = 350;
        }

        player = frog_idle;

        p_state = PLAYER::IDLE;
        m_state = MONSTER::IDLE;

        SetTimer(hWnd, 1, 100, NULL);
        SetTimer(hWnd, 2, 100, NULL);
        SetTimer(hWnd, 3, 100, NULL);
        SetTimer(hWnd, 4, 100, NULL);
        SetTimer(hWnd, 5, 100, NULL);

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

            // 1�� Ÿ�̸� - Player �ִϸ��̼�
        case 1:
            p_anim += P_IMAGE_SIZE;
            if (p_anim == P_IMAGE_SIZE * (p_imageCount - 1))
                p_anim = 0;
            break;

            // 2�� Ÿ�̸� - Monster �ִϸ��̼�
        case 2:
            m_anim += M_IMAGE_SIZE;
            if (m_anim == M_IMAGE_SIZE * (m_imageCount - 1))
                m_anim = 0;
            break;

            // 3�� Ÿ�̸� - Bullet �ִϸ��̼�
        case 3:
            for (int i = 0; i < bullet_count; ++i) {
                b_x[i] += (20 * b_dir[i]);

                // �Ѿ� RECT ���� ����
                b_rect[i].left = b_x[i] + 10;
                b_rect[i].right = b_x[i] + 26;
                b_rect[i].top = b_y[i];
                b_rect[i].bottom = b_y[i] + 16;
            }


            //  �Ѿ� <-> ���� �浹ó��
            for (int i = 0; i < MONSTER_AMOUNT; ++i) {
                for (int j = 0; j < BULLET_AMOUNT; ++j) {
                    if (CollisionHelper(m_rect[i], b_rect[j]))
                    {
                        mb_isCollide = true;
                        mb_collide_x = b_x[i];
                        mb_collide_y = b_y[i];

                        m_x[i] = -100;
                        m_y[i] = -100;
                        b_x[j] = -100;
                        b_y[j] = -100;
                    }
                }
            }

            break;

            // 4�� Ÿ�̸� - Monster ������
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

                // ���� RECT ���� ����
                m_rect[i].left = m_x[i];
                m_rect[i].right = m_x[i] + 44;
                m_rect[i].top = m_y[i];
                m_rect[i].bottom = m_y[i] + 42;
            }
            break;
            // 5�� Ÿ�̸� - ���ȭ�� Ⱦ ��ũ��
        case 5:
            scroll_x += 3;

            if (scroll_x > Window_Size_X)
                scroll_x = 0;
            break;
        }

        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_CHAR:
        switch (wParam)
        {
        case 'a':
        case 'A':
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
        case VK_UP:
            p_y -= 3;

            p_state = PLAYER::MOVE;
            break;
        case VK_DOWN:
            p_y += 3;

            p_state = PLAYER::MOVE;
            break;
        case VK_LEFT:
            p_anim = 0; // p_anim�� �ʱ�ȭ ������ ������ ��,�� ���� �� �ִϸ��̼��� ��߳��� ��
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
            is_bullet = true;

            // �����̽� ������ ���� �÷��̾ ��� ������ �ٶ󺸴��� �Ǵ��ϰ� �÷��̾�� �� �� �ռ� ��ġ�� �Ѿ��� �׸���
            if (p_dir == P_DIR_LEFT) {
                b_x[bullet_count] = p_x - 15;
                b_dir[bullet_count] = -1;
            }

            if (p_dir == P_DIR_RIGHT) {
                b_x[bullet_count] = p_x + 15;
                b_dir[bullet_count] = 1;

            }
            b_y[bullet_count] = p_y + 15;

            // �Ѿ��� �ִ� 20��
            bullet_count++;

            if (bullet_count >= BULLET_AMOUNT)
                bullet_count = 0;

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

        // 1. ��� �׸���
        bg.Draw(memdc1, -Window_Size_X + scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
        bg.Draw(memdc1, scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);


        // 2. ���� �׸���
        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                if (Check[i][j] == WALL)
                {
                    wall.Draw(memdc1, Board[i][j].left, Board[i][j].top, wall_width, wall_height);
                }
            }
        }

        // 3. ���� �׸���
        switch (m_state)
        {
        case MONSTER::IDLE:
            m_imageCount = 11;
            monster = monster_idle;
            monster.Draw(memdc1, m_x[0], m_y[0], 44, 42, m_anim, 0, 44, 42);
            monster.Draw(memdc1, m_x[1], m_y[1], 44, 42, m_anim, 0, 44, 42);
            monster.Draw(memdc1, m_x[2], m_y[2], 44, 42, m_anim, 0, 44, 42);
            break;
        case MONSTER::DEAD:
            //monster = monster_idle;
            //monster.Draw(memdc1, m_x[0], m_y[0], 44, 42, m_anim, 0, 44, 42);           
            //monster.Draw(memdc1, m_x[1], m_y[1], 44, 42, m_anim, 0, 44, 42);
            //monster.Draw(memdc1, m_x[2], m_y[2], 44, 42, m_anim, 0, 44, 42);
            break;
        }

        // 4. �÷��̾� �׸���
        switch (p_state)
        {
        case PLAYER::IDLE:
            p_imageCount = 11;
            player = frog_idle;
            player.Draw(memdc1, p_x, p_y, 32, 32, p_anim, p_dir, 32, 32);

            // �÷��̾� RECT ���� ����
            p_rect.left = p_x;
            p_rect.right = p_x + 32;
            p_rect.top = p_y;
            p_rect.bottom = p_y + 32;
            break;

        case PLAYER::MOVE:
            p_imageCount = 12;
            player = frog_move;
            player.Draw(memdc1, p_x, p_y, 32, 32, p_anim, p_dir, 32, 32);
            
            // �÷��̾� RECT ���� ����
            p_rect.left = p_x;
            p_rect.right = p_x + 32;
            p_rect.top = p_y;
            p_rect.bottom = p_y + 32;
            break;
            
        case PLAYER::JUMP:
            player = frog_jump;
            player.Draw(memdc1, p_x, p_y, 32, 32, 0, p_dir, 32, 32);
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

        // 5. �Ѿ� �׸���
        if (is_bullet)
        {
            for (int i = 0; i < bullet_count; ++i) {
                bullet.Draw(memdc1, b_x[i] + 10, b_y[i], 16, 16, 0, 0, 16, 16);
            }
        }

        // 6. �浹�� ������ �ؾ��ϴ� �� - effect ��� ���Դ� ������� �ϱ�

        if (mb_isCollide) 
        {
            effect.Draw(memdc1, mb_collide_x, mb_collide_y, 65, 65, 0, 0, 65, 65);
            mb_isCollide = false;
        }



        //// RECT �׽�Ʈ

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
    bg.Load(L"background.png");
    bg_width = bg.GetWidth();
    bg_height = bg.GetHeight();

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