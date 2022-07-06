#ifndef POO_H
#define POO_H

#include "uart.h"
#include "mbed.h"
#include "C12832.h"
#include "pitches.h"
#include "lcd.h"
#include "led.h"
#include "speaker.h"
#include "common_func.h"

#define WIDTH 40
#define HEIGHT 20
#define TRUE 1
#define FALSE 0
#define ASCII_ESC 27
#define OFFSET 0

// 피할 것
typedef struct
{
    int x;
    int y;
    int act;
    
}Poo;

// 플레이어
typedef struct
{
    int x;
}Player;

// 초기값
void poo_init();

// 피할 적들 처리
// 적 생성
void CreatePoo();

// 적 이동
void MovePoo();

// 피하기 성공한 적 삭제
void DeletePoo();

// 플레이어가 적에게 닿을 경우 (패배)
int PlayerDeath();

// 플레이어 이동 (좌/우)
void MovePlayer();

// 게임 화면 출력
void PrintGame();

void poo_start();

// Game threads
void run_poo_thread();

#endif