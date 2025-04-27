#include "temperature_converter.h"

// Constantes baseadas na documentação e parâmetros do RP2040
const float ADC_VREF = 3.3f;
const float ADC_RANGE = (1 << 12); // 4096 valores (0 a 4095)
const float ADC_CONVERSION_FACTOR = ADC_VREF / (ADC_RANGE - 1); // Fator para converter valor ADC em Volts (3.3 / 4095.0)

// Constantes da fórmula de temperatura
const float TEMP_SENSOR_VOLTAGE_AT_27C = 0.706f;
const float TEMP_SENSOR_SLOPE = 0.001721f;

float adc_to_celsius(uint16_t adc_val) {
    // Converte o valor bruto do ADC para tensão
    // Garante que o valor não exceda o máximo de 12 bits (4095)
    if (adc_val > 4095) {
        adc_val = 4095;
    }
    float adc_voltage = (float)adc_val * ADC_CONVERSION_FACTOR;

    // Aplica a fórmula de conversão para Celsius
    float temperature_c = 27.0f - (adc_voltage - TEMP_SENSOR_VOLTAGE_AT_27C) / TEMP_SENSOR_SLOPE;

    return temperature_c;
}