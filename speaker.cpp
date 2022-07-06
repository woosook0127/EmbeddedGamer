#include "speaker.h"

PwmOut speaker(D6); // speaker = D6
int status=666;

int length = 75;
float frequency[] = {
    NOTE_E6, NOTE_E6, 0, NOTE_E6, 0, NOTE_C6, NOTE_E6, 0, NOTE_G6, 0, 0,
    0, NOTE_G5, 0, 0, 0, NOTE_C6, 0, 0, NOTE_G5, 0, 0, NOTE_E5, 0, 0,
    NOTE_A5, 0, NOTE_B5, 0, NOTE_AS5, NOTE_A5, 0, NOTE_G5, NOTE_E6, NOTE_G6,
    NOTE_A6, 0, NOTE_F6, NOTE_G6, 0, NOTE_E6, 0, NOTE_C6, NOTE_D6, NOTE_B5,
    0, 0, NOTE_C6, 0, 0, NOTE_G5, 0, 0, NOTE_E5, 0, 0, NOTE_A5, 0, NOTE_B5,
    0, NOTE_AS5, NOTE_A5, 0, NOTE_G5, NOTE_E6, NOTE_G6, NOTE_A6, 0, NOTE_F6,
    NOTE_G6, 0, NOTE_E6, 0, NOTE_C6, NOTE_D6, NOTE_B5, 0, 0
};

float beat[] = {
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 9, 9, 9, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12, 12, 12, 12, 12, 9, 9, 9, 12, 12, 12, 12, 12, 12, 12,
    12, 12, 12, 12, 12,
};

void mario_bgm() {
    stop_speaker();
    while(1){
        for(int i = 0; i <= length; i++) {
            if (status==GAMEOVER) {
                speaker=0.0;
                break;
            }
            if(frequency[i] == 0) {
                speaker = 0.0;
            } else {
                speaker.period(1.0 / frequency[i]);  // period = (1.0 / frequency)
                speaker = 0.5;                       // duty cycle = 50%
            }
            thread_sleep_for(2500.0 / beat[i]);     // duration = (C / beat) ms
        }
    }
}

void start_speaker() {
    status=0;
}

void stop_speaker() {
    status = 666;
}
