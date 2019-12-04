/*
 * main.c
 *
 *       Created on:  Nov 21, 2019
 *  Last Updated on:  Dec 04, 2019
 *          Authors:  Ondrej Kolar
 *                    Vojtech Herbrych
 *
 * Copyright (c) 2019 Ondrej Kolar, Vojtech Herbrych
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
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"

/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 76800
#define NO_MEAS_MODE   0
#define U_MEAS_MODE    1
#define I_MEAS_MODE    2

/* Variables ---------------------------------------------------------*/
uint8_t  measuring = U_MEAS_MODE;   // Current measuring mode
uint16_t uval = 0;  // Raw voltage value
uint16_t ival = 0;  // Raw current value
uint8_t  cnt = 0;   // Counter for discarding invalid ADC conversions
char buffer[3];     // Outgoing UART buffer

/* Function prototypes -----------------------------------------------*/
void startConversion(uint8_t pin);
void sendUART(void);

/* Main --------------------------------------------------------------*/
int main(void)
{
    // Internal 1,1 V ADC reference selection
    ADMUX |= _BV(REFS0)|_BV(REFS1);
    // Enabling ADC, interrupt, select prescaling by 128 (f=125kHz)
    ADCSRA |= _BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0)|_BV(ADIE);

    // Init LCD library
    lcd_init(LCD_DISP_ON);

    // Timer2 - 0.5 ms
    TIM_config_prescaler(TIM2, TIM_PRESC_32);
    TIM_config_interrupt(TIM2, TIM_OVERFLOW_ENABLE);

    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop - reading UART
    while (1) {
        uint16_t in = uart_getc();
        // If byte is received
        if(in>>8 == 0) {
            // If control byte is received
            if((uint8_t)in%256 > 127) {
                // If clear bit is set
                if((in>>6)%2 == 1) {
                    lcd_clrscr();
                }
                // Set received cursor position
                lcd_gotoxy(in%16, ((in>>4)%4));
            } else if((uint8_t)in%256 == 0) {
                // Ending byte received
            } else {
                lcd_putc((char)(in%256));
            }
        }
    }

    // Will never reach here
    return (0);
}

/* Interrupts --------------------------------------------------------*/
// Timer2 overflow interrupt routine.
ISR(TIMER2_OVF_vect)
{
    startConversion(U_MEAS_MODE);
    measuring = U_MEAS_MODE;
}

// ADC complete interrupt routine
ISR(ADC_vect)
{   
    switch(measuring){
        case U_MEAS_MODE: 
            if (cnt == 0){
                startConversion(U_MEAS_MODE);
                measuring = U_MEAS_MODE;
            }
            if (cnt == 1){
                uval = ADC;
                startConversion(I_MEAS_MODE);
                measuring = I_MEAS_MODE;
            }
            cnt++;
            break;
        case I_MEAS_MODE:
            if (cnt == 2){
                startConversion(I_MEAS_MODE);
                measuring = I_MEAS_MODE;
                cnt++;
            }
            if (cnt == 3){
                ival = ADC;
                sendUART();
                measuring = NO_MEAS_MODE;
                cnt = 0;
            }
            break;
        case NO_MEAS_MODE:
            break;
        default:
            measuring = NO_MEAS_MODE;
            break;
    }
 
}

/* Functions ---------------------------------------------------------*/
void startConversion(uint8_t mode) {
    switch(mode) {
        case U_MEAS_MODE:
            // Select pin PA1
            ADMUX |= _BV(MUX0);
            ADMUX &= ~(_BV(MUX1));
            break;
        case I_MEAS_MODE:
            // Select pin PA2
            ADMUX |= _BV(MUX1);
            ADMUX &= ~(_BV(MUX0));
            break;
        default:
            return;
    }
    // Start conversion
    ADCSRA |= _BV(ADSC);
}

void sendUART(void) {
    buffer[1] = uval%128;
    buffer[2] = ival%128;
    buffer[0] = 0;
    buffer[0] = 1<<7 | (uval>>7)<<3 | ival>>7;

    uart_putc(buffer[0]);
    uart_putc(buffer[1]);
    uart_putc(buffer[2]);
}