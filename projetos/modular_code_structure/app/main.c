#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hal_led.h" // Camada de abstracao para o LED

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        return -1;
    }

    // Inicializa a camada de abstração do LED
    hal_led_init();

    while (true) {
        hal_led_toggle(); // Usa a função de abstração para alternar o LED
        sleep_ms(500);    
    }
}