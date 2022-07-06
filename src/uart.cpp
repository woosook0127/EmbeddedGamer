#include "uart.h"

// 지정된 위치로 커서를 이동시키는 함수
void putgoto(int x, int y)
{
    printf("%c[%d;%dH", ASCII_ESC, y, x);
}

// screen clear
void putclear(void)
{
    printf("%c[2J%c[0;0H", ASCII_ESC, ASCII_ESC);
}
