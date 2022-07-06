#include "poo.h"
#include "Dino.h"
#include "mbed.h"
#include "ESP-call.h"

Thread set_mode_thread, speaker_tread;
AnalogIn pot2(A1);
AnalogIn pot1(A0);

int MODE = 0;

NodeMCU_PGM http_server_LED[] = {
    "", 1,
    "wifi.setmode(wifi.STATION)", 1,
    "station_cfg={}", 1,
    "station_cfg.ssid=\"ASUS_D8\"", 1,
    "station_cfg.pwd=\"zaq0505zaq!723\"", 1,
    "station_cfg.save=false", 1,
    "wifi.sta.config(station_cfg)", 1,
    "", 1, 
    "wifi.sta.connect()", 80,
    "", 1,
    "print(wifi.sta.status())", 1,
    "print(wifi.sta.getip())", 1,
    "", 1,
    "http_resp = \"HTTP/1.0 200 OK\\r\\nContent-Type: text/html\\r\\n\\r\\n\"", 5,
    "html_main1 = \"<h1>Welcome GUI WOO Workspace!</h1>\"", 1, 
    "html_main2 = \"<h1>Select Games\"", 1,
    "", 1,
    "html_sub0 = \"<label><form method='post' action='set_mode_thread'></label>\"", 1, 
    "html_sub1 = \"<label><input type='radio' name='MODE' value=1> AvoidPoop</label>\"", 1,
    "html_sub2 = \"<label><input type='radio' name='MODE' value=2> DinoJumping</label>\"", 1,
    "html_sub3 = \"<label><input type='submit' value='START'></label></h1>\"", 1,
    "html_sub4 = \"</form>\"", 1,
    "", 1,
    "srv = net.createServer(net.TCP)", 1, 
    "srv:listen(80, function(conn)", 1,
    "   conn:on(\"receive\", function(sck, payload)", 1,
    "       if string.match(payload, \"MODE=1\")", 1,
    "       then print(\"MODE=1\") AvoidPoop=\"checked\" DinoJumping=\"\" end", 1,
    "       if string.match(payload, \"MODE=2\")", 1,
    "       then print(\"MODE=2\") AvoidPoop=\"\" DinoJumping=\"checked\" end", 1,
    "       print(\"~\")", 1,
    "       sck:send(http_resp)", 1,
    "       sck:send(html_main1)", 1,
    "       sck:send(html_main2)", 1,
    "       sck:send(html_sub0)", 1,
    "       sck:send(html_sub1)", 1, 
    "       sck:send(html_sub2)", 1, 
    "       sck:send(html_sub3)", 1, 
    "       sck:send(html_sub4)", 1,
    "       end)", 1,
    "   conn:on(\"sent\", function(sck) sck:close() end)", 1,
    "end)", 1,
    NULL, 0,
};


void run_games() {
    if (MODE == 1){    
        run_poo_thread();
        MODE = 0;
    } else if (MODE == 2){
        run_dino_thread();
        MODE = 0;
    } else
       return;
}

//void set_mode_th(void) {
//    while (true) {
//        if (pot2.read() > 0.5) {
//            MODE = 2;
//            thread_sleep_for(100);
//        }
//        if (pot1.read() > 0.5 ) {
//            MODE = 1;
//            thread_sleep_for(100);
//        }
//        run_games();
////        ESP_recv_buffer_index = 0;
////        ESP_recv_buffer[0] = '\0';
//        thread_sleep_for(100);
//    }
//}

void set_mode_th(void) {
    while (true) {
        if (strstr(ESP_recv_buffer, "MODE=1")!=NULL) {
            MODE = 1;
        } else if (strstr(ESP_recv_buffer, "MODE=2")!=NULL) {
            MODE = 2;
        } 
        run_games();
        ESP_recv_buffer_index = 0;
        ESP_recv_buffer[0] = '\0';
        thread_sleep_for(1000);
    }
}
Serial pc(USBTX, USBRX, 115200);

int main()
{
    set_mode_thread.start(set_mode_th);
    speaker_tread.start(mario_bgm);
    // Config baudrate of PC and ESP
    PC.baud(115200);
    ESP.baud(115200);
    
    // Reset ESP
    PC.printf("\r\nReset ESP...\r\n");
    ESP_reset();

    // Setup ESP noecho mode
    PC.printf("Setup ESP noecho...\r\n");
    ESP_noecho();
    
    // Execute a NodeMCU program
    PC.printf("Execute a NodeMCU program...\r\n");
    ESP_call_multi(http_server_LED);
    
    // Config ESP to PC receive mode
    PC.printf("\r\nESP receive mode...\r\n");
//    ESP.attach(&ISR_ESP_to_PC, Serial::RxIrq);
    ESP.attach(&ISR_ESP_to_recv_buffer, Serial::RxIrq);
    
    while (true);
    set_mode_thread.terminate();
}
