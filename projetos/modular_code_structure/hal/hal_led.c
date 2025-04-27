#include "hal_led.h"
#include "led_embutido.h" // Driver para o LED embutido

// Estado interno para monitorar o status do LED para alternância
static bool current_led_state = false;

void hal_led_init(void) {
    // Defina um estado inicial conhecido (por exemplo, desligado) usando o driver
    led_embutido_set(0);
    current_led_state = false;
}

void hal_led_toggle(void) {
    current_led_state = !current_led_state; // Inverte o estado
    led_embutido_set(current_led_state);    // Chamar a função do driver
}

void hal_led_on(void) {
    led_embutido_set(1);
    current_led_state = true;
}

void hal_led_off(void) {
    led_embutido_set(0);
    current_led_state = false;
}