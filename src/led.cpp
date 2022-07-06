# include "led.h"

PwmOut led_r(D5);   // led_r = (Red LED)
PwmOut led_g(D9);   // led_r = (Red LED)

void green_led() { // Game 중인 표시
    led_r = 1;
    led_g = 0;   
}


void red_led() { // Game 대기 및 끝난 표시
    led_r = 0;
    led_g = 1;
}

void popping_led() {
    for (int i=0 ; i<10; i++) {
        red_led(); thread_sleep_for(50);
        green_led(); thread_sleep_for(50);   
    }
    red_led();
}
