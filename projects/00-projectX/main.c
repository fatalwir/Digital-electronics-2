/***********************************************************************
 * 
 * Analog-to-digital conversion with displaying result on LCD and 
 * transmitting via UART.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2019 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>         // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "gpio.h"
#include <util/delay.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

/* Variables ---------------------------------------------------------*/
/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/* Main --------------------------------------------------------------*/
/* Read ADC result and transmit via UART. */
int main(void)
{
    lcd_init(LCD_DISP_ON);

    uint8_t customChars[] = {
        //Arrow up
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        0b00100,
        0b01110,
        0b01110,
        0b11111,
        //Arrow down
        0b11111,
        0b01110,
        0b01110,
        0b00100,
        0b00000,
        0b00000,
        0b00000,
        0b00000,
        //BUT logo
	    0b00000,
        0b11000,
        0b00111,
        0b00110,
        0b00100,
        0b00100,
        0b00000,
        0b00000
    };

    lcd_command(1<<LCD_CGRAM);
    int i;
    for (i=0; i<24; i++) {
        lcd_data(customChars[i]);
    }

    int a;
    // LCD display
    for (a = 1; a == 1; a = a + 1){
    lcd_clrscr();
    lcd_puts("LCD test ... OK");
    _delay_ms(1000);
    lcd_clrscr();
    lcd_puts("Initializing ...");
    lcd_gotoxy(0,1);
    lcd_puts("VUT FEKT 2019  ");
    lcd_putc(2);
    _delay_ms(2000);
    lcd_clrscr();
    }

    //ADC
    /* ADC0
     * TODO: Configure ADC reference, clock source, enable ADC module, 
     *       and enable conversion complete interrupt */
    ADMUX |= _BV(REFS0);
    ADCSRA |= _BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0)|_BV(ADIE);


    //Timers
    /* Timer0
     * TODO: Configure Timer0 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM0, TIM_PRESC_64);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);

     /* Timer1
     * TODO: Configure Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put string to ringbuffer for transmitting via UART.
    uart_puts("UART testing\r\n");

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/* Interrupts --------------------------------------------------------*/
/* Timer0 overflow interrupt routine.
 * Start ADC conversion. */

ISR(TIMER0_OVF_vect)
{
    
}
ISR(TIMER1_OVF_vect)
{
    ADCSRA |= _BV(ADSC);
}

/* -------------------------------------------------------------------*/
/* ADC complete interrupt routine.
 * Update UART transmiter. */
ISR(ADC_vect)
{
    //uint16_t val = ADC;
    //char num[4];
    //uart_puts("Value: \r\n");
    //uart_puts(itoa(val, num, 10));
    
    uint16_t val = ADC;
    char num[4];

    lcd_gotoxy(0, 0);
    lcd_puts("Voltage: ");
    lcd_puts("    ");
    lcd_gotoxy(9, 0);
    lcd_puts(itoa(val, num, 10)); //Write voltage value
    //uart_puts("Voltage: ");
    uart_puts(itoa(val, num, 10));
    uart_puts("\r\n");

    lcd_gotoxy(0, 1);
    lcd_puts("Current: ");
    lcd_puts("    ");
    lcd_gotoxy(9, 1);
    lcd_puts(itoa(val, num, 10)); //Write current value 
    //uart_puts("Current: ");
    uart_puts(itoa(val, num, 10));
    uart_puts("\r\n");
    uart_puts("\r\n");
 
}
