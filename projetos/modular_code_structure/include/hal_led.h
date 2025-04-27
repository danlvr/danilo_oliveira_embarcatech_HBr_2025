#ifndef HAL_LED_H
#define HAL_LED_H

#include <stdbool.h> // Include para o tipo bool

/**
 * @brief Inicializa o módulo LED HAL.
 */
void hal_led_init(void);

/**
 * @brief Alterna o estado atual do LED integrado.
 */
void hal_led_toggle(void);

/**
 * @brief Liga o LED integrado.
 */
void hal_led_on(void);

/**
 * @brief Desliga o LED integrado.
 */
void hal_led_off(void);


#endif // HAL_LED_H