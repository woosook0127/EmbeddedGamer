#include "poo.h"

Mutex poo_mutex;

DigitalIn i_left(A4); // left = A4
DigitalIn i_rite(A5); // rite = A5

Poo poo[WIDTH];
Player player;

int poo_tic; 

void poo_score(int tic) { //점수 출력
    poo_mutex.lock();
    putgoto(45, 1);
    printf("score : %4d", tic / 5 * 10); // tic/5 당 10점     //필드 폭 4칸 확보 (n천점까지)
    poo_mutex.unlock();
}

void poo_update_tic() {
    poo_tic++;
    thread_sleep_for(50);
}

void poo_init()
{
    poo_setting_lcd();
    poo_tic = 0;
    green_led();
    for (int i = 0; i < WIDTH; i++)
        poo[i].act = FALSE;

    player.x = WIDTH / 2;
} 

void CreatePoo()
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (!poo[i].act)
        {
            poo[i].x = rand() % WIDTH;
            poo[i].y = HEIGHT - 1;

            poo[i].act = TRUE;

            return;
        }
    }
}

void MovePoo()
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (poo[i].act)
        {
            poo[i].y--;
        }
    }
}

void DeletePoo()
{
    for (int i = 0; i < WIDTH; i++)
    {
        if (poo[i].act && poo[i].y < 0)
        {
            poo[i].act = FALSE;
        }
    }
}

int PlayerDeath()
{
    for (int i = 0; i < WIDTH; i++)
    {
        if ((poo[i].act) && (poo[i].y == 0) && (poo[i].x == player.x))
        {
            stop_speaker();
            return TRUE;
        }
    }
    return FALSE;
}

void MovePlayer(){
    // 좌우 방향키 입력 시
    if (i_left)
        player.x--;
    if (i_rite)
        player.x++;

    // 위치 범위 제한
    if (player.x < 0)
        player.x = 0;
    if (player.x > WIDTH - 1)
        player.x = WIDTH - 1;
}

void PrintGame() {
    // 모든 화면 clear
    putclear();
    poo_score(poo_tic);
    // 적 위치에 적 출력
    poo_mutex.lock();
    for (int i = 0; i < WIDTH; i++) {
        if (poo[i].act) {
            putgoto(poo[i].x + OFFSET, OFFSET + HEIGHT - poo[i].y);
            printf("O");
        }
    }
    poo_mutex.unlock();
    
    // 플레이어 출력, 바닥 출력
    poo_mutex.lock();
    putgoto(player.x + OFFSET, HEIGHT + OFFSET);
    printf("o");
    putgoto(OFFSET, OFFSET + HEIGHT + 1);
    printf("----------------------------------------");
    poo_mutex.unlock();
}

void run_poo_thread() {
    putclear();
    poo_start();
    poo_init();
    
    start_speaker();
    while (!(PlayerDeath())) {
        // 매번 실행할 때마다 다른 값을 주기 위한 시드값
        srand((int)malloc(NULL));
        CreatePoo();
        MovePoo();
        DeletePoo();
        MovePlayer();
        PrintGame();
        
        poo_update_tic(); 
    }
    
    end(poo_tic);
    poo_end_lcd();
    popping_led();
    thread_sleep_for(1000);
}

void poo_start() { //시작 화면
    while (1) { 
        poo_mutex.lock();
        putgoto(30, 10);
        printf("Press 'Left or Right Button' to start the game");
        poo_mutex.unlock();

        if (i_left||i_rite) {
            poo_setting_lcd();
            poo_start_lcd();
            putclear();
            break;
        }
    }
}

