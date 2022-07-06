#ifndef DINO_H
#define DINO_H

#include "uart.h"
#include "mbed.h"
#include "C12832.h"
#include "lcd.h"
#include "speaker.h"
#include "led.h"
#include "common_func.h"

#define DINO_X 0
#define DINO_Y 15
#define TREE_X 94
#define TREE_Y 25

// 게임 관리
void init_parameter();
void dino_update_tic();
void run_dino_thread();
int game(int tic);
void games(int tic);
void dino_start() ;
void score(int tic);
void end(int tic) ;

// 게임 policy
void jump();
int crashing();

// 그리기 관련
void draw_dino(int tic) ; //공룡
void earase_foot() ;
void earase_dino();

void draw_tree() ;  // 나무
void erase_tree() ;
void tree() ;


#endif
