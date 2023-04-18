/*
#include "esp_timer.h"

#define BYTES_PER_SECOND 22023.4
#define BUFFER_SIZE 246

int main() {
    uint8_t buffer[BUFFER_SIZE];
    esp_timer_handle_t timer;
    int64_t start_time, end_time, elapsed_time, expected_time, delay_time;

    // inicializa o temporizador
    esp_timer_create_args_t timer_config = {
        .callback = NULL,
        .arg = NULL,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "timer"
    };
    esp_timer_create(&timer_config, &timer);

    // marca o início do processamento
    start_time = esp_timer_get_time();

    while (1) {
        // produzir dados no buffer
        buffer = {0x11,0x01 ,0x01 ,0x01,0x11,0x01 ,0x01 ,0x01,0x11,0x01 ,0x01 ,0x01,0x11,0x01 ,0x01 ,0x01 ,0x01 ,0x01,0x11,0x01 ,0x01 ,0x01};

        // calcular o tempo esperado para produzir BUFFER_SIZE bytes
        expected_time = (int64_t) ((double) BUFFER_SIZE / BYTES_PER_SECOND * 1000000);

        // calcular o tempo decorrido desde o início do processamento
        end_time = esp_timer_get_time();
        elapsed_time = end_time - start_time;

        // calcular o tempo de atraso necessário para atingir a taxa de produção desejada
        delay_time = expected_time - elapsed_time;

        if (delay_time > 0) {
            // esperar pelo tempo de atraso
            esp_timer_start_once(timer, delay_time);
            while (esp_timer_get_time() - end_time < delay_time);
        }
    }

    return 0;
}

*/