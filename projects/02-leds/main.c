/*
 * ---------------------------------------------------------------------
 * Authors:     Ondrej Kolar, Vojtech Herbrych
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2019-10-02
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_PIN_1       PB5
#define LED_PIN_2       PB0
#define BTN_PIN         PB1
#define DEBOUNCE_DELAY  50

/* Variables ---------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/
void swapLeds();

/* Functions ---------------------------------------------------------*/
int main(void)
{
    DDRB |= _BV(LED_PIN_1);
    DDRB |= _BV(LED_PIN_2);
    DDRB &= ~_BV(BTN_PIN);
    PORTB |= _BV(BTN_PIN);
    PORTB &= ~_BV(LED_PIN_1);

    while (true)
    {
        if(bit_is_set(PINB , BTN_PIN)) {
          _delay_ms(DEBOUNCE_DELAY);
          loop_until_bit_is_clear(PINB , BTN_PIN);
          swapLeds();
        }
    }

    return (0);
}

void swapLeds() {
    PORTB ^= _BV(LED_PIN_1);
    PORTB ^= _BV(LED_PIN_2);
}