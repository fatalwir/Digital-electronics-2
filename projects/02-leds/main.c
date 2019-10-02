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

/* Macro defines -----------------------------------------------------*/
#define set_bit(aPORT, aPIN)   aPORT |= _BV(aPIN)
#define clear_bit(aPORT, aPIN)   aPORT &= ~_BV(aPIN)

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_PIN_1       PB5
#define LED_PIN_2       PB0
#define BTN_PIN         PB1
#define DEBOUNCE_DELAY  100

/* Variables ---------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/
void swapLeds();

/* Functions ---------------------------------------------------------*/
int main(void)
{
//  DDRB |= _BV(LED_PIN_1);   //First LED pin set as output
    set_bit(DDRB, LED_PIN_1);
//  DDRB |= _BV(LED_PIN_2);   //Second LED pin set as output
    set_bit(DDRB, LED_PIN_2);

//  DDRB &= ~_BV(BTN_PIN);    //Button pin set as input
    clear_bit(DDRB, BTN_PIN);
//  PORTB |= _BV(BTN_PIN);    //Activate button pin pull-up 
    set_bit(PORTB, BTN_PIN);

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