# Exemplo Código Modular (Pico W)

## 🎯 Objetivo do Projeto

Este projeto demonstra uma arquitetura de código modular (Driver/HAL/Aplicação) para um sistema embarcado simples utilizando a placa Raspberry Pi Pico W. O objetivo é estruturar o código para piscar o LED embutido de forma a promover manutenibilidade, testabilidade e reusabilidade, separando o controle específico de hardware (Driver), a abstração de hardware (HAL) e a lógica principal da aplicação.

## 🔧 Componentes Utilizados

- Placa Raspberry Pi Pico W (inclui microcontrolador RP2040 e chip CYW43439 para controle de Wi-Fi/Bluetooth e do LED embutido)
- LED embutido da Pico W

## 📌 Pinagem / Controle do Dispositivo

| Recurso      | Mecanismo de Controle      | Observações                            |
|--------------|----------------------------|----------------------------------------|
| LED Embutido | `cyw43_arch_gpio_put()`    | Controlado através do chip Wi-Fi CYW43. Macro utilizada: `CYW43_WL_GPIO_LED_PIN` |

*Observação: O LED embutido não está conectado a um pino GPIO padrão do RP2040 acessível pelo usuário, mas é gerenciado através da API `cyw43_arch`.*

## ⚙️ Como Compilar e Executar

### Pré-requisitos

- SDK do Raspberry Pi Pico instalado
- CMake
- Compilador ARM GCC (geralmente incluído na configuração do SDK Pico)

### Compilação

1.  Clone o repositório (substitua pela URL e caminho do seu repositório real):
    ```bash
    git clone https://github.com/danlvr/danilo_oliveira_embarcatech_HBr_2025.git
    cd projetos/modular_code_structure
    ```

2.  Configure a variável de ambiente do SDK Pico (se ainda não estiver configurada):
    ```bash
    # Linux/macOS
    export PICO_SDK_PATH=/caminho/para/seu/pico-sdk
    # Windows (Prompt de Comando - ajuste o caminho)
    # set PICO_SDK_PATH=C:\caminho\para\seu\pico-sdk
    # Windows (PowerShell - ajuste o caminho)
    # $env:PICO_SDK_PATH = "C:\caminho\para\seu\pico-sdk"
    ```

3.  Crie um diretório de build e compile:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4.  O arquivo binário (`.uf2`) será gerado no diretório `build` (ex: `build/nome_do_projeto.uf2`).

### Execução

1.  Conecte a Raspberry Pi Pico W ao seu computador enquanto mantém pressionado o botão BOOTSEL.
2.  Ela aparecerá como um dispositivo de armazenamento em massa (semelhante a um pendrive).
3.  Copie o arquivo `.uf2` gerado para esta unidade.
4.  O dispositivo reiniciará automaticamente e começará a executar o programa.

## 📊 Resultados Esperados/Observados

- Ao ligar ou após a gravação do firmware, a Pico W inicializa.
- O LED embutido começa a piscar de forma constante com um período de aproximadamente 1 segundo (500ms ligado, 500ms desligado).
- A aplicação utiliza a Camada de Abstração de Hardware (HAL) para controlar o LED, abstraindo as chamadas diretas de hardware.

## 🔍 Notas Técnicas

- O projeto segue uma estrutura modular:
    - **`drivers/`**: Contém código de baixo nível (`led_embutido.c`) que interage diretamente com a API `cyw43_arch` para controlar o LED físico.
    - **`hal/`**: Fornece uma Camada de Abstração de Hardware (`hal_led.c`) com funções como `hal_led_toggle()`, `hal_led_on()`, `hal_led_off()`. Esta camada utiliza as funções do driver.
    - **`app/`**: Contém a lógica principal da aplicação (`main.c`) que interage apenas com a HAL, não diretamente com os drivers.
    - **`include/`**: Contém os arquivos de cabeçalho (`.h`) para os drivers e a HAL, definindo suas interfaces públicas.
- O `CMakeLists.txt` está configurado para encontrar os arquivos-fonte em seus respectivos diretórios e linkar corretamente as bibliotecas necessárias do SDK Pico (`pico_stdlib`, `cyw43_arch...`).
- O uso de `cyw43_arch_init()` e funções relacionadas é necessário para controlar o LED embutido na Pico W.

## 📚 Bibliotecas Utilizadas

- SDK do Raspberry Pi Pico:
    - `pico_stdlib` para funções básicas (`sleep_ms`, `stdio_init_all`).
    - `pico/cyw43_arch.h` para controlar o LED embutido através do chip Wi-Fi/BT.

---

*Este projeto foi desenvolvido como parte do programa EmbarcaTech*