/***********************************************************************
 * 
 * Implementation of LFSR-based (Linear Feedback Shift Register) 
 * pseudo-random generator in AVR assembly.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2019 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "gpio.h"

/* Typedef -----------------------------------------------------------*/

/* Define ------------------------------------------------------------*/
#define OUT_PIN         PB5
/* Variables ---------------------------------------------------------*/
uint8_t n = 0;

/* Function prototypes -----------------------------------------------*/
extern uint8_t burst_asm(uint8_t num);

/* Functions ---------------------------------------------------------*/

/* Main --------------------------------------------------------------*/
/* Generate a sequence of LFSR preudo-random values using 4- and 8-bit
 * structure. */
int main(void)
{
    GPIOConfigOutput(&DDRB, OUT_PIN);

    // Timer1
    TIM_config_prescaler(TIM1, TIM_PRESC_8);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    sei();

    for (;;) {
    }

    return (0);
}

/* Interrupts --------------------------------------------------------*/
/* Timer1 overflow interrupt routine.
 * Update state of the FSM. */
ISR(TIMER1_OVF_vect)
{
    burst_asm(n&0x0F);
    n++;
}
