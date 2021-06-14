#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <atlimage.h>
#include <string.h>

#include "Image.h"
#include "Map.h"
#include "State.h"
#include "Define.h"
#include "Obstacle.h"


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

// 시간을 구하기 위한 변수들
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;


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
void GaroObstacle();
void SeroObstacle();
void InitMonster(int m_x[MONSTER_AMOUNT], int m_y[MONSTER_AMOUNT]);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hdc, memdc1, memdc2;
    HBITMAP hBitmap1, hBitmap2;
    HBRUSH hBrush, oldBrush;

    // Player 위치, 애니메이션, 크기, 칸수 등등
    static float p_x, p_y;
    static int p_anim;
    static int p_dir;
    static int p_imageCount;
    static float p_speed = 300.0f;
    static float temp_p_y;

    // Monster 위치, 애니메이션, 크기, 칸수 등등
    static int m_x[MONSTER_AMOUNT];
    static int temp_m_x[MONSTER_AMOUNT];
    static int m_y[MONSTER_AMOUNT];
    static int m_anim;
    static int m_imageCount;
    static int m_dir[MONSTER_AMOUNT] = { -1,-1,-1 };

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
    static RECT w_rect[820];

    static int w_rect_count = 0;

    static RECT obs_g_rect[OBS_GARO_COUNT];
    static RECT obs_s_rect[OBS_SERO_COUNT];

    // 충돌 시점
    static bool mp_isCollide;
    static int mp_collide_x;
    static int mp_collide_y;

    static bool p_isCollide;
    static int p_collide_x;
    static int p_collide_y;

    // BackGround for Scroll 
    static int scroll_x;

    // 물리
    static float dx, dy;
    static float accX, accY;
    static float velX, velY;
    static float  PLAYER_ACC = 1.f;
    static float  PLAYER_FRICTION = -0.2f;
    static float  PLAYER_GRAVITY = 0.5f;

    static bool isRanding = false;
    static bool isJump = false;
    static int jumpCount = 0;

    // Heart
    static int heart_count = 5;

    // 게임 Ending 관련
    static int dest_x = 10300;
    static int dest_y = 600;
    static int is_game_fail = false;
    static int is_game_success = false;


    switch (uMsg) {

    case WM_CREATE:

        LoadImage();

        //p_x = 230;
        //p_y = 650;
        p_x = 9900;
        p_y = 550;

        InitMonster(m_x, m_y);

        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            temp_m_x[i] = m_x[i];
        }

        player = frog_idle;

        p_state = PLAYER::IDLE;
        m_state = MONSTER::IDLE;


        SetTimer(hWnd, 1, 16, NULL);


        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                Board[i][j].left = 0 + j * BLOCK_SIZE;
                Board[i][j].right = 50 + j * BLOCK_SIZE;
                Board[i][j].top = 0 + i * BLOCK_SIZE;
                Board[i][j].bottom = 50 + i * BLOCK_SIZE;
            }
        }

        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                if (Map[i][j] == WALL)
                {
                    w_rect[w_rect_count].left = Board[i][j].left;
                    w_rect[w_rect_count].right = Board[i][j].right;
                    w_rect[w_rect_count].top = Board[i][j].top;
                    w_rect[w_rect_count].bottom = Board[i][j].bottom;

                    w_rect_count++;
                }
            }
        }

        InitObstacle();


        break;

    case WM_TIMER:
        switch (wParam) {

        case 1:

            accY = PLAYER_GRAVITY;
            accX = dx;
            velY += accY;

            // 0번 기능 - 키보드 입력에 따른 기능
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                accY = PLAYER_GRAVITY;
                p_state = PLAYER::MOVE;
                p_dir = P_DIR_LEFT;
                p_x -= (p_speed * 0.016f);
            }

            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                accY = PLAYER_GRAVITY;
                p_state = PLAYER::MOVE;
                p_dir = P_DIR_RIGHT;
                p_x += (p_speed * 0.016f);
            }

            if (!isJump) {
                for (int i = 0; i < w_rect_count; ++i) {
                    if (CollisionHelper(w_rect[i], p_rect)) {
                        //위 충돌

                        if (w_rect[i].bottom < p_rect.bottom)
                        {
                            p_y = w_rect[i].bottom + 32;
                            accY = 0;
                            velY = 0;

                        }
                        //아래 충돌
                        else if (w_rect[i].top > p_rect.top)
                        {

                            //velY = -100;
                            if (isRanding) {
                                p_y = w_rect[i].top - 32;
                                velY = 0;
                                accY = 0;

                            }
                            else {
                                p_y -= 1;
                            }
                            isRanding = true;
                            jumpCount = 0;
                            //p_locate = PLAYER::GROUND;
                        }
                        // 왼쪽 충돌

                        else if (w_rect[i].right > p_rect.right )
                        {
                            p_x -= 10;
                            
                        }
                        // 오른쪽 충돌
                        else if (w_rect[i].left < p_rect.left )
                        {
                            p_x += 10;

                        }

                    }
                }
            }

            isJump = false;
            p_y += velY + accY * 0.016f;

            // 떨어지면 처음부터 - Fall            
            if (p_y > 1000) {
                p_x = 230;
                p_y = 650;
            }

            // 마지막에 도달하면 - End
            if (10300 - 52 < p_x && p_x < 10352
                && 600 - 52 < p_y && p_y < 600 + 52)
            {
                is_game_success = true;
            }




            // 1번 기능 - Player 애니메이션
            p_anim += P_IMAGE_SIZE;
            if (p_anim >= P_IMAGE_SIZE * (p_imageCount - 1))
                p_anim = 0;

            // 2번 기능 - Monster 애니메이션
            m_anim += M_IMAGE_SIZE;
            if (m_anim == M_IMAGE_SIZE * (m_imageCount - 1))
                m_anim = 0;

            // 3번 기능 - Bullet 애니메이션
            for (int i = 0; i < bullet_count; ++i) {
                b_x[i] += (20 * b_dir[i]);

                // 총알 RECT 범위 설정
                b_rect[i].left = b_x[i] + 10;
                b_rect[i].right = b_x[i] + 26;
                b_rect[i].top = b_y[i];
                b_rect[i].bottom = b_y[i] + 16;
            }

            // 4번 기능 - Monster 움직임
            for (int i = 0; i < MONSTER_AMOUNT; ++i) {

                m_x[i] += 5 * m_dir[i];

                if (temp_m_x[i] - m_x[i] > 100) {
                    m_dir[i] = 1;
                }
                else if (m_x[i] - temp_m_x[i] > 100) {
                    m_dir[i] = -1;
                }

                // 몬스터 RECT 범위 설정
                m_rect[i].left = m_x[i];
                m_rect[i].right = m_x[i] + 44;
                m_rect[i].top = m_y[i];
                m_rect[i].bottom = m_y[i] + 42;

                // 플레이어 <-> 몬스터 충돌처리
                if (CollisionHelper(m_rect[i], p_rect))
                {
                    mp_isCollide = true;
                    mp_collide_x = m_x[i];
                    mp_collide_y = m_y[i];

                    is_game_fail = true;                    
                }


                // 플레이어 총알 <-> 몬스터 충돌처리
                for (int j = 0; j < BULLET_AMOUNT; ++j) {
                    if (CollisionHelper(b_rect[j], m_rect[i]))
                    {
                        mp_isCollide = true;
                        mp_collide_x = m_x[i];
                        mp_collide_y = m_y[i];

                        m_x[i] = -100;
                        m_y[i] = -100;
                        b_x[j] = -300;
                        b_y[j] = -300;
                    }
                }
            }

            // 5번 기능 - 배경화면 횡 스크롤
            scroll_x += 5;

            if (scroll_x > Window_Size_X)
                scroll_x = 0;

            //6번 기능 - 가로 장애물
            GaroObstacle();

            for (int i = 0; i < OBS_GARO_COUNT; ++i) {
                obs_g_rect[i].left = obs_garo[i].pos_x;
                obs_g_rect[i].right = obs_garo[i].pos_x + 50;
                obs_g_rect[i].top = obs_garo[i].pos_y + obs_garo[i].rand_num;
                obs_g_rect[i].bottom = obs_garo[i].pos_y + obs_garo[i].rand_num + 50;
                if (CollisionHelper(obs_g_rect[i], p_rect)) {
                    p_isCollide = true;
                    p_collide_x = p_x;
                    p_collide_y = p_y;

                    heart_count--;
                    if (heart_count == 0)
                        is_game_fail = true;

                    obs_garo[i].pos_x = -100;
                    obs_garo[i].pos_y = -100;
                }
            }

            //7번 기능 - 세로 장애물
            SeroObstacle();

            for (int i = 0; i < OBS_SERO_COUNT; ++i) {
                obs_s_rect[i].left = obs_sero[i].pos_x + obs_sero[i].rand_num;
                obs_s_rect[i].right = obs_sero[i].pos_x + obs_sero[i].rand_num + 50;
                obs_s_rect[i].top = obs_sero[i].pos_y;
                obs_s_rect[i].bottom = obs_sero[i].pos_y + 50;
                if (CollisionHelper(obs_s_rect[i], p_rect)) {
                    p_isCollide = true;
                    p_collide_x = p_x;
                    p_collide_y = p_y;

                    heart_count--;
                    if (heart_count == 0) 
                        is_game_fail = true;
                    

                    obs_sero[i].pos_x = -100;
                    obs_sero[i].pos_y = -100;
                }
            }
            break;
        }
        

        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_CHAR:
        switch (wParam)
        {
        case 'a':
        case 'A':
            p_anim = 0;
            p_state = PLAYER::ATTACK;

            is_bullet = true;

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

            // 총알은 최대 20발
            bullet_count++;

            if (bullet_count >= BULLET_AMOUNT)
                bullet_count = 0;

            break;
        case 'z':
        case 'Z':
            velY = 7.5f;
            break;
        case 'r':
        case 'R':
            p_x = 230;
            p_y = 650;
            heart_count = 3;
            is_game_fail = false;
            is_game_success = false;
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
        case VK_SPACE:
            if (jumpCount < 2) {
                isJump = true;
                isRanding = false;
                velY = -11.5f;
                jumpCount++;
                break;
            }
        case VK_SHIFT:
            p_speed = 600.f;
            break;
        }
        break;
    case WM_KEYUP:
        p_state = PLAYER::IDLE;
        p_speed = 300.f;
        break;

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);

        memdc1 = CreateCompatibleDC(hdc);
        hBitmap1 = CreateCompatibleBitmap(hdc, 11500, Window_Size_Y);
        SelectObject(memdc1, hBitmap1);

        // 1. 배경 그리기
        bg.Draw(memdc1, -Window_Size_X + scroll_x + p_x - 200, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
        bg.Draw(memdc1, scroll_x + p_x - 200, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);


        // 2. 발판 그리기
        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                if (Map[i][j] == WALL)
                {
                    wall.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 50);
                }

                if (Map[i][j] == MAGMA)
                {
                    magma.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 50);
                }
            }
        }

        // 3. 몬스터 그리기
        switch (m_state)
        {
        case MONSTER::IDLE:
            m_imageCount = 11;
            monster = monster_idle;
            for (int i = 0; i < MONSTER_AMOUNT; ++i) {
                monster.Draw(memdc1, m_x[i], m_y[i], 44, 42, m_anim, 0, 44, 42);
            }

            break;
        case MONSTER::DEAD:
            break;
        }

        // 4. 플레이어 그리기
        switch (p_state)
        {
        case PLAYER::IDLE:
            p_imageCount = 11;
            player = frog_idle;
            player.Draw(memdc1, p_x, p_y, 32, 32, p_anim, p_dir, 32, 32);

            // 플레이어 RECT 범위 설정
            p_rect.left = p_x + 5;
            p_rect.right = p_x + 27;
            p_rect.top = p_y;
            p_rect.bottom = p_y + 32;
            break;

        case PLAYER::MOVE:
            p_imageCount = 12;
            player = frog_move;
            player.Draw(memdc1, p_x, p_y, 32, 32, p_anim, p_dir, 32, 32);

            // 플레이어 RECT 범위 설정
            p_rect.left = p_x + 5;
            p_rect.right = p_x + 27;
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

        // 5. 총알 그리기
        if (is_bullet)
        {
            for (int i = 0; i < bullet_count; ++i) {
                bullet.Draw(memdc1, b_x[i] + 10, b_y[i], 16, 16, 0, 0, 16, 16);
            }
        }

        // 6, 가로 장애물 그리기
        for (int i = 0; i < OBS_GARO_COUNT; ++i) {
            garo.Draw(memdc1, obs_garo[i].pos_x, obs_garo[i].pos_y + obs_garo[i].rand_num, 50, 50, 0, 0, 50, 50);
        }


        // 7. 세로 장애물 그리기
        for (int i = 0; i < OBS_SERO_COUNT; ++i) {
            sero.Draw(memdc1, obs_sero[i].pos_x + obs_sero[i].rand_num, obs_sero[i].pos_y, 50, 50, 0, 0, 50, 50);
        }

        // 8. 충돌된 시점에 해야하는 일 - effect         
        if (mp_isCollide)
        {
            effect.Draw(memdc1, mp_collide_x, mp_collide_y, 65, 65, 0, 0, 65, 65);
            mp_isCollide = false;
        }

        if (p_isCollide)
        {
            effect.Draw(memdc1, p_collide_x, p_collide_y, 65, 65, 0, 0, 65, 65);
            p_isCollide = false;
        }

        // 9. Heart 그리기
        for (int i = 0; i < heart_count; ++i) {
            My_heart.Draw(memdc1, 30 + 45 * i + p_x - 200, 20, 45, 45, 0, 0, 45, 45);
        }

        // 10. 최종 Portal 그리기
        portal.Draw(memdc1, 10300, 600, 52, 52, 0, 0, 52, 52);

        if (is_game_fail)
        {
            FailGame.Draw(memdc1, p_x - 200, 0, 1000, 1000, 0, 0, 1000, 1000);
        }

        if (is_game_success)
        {
            SuccessGame.Draw(memdc1, p_x - 200, 0, 1000, 1000, 0, 0, 1000, 1000);
        }

        // --- RECT 테스트 ---

        //for (int i = 0; i < bullet_count; ++i) {
        //    Rectangle(memdc1, b_rect[i].left, b_rect[i].top, b_rect[i].right, b_rect[i].bottom);
        //}

        //for (int i = 0; i < MONSTER_AMOUNT; ++i) {
        //    Rectangle(memdc1, m_rect[i].left, m_rect[i].top, m_rect[i].right, m_rect[i].bottom);
        //}

        //for (int i = 0; i < OBS_SERO_COUNT; ++i) {
        //    Rectangle(memdc1, obs_s_rect[i].left, obs_s_rect[i].top, obs_s_rect[i].right, obs_s_rect[i].bottom);
        //}

        //for (int i = 0; i < OBS_GARO_COUNT; ++i) {
        //    Rectangle(memdc1, obs_g_rect[i].left, obs_g_rect[i].top, obs_g_rect[i].right, obs_g_rect[i].bottom);
        //}

        //Rectangle(memdc1, p_rect.left, p_rect.top, p_rect.right, p_rect.bottom);

        //for (int i = 0; i < w_rect_count; ++i) {            
        //    Rectangle(memdc1, w_rect[i].left, w_rect[i].top, w_rect[i].right, w_rect[i].bottom);
        //}

        // --- RECT 테스트 ---


        BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, p_x - 200, 0, SRCCOPY);

        DeleteObject(SelectObject(memdc1, hBitmap1));
        DeleteDC(memdc1);
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        KillTimer(hWnd, 1);
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

    monster_idle.Load(L"monster_idle.png");
    monster_dead.Load(L"monster_dead.png");

    frog_idle.Load(L"frog_idle.png");
    frog_move.Load(L"frog_move.png");
    frog_jump.Load(L"frog_jump.png");
    frog_fall.Load(L"frog_fall.png");
    frog_attack.Load(L"frog_attack.png");

    bullet.Load(L"bullet.png");

    effect.Load(L"effect.png");

    magma.Load(L"magma.jpg");

    garo.Load(L"garo.png");
    sero.Load(L"sero.png");

    My_heart.Load(L"heart.png");

    portal.Load(L"portal.png");

    FailGame.Load(L"FailGame.png");
    SuccessGame.Load(L"SuccessGame.png");
}

bool CollisionHelper(RECT r1, RECT r2)
{
    if (r1.left > r2.right) return false;
    if (r1.top > r2.bottom) return false;
    if (r1.right < r2.left) return false;
    if (r1.bottom < r2.top) return false;

    return true;
}

void GaroObstacle()
{
    obs_garo[0].pos_x -= 8;
    if (obs_garo[0].pos_x < 0) {
        obs_garo[0].pos_x = 950;
        obs_garo[0].rand_num = rand() % 450;
    }

    obs_garo[1].pos_x -= 10;
    if (obs_garo[1].pos_x < 0) {
        obs_garo[1].pos_x = 950;
        obs_garo[1].rand_num = 450 + rand() % 350;
    }

    obs_garo[2].pos_x -= 8;
    if (obs_garo[2].pos_x < 1000) {
        obs_garo[2].pos_x = 1000 + 950;
        obs_garo[2].rand_num = rand() % 450;
    }

    obs_garo[3].pos_x -= 10;
    if (obs_garo[3].pos_x < 1000) {
        obs_garo[3].pos_x = 1000 + 950;
        obs_garo[3].rand_num = 450 + rand() % 350;
    }

    obs_garo[4].pos_x -= 8;
    if (obs_garo[4].pos_x < 2000) {
        obs_garo[4].pos_x = 2000 + 950;
        obs_garo[4].rand_num = rand() % 450;
    }

    obs_garo[5].pos_x -= 10;
    if (obs_garo[5].pos_x < 2000) {
        obs_garo[5].pos_x = 2000 + 950;
        obs_garo[5].rand_num = 450 + rand() % 350;
    }

    obs_garo[6].pos_x -= 8;
    if (obs_garo[6].pos_x < 3000) {
        obs_garo[6].pos_x = 3000 + 950;
        obs_garo[6].rand_num = rand() % 450;
    }

    obs_garo[7].pos_x -= 10;
    if (obs_garo[7].pos_x < 3000) {
        obs_garo[7].pos_x = 3000 + 950;
        obs_garo[7].rand_num = 450 + rand() % 350;
    }

    obs_garo[8].pos_x -= 8;
    if (obs_garo[8].pos_x < 4000) {
        obs_garo[8].pos_x = 4000 + 950;
        obs_garo[8].rand_num = rand() % 450;
    }

    obs_garo[9].pos_x -= 10;
    if (obs_garo[9].pos_x < 4000) {
        obs_garo[9].pos_x = 4000 + 950;
        obs_garo[9].rand_num = 450 + rand() % 350;
    }

    obs_garo[10].pos_x -= 8;
    if (obs_garo[10].pos_x < 5000) {
        obs_garo[10].pos_x = 5000 + 950;
        obs_garo[10].rand_num = rand() % 450;
    }

    obs_garo[11].pos_x -= 10;
    if (obs_garo[11].pos_x < 5000) {
        obs_garo[11].pos_x = 5000 + 950;
        obs_garo[11].rand_num = 450 + rand() % 350;
    }

    obs_garo[12].pos_x -= 8;
    if (obs_garo[12].pos_x < 6000) {
        obs_garo[12].pos_x = 6000 + 950;
        obs_garo[12].rand_num = rand() % 450;
    }

    obs_garo[13].pos_x -= 10;
    if (obs_garo[13].pos_x < 6000) {
        obs_garo[13].pos_x = 6000 + 950;
        obs_garo[13].rand_num = 450 + rand() % 350;
    }

    obs_garo[14].pos_x -= 8;
    if (obs_garo[14].pos_x < 7000) {
        obs_garo[14].pos_x = 7000 + 950;
        obs_garo[14].rand_num = rand() % 450;
    }

    obs_garo[15].pos_x -= 10;
    if (obs_garo[15].pos_x < 7000) {
        obs_garo[15].pos_x = 7000 + 950;
        obs_garo[15].rand_num = 450 + rand() % 350;
    }

    obs_garo[16].pos_x -= 8;
    if (obs_garo[16].pos_x < 8000) {
        obs_garo[16].pos_x = 8000 + 950;
        obs_garo[16].rand_num = rand() % 450;
    }

    obs_garo[17].pos_x -= 10;
    if (obs_garo[17].pos_x < 9000) {
        obs_garo[17].pos_x = 9000 + 950;
        obs_garo[17].rand_num = 450 + rand() % 350;
    }

    obs_garo[18].pos_x -= 8;
    if (obs_garo[18].pos_x < 10000) {
        obs_garo[18].pos_x = 10000 + 950;
        obs_garo[18].rand_num = rand() % 450;
    }

    obs_garo[19].pos_x -= 10;
    if (obs_garo[19].pos_x < 10000) {
        obs_garo[19].pos_x = 10000 + 950;
        obs_garo[19].rand_num = 450 + rand() % 350;
    }
}

void SeroObstacle()
{
    obs_sero[0].pos_x += 5;
    obs_sero[0].pos_y += 5;
    if (obs_sero[0].pos_x > 800 || obs_sero[0].pos_y > 800) {
        obs_sero[0].pos_x = 0;
        obs_sero[0].pos_y = 0;
        obs_sero[0].rand_num = rand() % 400;
    }

    obs_sero[1].pos_x += 3;
    obs_sero[1].pos_y += 3;
    if (obs_sero[1].pos_x > 800 || obs_sero[1].pos_y > 800) {
        obs_sero[1].pos_x = 0;
        obs_sero[1].pos_y = 0;
        obs_sero[1].rand_num = 450 + rand() % 300;
    }

    obs_sero[2].pos_x += 7;
    obs_sero[2].pos_y += 7;
    if (obs_sero[2].pos_x > 800 || obs_sero[2].pos_y > 800) {
        obs_sero[2].pos_x = 0;
        obs_sero[2].pos_y = 0;
        obs_sero[2].rand_num = rand() % 400;
    }

    obs_sero[3].pos_x += 5;
    obs_sero[3].pos_y += 5;
    if (obs_sero[3].pos_x > 1800 || obs_sero[3].pos_y > 800) {
        obs_sero[3].pos_x = 800;
        obs_sero[3].pos_y = 0;
        obs_sero[3].rand_num = rand() % 400;
    }

    obs_sero[4].pos_x += 3;
    obs_sero[4].pos_y += 3;
    if (obs_sero[4].pos_x > 1800 || obs_sero[4].pos_y > 800) {
        obs_sero[4].pos_x = 800;
        obs_sero[4].pos_y = 0;
        obs_sero[4].rand_num = 450 + rand() % 300;
    }

    obs_sero[5].pos_x += 7;
    obs_sero[5].pos_y += 7;
    if (obs_sero[5].pos_x > 1800 || obs_sero[5].pos_y > 800) {
        obs_sero[5].pos_x = 800;
        obs_sero[5].pos_y = 0;
        obs_sero[5].rand_num = rand() % 400;
    }

    obs_sero[6].pos_x += 5;
    obs_sero[6].pos_y += 5;
    if (obs_sero[6].pos_x > 2800 || obs_sero[6].pos_y > 800) {
        obs_sero[6].pos_x = 1600;
        obs_sero[6].pos_y = 0;
        obs_sero[6].rand_num = rand() % 400;
    }

    obs_sero[7].pos_x += 3;
    obs_sero[7].pos_y += 3;
    if (obs_sero[7].pos_x > 2800 || obs_sero[7].pos_y > 800) {
        obs_sero[7].pos_x = 1600;
        obs_sero[7].pos_y = 0;
        obs_sero[7].rand_num = 450 + rand() % 300;
    }

    obs_sero[8].pos_x += 7;
    obs_sero[8].pos_y += 7;
    if (obs_sero[8].pos_x > 3800 || obs_sero[8].pos_y > 800) {
        obs_sero[8].pos_x = 2400;
        obs_sero[8].pos_y = 0;
        obs_sero[8].rand_num = rand() % 400;
    }

    obs_sero[9].pos_x += 5;
    obs_sero[9].pos_y += 5;
    if (obs_sero[9].pos_x > 3800 || obs_sero[9].pos_y > 800) {
        obs_sero[9].pos_x = 2400;
        obs_sero[9].pos_y = 0;
        obs_sero[9].rand_num = rand() % 400;
    }

    obs_sero[10].pos_x += 3;
    obs_sero[10].pos_y += 3;
    if (obs_sero[10].pos_x > 3800 || obs_sero[10].pos_y > 800) {
        obs_sero[10].pos_x = 2400;
        obs_sero[10].pos_y = 0;
        obs_sero[10].rand_num = 450 + rand() % 300;
    }

    obs_sero[11].pos_x += 7;
    obs_sero[11].pos_y += 7;
    if (obs_sero[11].pos_x > 3800 || obs_sero[11].pos_y > 800) {
        obs_sero[11].pos_x = 2400;
        obs_sero[11].pos_y = 0;
        obs_sero[11].rand_num = rand() % 400;
    }

    obs_sero[12].pos_x += 5;
    obs_sero[12].pos_y += 5;
    if (obs_sero[12].pos_x > 4800 || obs_sero[12].pos_y > 800) {
        obs_sero[12].pos_x = 3200;
        obs_sero[12].pos_y = 0;
        obs_sero[12].rand_num = rand() % 400;
    }

    obs_sero[13].pos_x += 3;
    obs_sero[13].pos_y += 3;
    if (obs_sero[13].pos_x > 4800 || obs_sero[13].pos_y > 800) {
        obs_sero[13].pos_x = 3200;
        obs_sero[13].pos_y = 0;
        obs_sero[13].rand_num = 450 + rand() % 300;
    }

    obs_sero[14].pos_x += 7;
    obs_sero[14].pos_y += 7;
    if (obs_sero[14].pos_x > 3800 || obs_sero[14].pos_y > 800) {
        obs_sero[14].pos_x = 3200;
        obs_sero[14].pos_y = 0;
        obs_sero[14].rand_num = rand() % 400;
    }


    obs_sero[15].pos_x += 5;
    obs_sero[15].pos_y += 5;
    if (obs_sero[15].pos_x > 4800 || obs_sero[15].pos_y > 800) {
        obs_sero[15].pos_x = 4000;
        obs_sero[15].pos_y = 0;
        obs_sero[15].rand_num = rand() % 400;
    }

    obs_sero[16].pos_x += 3;
    obs_sero[16].pos_y += 3;
    if (obs_sero[16].pos_x > 4800 || obs_sero[16].pos_y > 800) {
        obs_sero[16].pos_x = 4000;
        obs_sero[16].pos_y = 0;
        obs_sero[16].rand_num = 450 + rand() % 300;
    }

    obs_sero[17].pos_x += 7;
    obs_sero[17].pos_y += 7;
    if (obs_sero[17].pos_x > 4800 || obs_sero[17].pos_y > 800) {
        obs_sero[17].pos_x = 4000;
        obs_sero[17].pos_y = 0;
        obs_sero[17].rand_num = rand() % 400;
    }

    obs_sero[18].pos_x += 5;
    obs_sero[18].pos_y += 5;
    if (obs_sero[18].pos_x > 5800 || obs_sero[18].pos_y > 800) {
        obs_sero[18].pos_x = 4800;
        obs_sero[18].pos_y = 0;
        obs_sero[18].rand_num = rand() % 400;
    }

    obs_sero[19].pos_x += 3;
    obs_sero[19].pos_y += 3;
    if (obs_sero[19].pos_x > 5800 || obs_sero[19].pos_y > 800) {
        obs_sero[19].pos_x = 4800;
        obs_sero[19].pos_y = 0;
        obs_sero[19].rand_num = 450 + rand() % 300;
    }

    obs_sero[20].pos_x += 7;
    obs_sero[20].pos_y += 7;
    if (obs_sero[20].pos_x > 5800 || obs_sero[20].pos_y > 800) {
        obs_sero[20].pos_x = 4800;
        obs_sero[20].pos_y = 0;
        obs_sero[20].rand_num = rand() % 400;
    }



    obs_sero[21].pos_x += 5;
    obs_sero[21].pos_y += 5;
    if (obs_sero[21].pos_x > 6800 || obs_sero[21].pos_y > 800) {
        obs_sero[21].pos_x = 5600;
        obs_sero[21].pos_y = 0;
        obs_sero[21].rand_num = rand() % 400;
    }

    obs_sero[22].pos_x += 3;
    obs_sero[22].pos_y += 3;
    if (obs_sero[22].pos_x > 6800 || obs_sero[22].pos_y > 800) {
        obs_sero[22].pos_x = 5600;
        obs_sero[22].pos_y = 0;
        obs_sero[22].rand_num = 450 + rand() % 300;
    }

    obs_sero[23].pos_x += 7;
    obs_sero[23].pos_y += 7;
    if (obs_sero[23].pos_x > 6800 || obs_sero[23].pos_y > 800) {
        obs_sero[23].pos_x = 5600;
        obs_sero[23].pos_y = 0;
        obs_sero[23].rand_num = rand() % 400;
    }

    obs_sero[24].pos_x += 5;
    obs_sero[24].pos_y += 5;
    if (obs_sero[24].pos_x > 7800 || obs_sero[24].pos_y > 800) {
        obs_sero[24].pos_x = 6400;
        obs_sero[24].pos_y = 0;
        obs_sero[24].rand_num = rand() % 400;
    }

    obs_sero[25].pos_x += 3;
    obs_sero[25].pos_y += 3;
    if (obs_sero[25].pos_x > 7800 || obs_sero[25].pos_y > 800) {
        obs_sero[25].pos_x = 6400;
        obs_sero[25].pos_y = 0;
        obs_sero[25].rand_num = 450 + rand() % 300;
    }

    obs_sero[26].pos_x += 7;
    obs_sero[26].pos_y += 7;
    if (obs_sero[26].pos_x > 7800 || obs_sero[26].pos_y > 800) {
        obs_sero[26].pos_x = 6400;
        obs_sero[26].pos_y = 0;
        obs_sero[26].rand_num = rand() % 400;
    }

    obs_sero[27].pos_x += 5;
    obs_sero[27].pos_y += 5;
    if (obs_sero[27].pos_x > 8800 || obs_sero[27].pos_y > 800) {
        obs_sero[27].pos_x = 7200;
        obs_sero[27].pos_y = 0;
        obs_sero[27].rand_num = rand() % 400;
    }

    obs_sero[28].pos_x += 3;
    obs_sero[28].pos_y += 3;
    if (obs_sero[28].pos_x > 8800 || obs_sero[28].pos_y > 800) {
        obs_sero[28].pos_x = 7200;
        obs_sero[28].pos_y = 0;
        obs_sero[28].rand_num = 450 + rand() % 300;
    }

    obs_sero[29].pos_x += 7;
    obs_sero[29].pos_y += 7;
    if (obs_sero[29].pos_x > 8800 || obs_sero[26].pos_y > 800) {
        obs_sero[29].pos_x = 7200;
        obs_sero[29].pos_y = 0;
        obs_sero[29].rand_num = rand() % 400;
    }

    obs_sero[30].pos_x += 5;
    obs_sero[30].pos_y += 5;
    if (obs_sero[30].pos_x > 9800 || obs_sero[30].pos_y > 800) {
        obs_sero[30].pos_x = 8000;
        obs_sero[30].pos_y = 0;
        obs_sero[30].rand_num = rand() % 400;
    }

    obs_sero[31].pos_x += 3;
    obs_sero[31].pos_y += 3;
    if (obs_sero[31].pos_x > 9800 || obs_sero[31].pos_y > 800) {
        obs_sero[31].pos_x = 8000;
        obs_sero[31].pos_y = 0;
        obs_sero[31].rand_num = 450 + rand() % 300;
    }

    obs_sero[32].pos_x += 7;
    obs_sero[32].pos_y += 7;
    if (obs_sero[32].pos_x > 9800 || obs_sero[32].pos_y > 800) {
        obs_sero[32].pos_x = 8000;
        obs_sero[32].pos_y = 0;
        obs_sero[32].rand_num = rand() % 400;
    }

    obs_sero[33].pos_x += 5;
    obs_sero[33].pos_y += 5;
    if (obs_sero[33].pos_x > 10800 || obs_sero[33].pos_y > 800) {
        obs_sero[33].pos_x = 8800;
        obs_sero[33].pos_y = 0;
        obs_sero[33].rand_num = rand() % 400;
    }

    obs_sero[34].pos_x += 3;
    obs_sero[34].pos_y += 3;
    if (obs_sero[34].pos_x > 10800 || obs_sero[34].pos_y > 800) {
        obs_sero[34].pos_x = 8800;
        obs_sero[34].pos_y = 0;
        obs_sero[34].rand_num = 450 + rand() % 300;
    }

    obs_sero[35].pos_x += 7;
    obs_sero[35].pos_y += 7;
    if (obs_sero[35].pos_x > 10800 || obs_sero[35].pos_y > 800) {
        obs_sero[35].pos_x = 8800;
        obs_sero[35].pos_y = 0;
        obs_sero[35].rand_num = rand() % 400;
    }
}

void InitMonster(int m_x[MONSTER_AMOUNT], int m_y[MONSTER_AMOUNT])
{
    //움직일 수 있는 놈

    m_x[0] = 10 * BLOCK_SIZE;
    m_y[0] = 12 * BLOCK_SIZE;

    m_x[1] = 13 * BLOCK_SIZE;
    m_y[1] = 12 * BLOCK_SIZE;

    m_x[2] = 12 * BLOCK_SIZE;
    m_y[2] = 10 * BLOCK_SIZE;

    m_x[3] = 93 * BLOCK_SIZE;
    m_y[3] = 1 * BLOCK_SIZE;

    m_x[3] = 97 * BLOCK_SIZE;
    m_y[3] = 1 * BLOCK_SIZE;

    m_x[4] = 97 * BLOCK_SIZE;
    m_y[4] = 13 * BLOCK_SIZE;

    m_x[5] = 95 * BLOCK_SIZE;
    m_y[5] = 15 * BLOCK_SIZE;

    m_x[6] = 87 * BLOCK_SIZE;
    m_y[6] = 13 * BLOCK_SIZE;

    m_x[7] = 128 * BLOCK_SIZE;
    m_y[7] = 3 * BLOCK_SIZE;

    m_x[8] = 128 * BLOCK_SIZE;
    m_y[8] = 14 * BLOCK_SIZE;

    m_x[9] = 151 * BLOCK_SIZE;
    m_y[9] = 1 * BLOCK_SIZE;

    m_x[10] = 151 * BLOCK_SIZE;
    m_y[10] = 3 * BLOCK_SIZE;

    m_x[11] = 162 * BLOCK_SIZE;
    m_y[11] = 14 * BLOCK_SIZE;

    m_x[12] = 175 * BLOCK_SIZE;
    m_y[12] = 12 * BLOCK_SIZE;

    m_x[13] = 178 * BLOCK_SIZE;
    m_y[13] = 12 * BLOCK_SIZE;

    m_x[14] = 183 * BLOCK_SIZE;
    m_y[14] = 12 * BLOCK_SIZE;

    m_x[15] = 186 * BLOCK_SIZE;
    m_y[15] = 12 * BLOCK_SIZE;

    m_x[16] = 191 * BLOCK_SIZE;
    m_y[16] = 12 * BLOCK_SIZE;

    m_x[17] = 194 * BLOCK_SIZE;
    m_y[17] = 12 * BLOCK_SIZE;


    // 못 움직이는 놈
    m_x[18] = 28 * BLOCK_SIZE;
    m_y[18] = 5 * BLOCK_SIZE;

    m_x[19] = 24 * BLOCK_SIZE;
    m_y[19] = 14 * BLOCK_SIZE;

    m_x[20] = 37 * BLOCK_SIZE;
    m_y[20] = 10 * BLOCK_SIZE;

    m_x[21] = 38 * BLOCK_SIZE;
    m_y[21] = 4 * BLOCK_SIZE;

    m_x[22] = 43 * BLOCK_SIZE;
    m_y[22] = 4 * BLOCK_SIZE;

    m_x[23] = 47 * BLOCK_SIZE;
    m_y[23] = 4 * BLOCK_SIZE;

    m_x[24] = 52 * BLOCK_SIZE;
    m_y[24] = 4 * BLOCK_SIZE;

    m_x[25] = 58 * BLOCK_SIZE;
    m_y[25] = 2 * BLOCK_SIZE;

    m_x[26] = 68 * BLOCK_SIZE;
    m_y[26] = 2 * BLOCK_SIZE;

    m_x[27] = 77 * BLOCK_SIZE;
    m_y[27] = 0 * BLOCK_SIZE;

    m_x[28] = 79 * BLOCK_SIZE;
    m_y[28] = 1 * BLOCK_SIZE;

    m_x[29] = 83 * BLOCK_SIZE;
    m_y[29] = 1 * BLOCK_SIZE;

    m_x[30] = 88 * BLOCK_SIZE;
    m_y[30] = 1 * BLOCK_SIZE;

    m_x[31] = 42 * BLOCK_SIZE;
    m_y[31] = 8 * BLOCK_SIZE;

    m_x[32] = 48 * BLOCK_SIZE;
    m_y[32] = 8 * BLOCK_SIZE;

    m_x[33] = 50 * BLOCK_SIZE;
    m_y[33] = 8 * BLOCK_SIZE;

    m_x[34] = 53 * BLOCK_SIZE;
    m_y[34] = 8 * BLOCK_SIZE;

    m_x[35] = 60 * BLOCK_SIZE;
    m_y[35] = 8 * BLOCK_SIZE;

    m_x[36] = 72 * BLOCK_SIZE;
    m_y[36] = 8 * BLOCK_SIZE;

    m_x[37] = 79 * BLOCK_SIZE;
    m_y[37] = 9 * BLOCK_SIZE;

    m_x[38] = 83 * BLOCK_SIZE;
    m_y[38] = 9 * BLOCK_SIZE;

    m_x[39] = 90 * BLOCK_SIZE;
    m_y[39] = 11 * BLOCK_SIZE;

    m_x[40] = 55 * BLOCK_SIZE;
    m_y[40] = 13 * BLOCK_SIZE;

    m_x[41] = 56 * BLOCK_SIZE;
    m_y[41] = 12 * BLOCK_SIZE;

    m_x[42] = 59 * BLOCK_SIZE;
    m_y[42] = 12 * BLOCK_SIZE;

    m_x[43] = 61 * BLOCK_SIZE;
    m_y[43] = 12 * BLOCK_SIZE;

    m_x[44] = 63 * BLOCK_SIZE;
    m_y[44] = 12 * BLOCK_SIZE;

    m_x[45] = 70 * BLOCK_SIZE;
    m_y[45] = 11 * BLOCK_SIZE;

    m_x[46] = 73 * BLOCK_SIZE;
    m_y[46] = 10 * BLOCK_SIZE;

    m_x[47] = 79 * BLOCK_SIZE;
    m_y[47] = 13 * BLOCK_SIZE;

    m_x[48] = 80 * BLOCK_SIZE;
    m_y[48] = 13 * BLOCK_SIZE;

    m_x[49] = 81 * BLOCK_SIZE;
    m_y[49] = 13 * BLOCK_SIZE;

    m_x[50] = 104 * BLOCK_SIZE;
    m_y[50] = 8 * BLOCK_SIZE;

    m_x[51] = 110 * BLOCK_SIZE;
    m_y[51] = 11 * BLOCK_SIZE;

    m_x[52] = 112 * BLOCK_SIZE;
    m_y[52] = 10 * BLOCK_SIZE;

    m_x[53] = 116 * BLOCK_SIZE;
    m_y[53] = 10 * BLOCK_SIZE;

    m_x[54] = 116 * BLOCK_SIZE;
    m_y[54] = 7 * BLOCK_SIZE;

    m_x[55] = 121 * BLOCK_SIZE;
    m_y[55] = 6 * BLOCK_SIZE;

    m_x[56] = 121 * BLOCK_SIZE;
    m_y[56] = 12 * BLOCK_SIZE;

    m_x[57] = 136 * BLOCK_SIZE;
    m_y[57] = 4 * BLOCK_SIZE;

    m_x[58] = 136 * BLOCK_SIZE;
    m_y[58] = 8 * BLOCK_SIZE;

    m_x[59] = 136 * BLOCK_SIZE;
    m_y[59] = 12 * BLOCK_SIZE;

    m_x[60] = 138 * BLOCK_SIZE;
    m_y[60] = 10 * BLOCK_SIZE;

    m_x[61] = 140 * BLOCK_SIZE;
    m_y[61] = 9 * BLOCK_SIZE;

    m_x[62] = 143 * BLOCK_SIZE;
    m_y[62] = 16 * BLOCK_SIZE;

    m_x[63] = 145 * BLOCK_SIZE;
    m_y[63] = 1 * BLOCK_SIZE;

    m_x[64] = 156 * BLOCK_SIZE;
    m_y[64] = 14 * BLOCK_SIZE;

    m_x[65] = 168 * BLOCK_SIZE;
    m_y[65] = 12 * BLOCK_SIZE;
}