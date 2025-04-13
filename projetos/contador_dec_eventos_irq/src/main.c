#include <stdio.h>
#include "pico/stdlib.h"
#include "include/ssd1306.h" // Biblioteca para o display OLED

#define BUTTON_A_PIN 5 // GPIO5 para Botão A (Start/Reset)
#define BUTTON_B_PIN 6 // GPIO6 para Botão B (Aumentar contador durante contagem regressiva)
#define I2C_SDA_PIN 14 // GPIO14 para SDA
#define I2C_SCL_PIN 15 // GPIO15 para SCL

#define DEBOUNCE_TIME_US 200000 // 200ms tempo de debounce

volatile int countdown_value = 0;            // Valor inicial da contagem regressiva
volatile int button_b_count = 0;             // Quantas vezes o botão B foi pressionado
volatile bool countdown_active = false;      // Flag: A contagem regressiva está ativa?
volatile bool button_a_flag = false;         // Flag: O Botão A foi pressionado?
volatile bool display_update_needed = false; // Flag: O display precisa ser atualizado?

// Variaveis para debouncing
volatile uint64_t last_press_time_btnA = 0;
volatile uint64_t last_press_time_btnB = 0;

// Handler para o timer
struct repeating_timer timer;

// Display OLED
static ssd1306_t display_oled;

char countdown_str[20];
char button_count_str[20];

// Prototipos das funções
void gpio_callback(uint gpio, uint32_t events);
bool repeating_timer_callback(struct repeating_timer *t);
void update_countdown_display(int countdown, int button_count);

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

    // Botão A
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    // Botão B
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Seta o mesmo callback para ambos os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true);

    // Adicione um timer de repetição que chama repeating_timer_callback a cada 1000 ms (1 segundo)
    // Observação: um intervalo negativo significa que ele se repete. O ponteiro user_data é NULL.
    if (!add_repeating_timer_ms(-1000, repeating_timer_callback, NULL, &timer))
    {
        printf("Falha ao criar timer\n");
        return 1;
    }

    // Texto inicial no display
    ssd1306_clear(&display_oled);
    ssd1306_draw_string(&display_oled, 14, 2, 1, "Cont. Regressiva");
    ssd1306_draw_string(&display_oled, 20, 20, 1, "Btn A: Inicia");
    ssd1306_draw_string(&display_oled, 63, 30, 1, "Reseta");
    ssd1306_draw_string(&display_oled, 20, 45, 1, "Btn B: Aumenta");
    ssd1306_draw_string(&display_oled, 63, 55, 1, "contador");
    ssd1306_show(&display_oled);

    while (true)
    {
        // Checa se o ISR do Botão A foi chamado
        if (button_a_flag)
        {
            // Reseta a flag
            button_a_flag = false;

            // Reseta a logica inicial da contagem regressiva
            countdown_value = 9;
            button_b_count = 0;      // Resetando o contador de Botão B
            countdown_active = true; // Flag para contagem regressiva ativa

            update_countdown_display(countdown_value, button_b_count);
        }

        // Checa se o display precisa ser atualizado
        if (display_update_needed)
        {
            display_update_needed = false;
            update_countdown_display(countdown_value, button_b_count);
        }

        sleep_ms(10); // Sleep breve para evitar uso excessivo da CPU
    }

    return 0;
}

// Função de callback para interrupção de GPIO
// (Interrupt Service Routine - ISR)
void gpio_callback(uint gpio, uint32_t events)
{
    uint64_t now = time_us_64();

    // Lida com o Botão A
    if (gpio == BUTTON_A_PIN)
    {
        // Checa debounce
        if ((now - last_press_time_btnA) > DEBOUNCE_TIME_US)
        {
            last_press_time_btnA = now;
            // Seta flag para indicar que o botão A foi pressionado e a logica é tratada no loop principal
            button_a_flag = true;
        }
    }
    // Lida com o Botão B
    else if (gpio == BUTTON_B_PIN)
    {
        // Checa debounce
        if ((now - last_press_time_btnB) > DEBOUNCE_TIME_US)
        {
            last_press_time_btnB = now;
            // Apenas aumenta a contagem se a contagem regressiva estiver ativa
            if (countdown_active)
            {
                button_b_count++;             // Incrementa o contador de Botão B
                display_update_needed = true; // Marca que o display precisa ser atualizado
            }
        }
    }
}

// Callback para o timer (chamado a cada 1 segundo)
bool repeating_timer_callback(struct repeating_timer *t)
{

    if (countdown_active)
    {
        if (countdown_value > 0)
        {
            countdown_value--;
            display_update_needed = true; // Marca que o display precisa ser atualizado
        }

        // Checa se a contagem regressiva chegou a zero
        if (countdown_value == 0)
        {
            countdown_active = false; // Flag para contagem regressiva inativa
            printf("Contagem Regressiva Finalizada!\n");
        }
    }
    return true; // Mantem o timer ativo
}

/**
 * @brief Atualiza o display OLED com o valor atual da contagem regressiva e o contador de Botão B.
 */
void update_countdown_display(int countdown, int button_count)
{
    ssd1306_clear(&display_oled);
    sprintf(countdown_str, "%d", countdown);
    sprintf(button_count_str, "%d", button_count);
    ssd1306_draw_string(&display_oled, 65, 10, 2, countdown_str);
    ssd1306_draw_string(&display_oled, 30, 40, 1, "Botao B:");
    ssd1306_draw_string(&display_oled, 90, 35, 2, button_count_str);
    ssd1306_show(&display_oled);
    sleep_ms(25); // Breve delay para evitar flicker
}