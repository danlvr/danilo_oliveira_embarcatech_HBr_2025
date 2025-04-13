#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"    // Biblioteca para o ADC
#include "include/ssd1306.h" // Biblioteca para o display OLED

#define I2C_SDA_PIN 14           // GPIO14 para SDA
#define I2C_SCL_PIN 15           // GPIO15 para SCL
#define JOYSTICK_X_GPIO 27       // VRx no GPIO27 -> ADC1
#define JOYSTICK_Y_GPIO 26       // VRy no GPIO26 -> ADC0
#define JOYSTICK_X_ADC_CHANNEL 1 // Canal ADC para GPIO27
#define JOYSTICK_Y_ADC_CHANNEL 0 // Canal ADC para GPIO26

#define READ_DELAY_MS 100 // Delay entre leituras do joystick

// Valores para calibração (medidos na leitura do joystick)
#define RAW_X_MIN 12
#define RAW_X_MAX 4076
#define RAW_Y_MIN 12
#define RAW_Y_MAX 4076

// Valores calibrados
#define CALIBRATED_MIN 0
#define CALIBRATED_MAX 4095

// Display OLED
static ssd1306_t display_oled;

// Strings para armazenar os valores lidos
char calibrated_x_str[20];
char calibrated_y_str[20];

// Função para mapear os valores lidos do joystick
uint16_t map_value(uint16_t value, uint16_t from_min, uint16_t from_max, uint16_t to_min, uint16_t to_max);

int main()
{
    stdio_init_all();

    // Inicializa o i2c para o display OLED
    i2c_init(i2c1, 400000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Instancia, inicializa e limpa o display OLED
    display_oled.external_vcc = false;
    ssd1306_init(&display_oled, 128, 64, 0x3C, i2c1);

    // Inicializa o ADC
    adc_init();

    // Inicializa os pinos GPIO para o joystick
    adc_gpio_init(JOYSTICK_X_GPIO);
    adc_gpio_init(JOYSTICK_Y_GPIO);

    while (true)
    {
        // Seleçao de canal e leitura do ADC - EIXO X
        adc_select_input(JOYSTICK_X_ADC_CHANNEL);
        uint16_t raw_x = adc_read();

        // Seleçao de canal e leitura do ADC - EIXO Y
        adc_select_input(JOYSTICK_Y_ADC_CHANNEL);
        uint16_t raw_y = adc_read();

        // O valor lido do joystick é calibrado para o intervalo desejado
        uint16_t calibrated_x = map_value(raw_x, RAW_X_MIN, RAW_X_MAX, CALIBRATED_MIN, CALIBRATED_MAX);
        uint16_t calibrated_y = map_value(raw_y, RAW_Y_MIN, RAW_Y_MAX, CALIBRATED_MIN, CALIBRATED_MAX);

        // Mostra os valores no display OLED
        ssd1306_clear(&display_oled);
        sprintf(calibrated_x_str, "%4d", calibrated_x);
        sprintf(calibrated_y_str, "%4d", calibrated_y);
        ssd1306_draw_string(&display_oled, 25, 15, 2, "X:");
        ssd1306_draw_string(&display_oled, 50, 15, 2, calibrated_x_str);
        ssd1306_draw_string(&display_oled, 25, 40, 2, "Y:");
        ssd1306_draw_string(&display_oled, 50, 40, 2, calibrated_y_str);
        ssd1306_show(&display_oled);

        // Delay antes de ler novamente
        sleep_ms(READ_DELAY_MS);
    }

    return 0;
}

// Garante que o resultado fique dentro da faixa desejada (clamping)
uint16_t map_value(uint16_t value, uint16_t from_min, uint16_t from_max, uint16_t to_min, uint16_t to_max)
{
    // Evitar divisão por zero se min e max observados forem iguais
    if (from_min == from_max)
    {
        return (to_min + to_max) / 2; // Retorna o valor médio
    }

    if (value <= from_min)
        return to_min;
    if (value >= from_max)
        return to_max;

    // Realiza o mapeamento linear usando float para melhor precisão intermediária
    float mapped = (float)(value - from_min) * (to_max - to_min) / (float)(from_max - from_min) + to_min;

    // Arredonda para o inteiro mais próximo e converte para uint16_t
    uint16_t result = (uint16_t)(mapped + 0.5f);

    // Garante  que o resultado final está estritamente dentro dos limites desejados
    if (result < to_min)
        result = to_min;
    if (result > to_max)
        result = to_max;

    return result;
}