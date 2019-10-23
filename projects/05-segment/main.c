/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-11
 * Last update: 2019-10-23
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description: Decimal counter with data display on 7-segment display.
 * TODO: Create and test functions to operate a 7-segment display using
 *       shift registers.
 * NOTE: Store segment.c and segment.h files in library/src and 
 *       library/inc folders.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"
#include "timer.h"
#include "segment.h"
#include <avr/interrupt.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_D1          PB5
#define BTN_S1          PC1 // PCINT 9
#define BTN_S2          PC2 // PCINT 10
#define BTN_S3          PC3 // PCINT 11

/* Variables ---------------------------------------------------------*/
uint16_t i = 0;
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on 7-segment display.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    /* D1 led */
    GPIOConfigOutput(&DDRB, LED_D1);

    /* Pin Change Interrupts 11:9 */
    PCICR |= _BV(PCIE1);
    PCMSK1 |= _BV(PCINT9)|_BV(PCINT10)|_BV(PCINT11);

    /* 7-segment display interface */
    SEG_init();

    /* Enable interrupts by setting the global interrupt mask */
    sei();

    /* Infinite loop */
    for (;;) {
        SEG_disp_number(i);
        _delay_ms(10);
        i++;
        i=i%10000;
    }

    return (0);
}

/**
 *  Brief: Pin Change Interrupt 11:9 routine. Toggle a LED.
 */
ISR(PCINT1_vect){
    GPIOToggle(&DDRB, LED_D1);
    _delay_ms(50);
}
