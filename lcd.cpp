# include "lcd.h"
#include "C12832.h"

C12832 lcd(D11, D13, D12, D7, D10); // lcd = (MOSI, SCK, RESET, A0, nCS)

void poo_setting_lcd() {
    lcd.cls();
    lcd.locate(0, 6);
    lcd.printf("Avoiding Poo !!!");
}

void poo_start_lcd() {
    lcd.locate(0, 16); // goto (0, 16)
    lcd.printf("Game Start !!!"); // print count
}

void poo_end_lcd() {
    lcd.locate(0, 16);
    lcd.printf("POO Game Loosed !!!");   
}

void dino_setting_lcd() {
    lcd.cls();
    lcd.locate(0, 6);
    lcd.printf("Jumping Dino !!!");  
}

void dino_start_lcd() {
    lcd.locate(0, 16);
    lcd.printf("Game Start !!!");  
}

void dino_end_lcd() {
    lcd.locate(0, 16);
    lcd.printf("DINO Game Loosed !!!");   
}