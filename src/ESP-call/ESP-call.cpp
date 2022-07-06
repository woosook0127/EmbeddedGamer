// ======================================================================

#include "mbed.h"
#include "ESP-call.h"

// ======================================================================

RawSerial PC(USBTX, USBRX);         // PC = (USBTX=TX, USBRX=RX)
RawSerial ESP(D1, D0);              // ESP = (D1=TX, D0=RX)
DigitalOut ESP_reset_pin(D2);       // ESP_reset_pin = D2

// ======================================================================

char ESP_recv_buffer[1028];         // ESP receive buffer
int ESP_recv_buffer_index;          // ESP receive buffer index

// ======================================================================
// ISR for redirecting PC RX to ESP TX

void ISR_PC_to_ESP()
{
    while(PC.readable()) {
        ESP.putc(PC.getc());
    }
}

// ======================================================================
// ISR for redirecting ESP RX to PC TX

void ISR_ESP_to_PC()
{
    while(ESP.readable()) {
        PC.putc(ESP.getc());
    }
}

// ======================================================================
// ISR for redirecting ESP RX to ESP_recv_buffer[]

void ISR_ESP_to_recv_buffer()
{
    while(ESP.readable()) {
        ESP_recv_buffer[ESP_recv_buffer_index++] = ESP.getc();
    }
}

// ======================================================================
// Reset ESP and wait for 1 second

void ESP_reset(void)
{
    ESP_reset_pin = 0;
    ESP_reset_pin = 1;
    thread_sleep_for(1000);
}

// ======================================================================
// Setup ESP echo/noecho mode
// ======================================================================

void ESP_echo(void)
{
    NodeMCU_PGM uart_setup_echo = {
        "uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 1)\r\n", 1,
    };

    ESP_call_single(uart_setup_echo);
}

void ESP_noecho(void)
{
    NodeMCU_PGM uart_setup_noecho = {
        "uart.setup(0, 115200, 8, uart.PARITY_NONE, uart.STOPBITS_1, 0)\r\n", 1,
    };

    ESP_call_single(uart_setup_noecho);
}

// ======================================================================
// Send NodeMCU_PGM to ESP and execute

char *ESP_call_single(NodeMCU_PGM pgm)
{
    ESP_recv_buffer_index = 0;
    ESP.attach(&ISR_ESP_to_recv_buffer, Serial::RxIrq);
    ESP.printf("%s\r\n", pgm.code);
    thread_sleep_for((pgm.delay) * 100);
    ESP.attach(NULL, Serial::RxIrq);
    ESP_recv_buffer[ESP_recv_buffer_index] = '\0';
    return ESP_recv_buffer;
}

void ESP_call_multi(NodeMCU_PGM pgms[])
{
    int i;
    char *p;

    for(i = 0; pgms[i].code != NULL; i++) {
        PC.printf("%s\r\n", pgms[i].code);
        p = ESP_call_single(pgms[i]);
        PC.printf("%s", p);
    }
}

// ======================================================================
