#include "common_func.h"

Mutex mutex;


void end(int tic) { //엔딩 화면
    putclear();
    int a = 10;
    
    mutex.lock();
    putgoto(a, a);       printf(" #####      ##    ##   ##  #######            #####    ##  ##  #######   ######  \n");
    putgoto(a, a + 1);   printf("##   ##    ####   ### ###   ##  ##           ##   ##   ##  ##   ##  ##   ##  ##  \n");
    putgoto(a, a + 2);   printf("##        ##  ##  #######   ##               ##   ##   ##  ##   ##       ##  ##  \n");
    putgoto(a, a + 3);   printf("##        ######  ## # ##   ####             ##   ##   ##  ##   ####     #####   \n");
    putgoto(a, a + 4);   printf("##  ###   ##  ##  ##   ##   ##               ##   ##   ##  ##   ##       ####    \n");
    putgoto(a, a + 5);   printf("##   ##   ##  ##  ##   ##   ##  ##           ##   ##    ####    ##  ##   ## ##   \n");
    putgoto(a, a + 6);   printf(" #####    ##  ##  ##   ##  #######            #####      ##    #######   ###  ## \n");
    putgoto(a, a + 12);   printf("                            Score = %d. Cheer up! \r\n", tic / 5 * 10);
    mutex.unlock();
}