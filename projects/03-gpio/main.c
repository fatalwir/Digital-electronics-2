/*
 * ---------------------------------------------------------------------
 * Authors:     Ondrej Kolar, Vojtech Herbrych
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2019-10-02
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 */

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>
#include <stdbool.h>

#include <gpio.h>

/* Macro defines -----------------------------------------------------*/
#define set_bit(aPORT, aPIN)      aPORT |= _BV(aPIN)
#define clear_bit(aPORT, aPIN)    aPORT &= ~_BV(aPIN)

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_PIN_1       PB5
#define LED_PIN_2       PB0
#define BTN_PIN         PB1
#define DEBOUNCE_DELAY  200

/* Variables ---------------------------------------------------------*/

/* Functions ---------------------------------------------------------*/
int main(void){
    GPIOConfigOutput(&DDRB, LED_PIN_1);
    GPIOConfigOutput(&DDRB, LED_PIN_2);
    GPIOConfigInputPullup(&DDRB, BTN_PIN);


    while (true){
        if(GPIORead(&PINB , BTN_PIN)) {
            _delay_ms(DEBOUNCE_DELAY);
            while(GPIORead(&PINB , BTN_PIN));
            GPIOToggle(&PORTB, LED_PIN_1);
            GPIOToggle(&PORTB, LED_PIN_2);
        }
    }

    return (0);
}