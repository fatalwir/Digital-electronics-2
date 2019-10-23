/***********************************************************************
 * Title:    Seven-segment display library
 * Author:   Tomas Fryza, Brno University of Technology, Czechia
 * Software: avr-gcc, tested with avr-gcc 4.9.2
 * Hardware: Any AVR
 *
 * MIT License
 *
 * Copyright (c) 2019 Tomas Fryza
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>
#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"
#include "segment.h"

/* Define ------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
/* Active low digit 0 to 9 */
const uint8_t segment_digit[] = {
   //DPgfedcba
    0b11000000,      // Digit 0
    0b11111001,      // Digit 1
    0b10100100,      // Digit 2
    0b10110000,      // Digit 3
    0b10011001,      // Digit 4
    0b10010010,      // Digit 5
    0b10000010,      // Digit 6
    0b11111000,      // Digit 7
    0b10000000,      // Digit 8
    0b10010000};     // Digit 9

const uint8_t segment_digit_dp[] = {
   //DPgfedcba
    0b01000000,      // Digit 0
    0b01111001,      // Digit 1
    0b00100100,      // Digit 2
    0b00110000,      // Digit 3
    0b00011001,      // Digit 4
    0b00010010,      // Digit 5
    0b00000010,      // Digit 6
    0b01111000,      // Digit 7
    0b00000000,      // Digit 8
    0b00010000};     // Digit 9

/* Active high position 0 to 3 */
const uint8_t segment_position[] = {
    0b00001000,   // Position 0
    0b00000100,   // Position 1
    0b00000010,   // Position 2
    0b00000001};  // Position 3

uint8_t disp_digits[] = {0, 0, 0, 0};
volatile uint8_t actual_digit = 0; 

/* Functions ---------------------------------------------------------*/
void SEG_init(){
    GPIOConfigOutput(&DDRB, SEGMENT_DATA);
    GPIOConfigOutput(&DDRD, SEGMENT_CLK);
    GPIOConfigOutput(&DDRD, SEGMENT_LATCH);
    sei();
    TIM_config_prescaler(TIM0, TIM_PRESC_256);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);
}

void SEG_putc(uint8_t digit,
              uint8_t position)
{
    uint8_t i;

    /* Read values from look-up tables */
    if(digit<10) digit = segment_digit[digit];
    else digit = segment_digit_dp[digit%10];

    position = segment_position[position];

    /* Put 1st byte to serial data */
    for (i = 0; i < 8; i++) {
        GPIOWrite(&PORTB, SEGMENT_DATA, (digit>>(7-i))&_BV(0));
        SEG_toggle_clk();
    }
    /* Put 2nd byte to serial data */
    for (i = 0; i < 8; i++) {
        GPIOWrite(&PORTB, SEGMENT_DATA, (position>>(7-i))&_BV(0));
        SEG_toggle_clk();
    }

    GPIOWrite(&PORTD, SEGMENT_LATCH, 1);
    _delay_us(1);
    GPIOWrite(&PORTD, SEGMENT_LATCH, 0);
}

void SEG_disp_number(uint16_t num)
{
    disp_digits[0] = num%10;
    disp_digits[1] = (num%100)/10;
    disp_digits[2] = (num%1000)/100;
    disp_digits[3] = (num%10000)/1000;
}

void SEG_toggle_clk(void)
{
    GPIOWrite(&PORTD, SEGMENT_CLK, 1);
    _delay_us(1);
    GPIOWrite(&PORTD, SEGMENT_CLK, 0);
    _delay_us(1);
}

ISR(TIMER0_OVF_vect) {
    SEG_putc(disp_digits[actual_digit], actual_digit);
    actual_digit++;
    actual_digit = actual_digit%4;
}