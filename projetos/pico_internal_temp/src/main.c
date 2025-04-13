#include <stdio.h>
#include "hardware/adc.h"    // Biblioteca para o ADC
#include "include/ssd1306.h" // Biblioteca para o display OLED

#define I2C_SDA_PIN 14                                            // GPIO14 para SDA
#define I2C_SCL_PIN 15                                            // GPIO15 para SCL
#define TEMP_ADC_CHANNEL 4                                        // Sensor de temperatura interna no canal 4 do ADC
#define ADC_REF_VOLTAGE 3.3f                                      // Voltagem de referência do ADC (3.3V)
#define ADC_RANGE (1 << 12)                                       // ADC de 12-bit tem 2^12 = 4096 valores possíveis
#define ADC_CONVERSION_FACTOR (ADC_REF_VOLTAGE / (ADC_RANGE - 1)) // Fator de conversão ADC para volts

#define READ_DELAY_MS 1000  // Delay entre leituras
#define NUM_TEMP_SAMPLES 100 // Número de amostras para tirar a média

// Display OLED
static ssd1306_t display_oled;

char temperature_str[20];

// Função para ler a temperatura interna do RP2040
float read_internal_temperature_celsius();

int main()
{
    // Initialize standard I/O over USB
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

    // Ativa o sensor de temperatura interna
    adc_set_temp_sensor_enabled(true);

    while (true)
    {
        // Leitura da temperatura interna
        float temperature = read_internal_temperature_celsius();

        // Mostra os valores no display OLED
        ssd1306_clear(&display_oled);
        sprintf(temperature_str, "%.2f", temperature);
        ssd1306_draw_string(&display_oled, 30, 10, 1, "Temperatura");
        ssd1306_draw_string(&display_oled, 20, 20, 1, "interna do Pico:");
        ssd1306_draw_string(&display_oled, 25, 40, 2, temperature_str);
        ssd1306_draw_string(&display_oled, 90, 40, 2, "C");
        ssd1306_show(&display_oled);

        // Aguarda um tempo antes de fazer a próxima leitura
        sleep_ms(READ_DELAY_MS);
    }

    return 0;
}

float read_internal_temperature_celsius()
{
    // Média de temperatura
    float temp_sum = 0;

    // Seleciona o canal do ADC para o sensor de temperatura interna
    adc_select_input(TEMP_ADC_CHANNEL);

    for (int i = 0; i < NUM_TEMP_SAMPLES; i++)
    {
        // Lê o valor do ADC de 12 bits
        uint16_t raw = adc_read();

        // Converte o valor bruto do ADC para tensão
        float voltage = raw * ADC_CONVERSION_FACTOR;

        // Converte a tensão em temperatura usando a fórmula do datasheet do RP2040
        // Temp (°C) = 27 - (ADC_voltage - 0.706) / 0.001721
        temp_sum += (27.0f - (voltage - 0.706f) / 0.001721f);

        // Aguarda um pequeno delay entre as amostras
        sleep_us(100);
    }

    float temperature_c = temp_sum / NUM_TEMP_SAMPLES; // Calcula a média

    return temperature_c;
}