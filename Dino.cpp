#include "Dino.h"

Mutex dino_mutex;

DigitalIn i_center(D4);
DigitalIn i_up(A2);

int treeX;
int dinoX; 
int dinoY; 

int dino_tic;    //게임 내 시간 단위이자 점수 결정 요소
int crash;  //충돌 체크
int jump_difficulty; //점프 속도와 관련
int tree_difficulty; //나무 오는 속도와 관련

void dino_score(int tic) { //점수 출력
    dino_mutex.lock();
    putgoto(45, 1);
    printf("score : %4d", tic / 5 * 10); // tic/5 당 10점     //필드 폭 4칸 확보 (n천점까지)
    dino_mutex.unlock();
}

void dino_update_tic() {
    while(1) {
        dino_tic++;
        dino_score(dino_tic);
        thread_sleep_for(20);
    }
}

void init_parameter() {
    treeX = TREE_X;
    dinoX = DINO_X;
    dinoY = DINO_Y;
    dino_tic = 0;  
    crash = 0;
    jump_difficulty = 1; 
    tree_difficulty = 2; 
}


void run_dino_thread() {
    putclear();
    init_parameter(); 
    start();
    green_led();
    start_speaker();
    
    while (1) {
        dino_tic = game(dino_tic);

        //joystick 위로 올리면 점프
        if (i_up) {
            jump();
        }
        crash = crashing();
        if (crash == -1) //충돌 시 탈출
            break;
    }
    popping_led();
    dino_end_lcd(); 
    end(dino_tic);
    thread_sleep_for(1000);
}

int game(int dino_tic) { //게임화면 메인 요소
    dino_score(dino_tic);
    tree();
    draw_dino(dino_tic);

    thread_sleep_for(20); //0.02초      //딜레이 예상
    dino_tic ++;
    return dino_tic;
}

void games(int dino_tic) {
    tree();
    draw_dino(dino_tic);
}

void jump() {
    if (dinoY - 15 == 0) { //'스페이스 바'가 눌리고 공룡이 바닥에 있을 때
        int h = 0; //점프 높이 초기화
        while (h < 8) { //y축으로 8칸 상승
            earase_dino();
            dinoY-=jump_difficulty;
            crash = crashing();
            if (crash == -1) {
                break;
            }
            h+=jump_difficulty;
            games(dino_tic);
        }

        while (h > 0) { //y축으로 8칸 하강
            crash = crashing();
            if (crash == -1)
                break;
            dinoY+=jump_difficulty;
            earase_dino();
            h-=jump_difficulty;
            games(dino_tic);
        }
    }
}

int crashing() { //충돌 판정    //난이도 '하'
    // (가로1 && 가로2) && 세로
    // 가로1: 나무가 가로 11칸보다 뒤에 있음
    // 가로2: 나무가 가로 15칸보다 앞에 있음
    // 가로1 && 가로2: 나무가 가로 11칸과 15칸 사이에 있음
    // 세로: 공룡 발 3칸이 나무 5칸 보다 높이가 같거나 낮을 때
    if ((dinoX + 6 <= treeX + 2 && dinoX + 10 >= treeX + 2) && dinoY + 12 >= TREE_Y + 2) {
        stop_speaker();
        return -1;
    } else
        return 0;
}

void draw_dino(int dino_tic) { //공룡 그리기
    int toc = dino_tic % 8;
    dino_mutex.lock();
    //몸통

    putgoto(dinoX, dinoY);           
    printf("              @@@@@@@@\r\n");
    printf("             @@@@@@@@@@@\r\n");
    printf("             @@@ @@@@@@@\r\n");
    printf("             @@@@@@@@@@@\r\n");
    printf("             @@@@@@\r\n");
    printf("     *      @@@@@@@@@@\r\n");
    printf("     @     @@@@@@\r\n");
    printf("     @@  @@@@@@@@@@@@\r\n");
    printf("     @@@@@@@@@@@@   @\r\n");
    printf("     @@@@@@@@@@@@\r\n");
    printf("      @@@@@@@@@@\r\n");
    printf("       @@@@@@@@\r\n");
    printf("         @@@@@@\r\n");  
    

    // 발 구르기 구현
    if (toc >= 0 && toc <= 3) //4tic 동안 유지
    {
        //earase_foot();
        putgoto(dinoX, dinoY + 13); //발 그리기
        printf("         @    @@@\r\n");
        printf("         @@      ");
    }
    else
    {
        //earase_foot();
        putgoto(dinoX, dinoY + 13); //발 그리기
        printf("         @@@  @  \r\n");
        printf("              @@");
    }
    dino_mutex.unlock();
}

void earase_foot() { //발 지우기
    dino_mutex.lock();
    putgoto(dinoX, dinoY + 13);
    printf("                 \n");
    printf("                 ");
    dino_mutex.unlock();
}

void earase_dino() { //공룡 지우기
    dino_mutex.lock();
    putclear();
    dino_score(dino_tic);
    dino_mutex.unlock();
    earase_foot();
}

void draw_tree() { //나무 그리기
    dino_mutex.lock();
    putgoto(treeX + 2, TREE_Y);          printf("##\r\n");
    putgoto(treeX, TREE_Y + 1);        printf("# ## #\r\n");
    putgoto(treeX, TREE_Y + 2);        printf("######\r\n");
    putgoto(treeX + 2, TREE_Y + 3);      printf("##\r\n");
    putgoto(treeX + 2, TREE_Y + 4);      printf("##");
    dino_mutex.unlock();
}

void erase_tree() { //나무 지우기
    dino_mutex.lock();
    putgoto(treeX + 3, TREE_Y);        printf("    \r\n");
    putgoto(treeX + 1, TREE_Y + 1);  printf("        \r\n");
    putgoto(treeX + 1, TREE_Y + 2);  printf("        \r\n");
    putgoto(treeX + 3, TREE_Y + 3);    printf("    \r\n");
    putgoto(treeX + 3, TREE_Y + 4);    printf("    ");
    dino_mutex.unlock();
}

void tree() { //나무 오른쪽에서 왼쪽으로 이동
    treeX-=tree_difficulty; //왼쪽으로 한 칸 이동
    erase_tree(); //지우고
    draw_tree(); //그리기

    if (treeX == 0)
        treeX = TREE_X; //나무가 왼쪽 끝으로 이동하면 초기화
}

void dino_start() { //시작 화면
    while (1) { 
        dino_mutex.lock();
        putgoto(30, 10);
        printf("Press 'Center Button' to start the game");
        dino_mutex.unlock();
        draw_dino(0);

        if (i_center) {
            dino_setting_lcd();
            dino_start_lcd();
            putclear();
            break;
        }
    }
}
