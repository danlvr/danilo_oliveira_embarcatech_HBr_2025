#ifndef LED_EMBUTIDO_H
#define LED_EMBUTIDO_H

/**
 * @brief Define o estado do LED integrado.
 * Esta função interage diretamente com a API de hardware de baixo nível.
 * É assumido que cyw43_arch_init() foi chamado com sucesso previamente.
 *
 * @param state 1 para ligar o LED, 0 para desligá-lo.
 */
void led_embutido_set(int state);

#endif // LED_EMBUTIDO_H