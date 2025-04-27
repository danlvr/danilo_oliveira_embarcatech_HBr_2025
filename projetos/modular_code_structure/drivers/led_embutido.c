#include "led_embutido.h"
#include "pico/cyw43_arch.h"

void led_embutido_set(int state) {
    // Usa a API de hardware de baixo nível para definir o estado do LED
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, state ? 1 : 0);
}