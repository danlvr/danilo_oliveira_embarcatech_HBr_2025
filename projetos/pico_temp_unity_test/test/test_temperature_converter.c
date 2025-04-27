#include "unity.h"
#include "temperature_converter.h"
#include "pico/stdlib.h"

const float TEST_ADC_VREF = 3.3f;
const float TEST_ADC_RANGE_MINUS_1 = 4095.0f; // (1 << 12) - 1
const float TEST_ADC_CONVERSION_FACTOR = TEST_ADC_VREF / TEST_ADC_RANGE_MINUS_1;
const float TEST_TEMP_SENSOR_VOLTAGE_AT_27C = 0.706f;
const float TEST_TEMP_SENSOR_SLOPE = 0.001721f;

void setUp(void) {}
void tearDown(void) {}

/**
 * @brief Testa a conversão ADC para Celsius usando um valor ADC próximo ao
 *        ponto de referência de 27°C e calculando a temperatura exata esperada
 *        para ESSE valor ADC.
 */
void test_adc_to_celsius_near_reference_point(void)
{
    // Valor ADC próximo ao esperado para 0.706V
    uint16_t test_adc_input = 876;

    // Calcular a temperatura ESPERADA para este valor ADC
    float expected_voltage = (float)test_adc_input * TEST_ADC_CONVERSION_FACTOR;
    float expected_temp_c = 27.0f - (expected_voltage - TEST_TEMP_SENSOR_VOLTAGE_AT_27C) / TEST_TEMP_SENSOR_SLOPE;

    // Tolerância
    float delta = 0.001f; // Margem pequena para erros de ponto flutuante

    // Função sob teste com o valor ADC escolhido
    float actual_temp_c = adc_to_celsius(test_adc_input);

    // Verifica se o resultado real corresponde ao resultado esperado calculado
    TEST_ASSERT_FLOAT_WITHIN(delta, expected_temp_c, actual_temp_c);
}

// Teste para 20C
void test_adc_to_celsius_at_20C(void)
{
    uint16_t test_adc_input = 891; // Valor ADC aproximado para 20C

    // Calcular o esperado para 891
    float expected_voltage = (float)test_adc_input * TEST_ADC_CONVERSION_FACTOR;
    float expected_temp_c = 27.0f - (expected_voltage - TEST_TEMP_SENSOR_VOLTAGE_AT_27C) / TEST_TEMP_SENSOR_SLOPE;

    float delta = 0.01f; // Tolerância
    float actual_temp_c = adc_to_celsius(test_adc_input);
    TEST_ASSERT_FLOAT_WITHIN(delta, expected_temp_c, actual_temp_c);
}

// Temperatura alta
void test_adc_to_celsius_higher_temp(void)
{
    uint16_t test_adc_input = 850;
    float expected_voltage = (float)test_adc_input * TEST_ADC_CONVERSION_FACTOR;
    float expected_temp_c = 27.0f - (expected_voltage - TEST_TEMP_SENSOR_VOLTAGE_AT_27C) / TEST_TEMP_SENSOR_SLOPE; // ~38.48 C
    float delta = 0.01f;                                                                                           // Tolerância

    float actual_temp_c = adc_to_celsius(test_adc_input);
    TEST_ASSERT_FLOAT_WITHIN(delta, expected_temp_c, actual_temp_c);
}

// Temperatura baixa
void test_adc_to_celsius_lower_temp(void)
{
    uint16_t test_adc_input = 950;
    float expected_voltage = (float)test_adc_input * TEST_ADC_CONVERSION_FACTOR;
    float expected_temp_c = 27.0f - (expected_voltage - TEST_TEMP_SENSOR_VOLTAGE_AT_27C) / TEST_TEMP_SENSOR_SLOPE; // ~ -8.13 C
    float delta = 0.01f;                                                                                           // Tolerância

    float actual_temp_c = adc_to_celsius(test_adc_input);
    TEST_ASSERT_FLOAT_WITHIN(delta, expected_temp_c, actual_temp_c);
}

int main(void)
{
    stdio_init_all();
    sleep_ms(5000); // Aguarda 5 segundos para garantir que a serial esteja pronta

    // Inicializa o Unity
    UNITY_BEGIN();
    printf("Iniciando os testes...\n");
    sleep_ms(2000);

    printf("Testando a conversão ADC para Celsius...\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_near_reference_point);
    sleep_ms(2000);

    printf("Testando a conversão ADC para Celsius em 20C...\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_at_20C);
    sleep_ms(2000);

    printf("Testando a conversão ADC para Celsius em temperatura mais alta...\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_higher_temp);
    sleep_ms(2000);

    printf("Testando a conversão ADC para Celsius em temperatura mais baixa...\n");
    sleep_ms(2000);
    RUN_TEST(test_adc_to_celsius_lower_temp);
    sleep_ms(2000);

    printf("Todos os testes concluídos.\n \n");
    printf("Resultados:\n");
    return UNITY_END();
}