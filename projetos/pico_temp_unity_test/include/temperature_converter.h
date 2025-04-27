#ifndef TEMPERATURE_CONVERTER_H
#define TEMPERATURE_CONVERTER_H

#include <stdint.h>

/**
 * @brief Converte um valor bruto de 12 bits do ADC do sensor de temperatura
 *        interno do RP2040 para graus Celsius.
 *
 * A conversão assume uma tensão de referência (Vref) de 3.3V.
 * A fórmula utilizada é baseada na documentação do RP2040:
 * Temp (°C) = 27 - (ADC_voltage - 0.706) / 0.001721
 * Onde ADC_voltage = adc_val * (3.3 / 4095.0)
 *
 * @param adc_val O valor de 12 bits lido do canal ADC do sensor de temperatura (0-4095).
 * @return A temperatura calculada em graus Celsius (float).
 */
float adc_to_celsius(uint16_t adc_val);

#endif // TEMPERATURE_CONVERTER_H