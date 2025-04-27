# Teste Unitário Conversor de Temperatura ADC (Pico W)

## 🎯 Objetivo do Projeto

Este projeto implementa e testa unitariamente uma função que converte a leitura bruta (12 bits) do sensor de temperatura interno do microcontrolador RP2040 (presente na Pico W) para graus Celsius (°C). O objetivo principal é verificar a corretude da função de conversão `adc_to_celsius` usando valores de entrada simulados e o framework de testes unitários Unity.

## 🔧 Componentes Utilizados

- **Software:**
    - SDK do Raspberry Pi Pico (para tipos e `pico_stdlib` na configuração)
    - Framework de Testes Unitários Unity
    - CMake
    - Compilador ARM GCC
- **Hardware (para execução do teste):**
    - Placa Raspberry Pi Pico W 
    - Cabo USB para conexão e comunicação serial

## 🧮 Função Testada

A função principal sob teste é `float adc_to_celsius(uint16_t adc_val)`, localizada em `src/temperature_converter.c`. Ela utiliza a fórmula documentada para o RP2040:

`Temp (°C) = 27 - (ADC_voltage - 0.706) / 0.001721`

Considerando `ADC_voltage = adc_val * (3.3 / 4095.0)`.

## ✅ Testes Realizados

Os testes unitários, localizados em `test/test_temperature_converter.c`, verificam a função `adc_to_celsius` com os seguintes cenários simulados:

1.  **Ponto de Referência (próximo a 27°C):** Usa um valor ADC (876) próximo ao que corresponde a 0.706V e verifica se a saída é próxima a 27°C, calculando o valor exato esperado para essa entrada ADC.
2.  **Temperatura Moderada (próximo a 20°C):** Usa um valor ADC (891) que deve resultar em aproximadamente 20°C e verifica o resultado contra o valor exato esperado para essa entrada.
3.  **Temperatura Mais Alta (ADC < 876):** Usa um valor ADC (850) e verifica se a saída corresponde à temperatura mais alta calculada (~38.48°C).
4.  **Temperatura Mais Baixa (ADC > 891):** Usa um valor ADC (950) e verifica se a saída corresponde à temperatura mais baixa calculada (~ -8.13°C).

Todos os testes utilizam a macro `TEST_ASSERT_FLOAT_WITHIN` do Unity para comparar os resultados com uma margem de erro aceitável (`delta`), cobrindo pequenas imprecisões de ponto flutuante.

## ⚙️ Como Compilar e Executar os Testes

### Pré-requisitos

- SDK do Raspberry Pi Pico instalado e configurado (`PICO_SDK_PATH`)
- CMake
- Compilador ARM GCC

### Compilação

1.  Clone o repositório (se ainda não o fez):
    ```bash
    git clone https://github.com/danlvr/danilo_oliveira_embarcatech_HBr_2025.git
    cd projetos/pico_temp_unity_test
    ```
2.  Certifique-se que a biblioteca Unity está presente no diretório `lib/unity/src/` (arquivos `unity.c`, `unity.h`, `unity_internals.h`).
3.  Crie um diretório de build e compile:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
4.  O arquivo binário do executor de testes (`.uf2`) será gerado no diretório `build` (ex: `build/pico_temp_unity_test.uf2`).

### Execução e Verificação

1.  Conecte a Raspberry Pi Pico W ao seu computador enquanto mantém pressionado o botão BOOTSEL.
2.  Copie o arquivo `.uf2` gerado para o drive RPI-RP2 que aparecerá.
3.  Abra um monitor serial (PuTTY, Minicom, Terminal Integrado do VS Code com extensão Serial Monitor, etc.) conectado à porta serial correspondente à Pico W (a taxa de baud geralmente é 115200).
4.  Você deverá ver a saída do Unity indicando o status de cada teste (PASS/FAIL) e um resumo final.

## 📊 Resultados Esperados/Observados na Saída Serial

A saída esperada no monitor serial deve ser semelhante a esta:

```/path/to/test_temperature_converter.c:LINE:test_adc_to_celsius_near_reference_point:PASS
/path/to/test_temperature_converter.c:LINE:test_adc_to_celsius_at_20C:PASS
/path/to/test_temperature_converter.c:LINE:test_adc_to_celsius_higher_temp:PASS
/path/to/test_temperature_converter.c:LINE:test_adc_to_celsius_lower_temp:PASS
4 Tests 0 Failures 0 Ignored
OK
```

*(Nota: Os números de linha (`LINE`) variarão. O caminho do arquivo pode ser completo ou omitido dependendo das configurações de compilação do Unity).*

## 🔍 Notas Técnicas

- O `CMakeLists.txt` está configurado para construir um executável (`pico_temp_unity_test`) que contém a função `adc_to_celsius`, o código de teste e o framework Unity.
- A função `main()` para executar os testes está localizada dentro do arquivo `test/test_temperature_converter.c`.
- A saída serial (USB ou UART) é habilitada no CMake para permitir a visualização dos resultados do Unity.
- A biblioteca `pico_stdlib` é linkada para fornecer `stdio_init_all()` e outras funções básicas necessárias.

## 📚 Bibliotecas Utilizadas

- SDK do Raspberry Pi Pico:
    - `pico_stdlib`
- Framework de Testes Unitários:
    - Unity (https://github.com/ThrowTheSwitch/Unity)

---

*Este projeto foi desenvolvido como parte do programa EmbarcaTech*