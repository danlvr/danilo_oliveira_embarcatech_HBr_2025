#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "temperature_converter.h"

// O canal ADC para o sensor de temperatura interno é o 4
const uint ADC_TEMP_CHANNEL = 4;

int main()
{
    stdio_init_all();

    printf("===================================\n");
    printf(" Leitor de Temperatura Interna RP2040\n");
    printf("===================================\n");

    // Inicializa o periférico ADC
    adc_init();

    // Habilita o sensor de temperatura interno
    adc_set_temp_sensor_enabled(true);

    // Seleciona o canal ADC 4 (sensor de temperatura) como entrada
    adc_select_input(ADC_TEMP_CHANNEL);

    printf("Iniciando leituras...\n");

    while (true)
    {
        // Realiza a leitura do ADC. Retorna um valor de 12 bits (0-4095)
        uint16_t raw_adc_value = adc_read();

        // Converte o valor bruto do ADC para graus Celsius 
        float temp_celsius = adc_to_celsius(raw_adc_value);

        // Imprime o valor formatado no console serial
        // %.2f formata o float para ter duas casas decimais
        printf("Temperatura: %.2f C\n", temp_celsius);

        // Pausa por 1000 milissegundos (1 segundo)
        sleep_ms(1000);
    }

    return 0;
}