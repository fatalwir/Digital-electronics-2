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
#include <stdio.h>   
#include <stdlib.h>      // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"
#include "gpio.h"
#include <util/delay.h>

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 115200
#define NO_MEAS_MODE  0
#define U_MEAS_MODE  1
#define I_MEAS_MODE  2

/* Variables ---------------------------------------------------------*/
uint8_t  measuring = U_MEAS_MODE;
uint16_t uval = 0;
uint16_t ival = 0;
uint8_t  cnt = 0;
uint8_t  a = 0;
//uint8_t i = 24;

char string[17];

char array[3];

// uint8_t first = 2;
// uint8_t uval_l = 0;
// uint16_t uval_h = 400;
// uint8_t ival_l = 6;
// uint16_t ival_h = 824;

char uart_string[24];

/* Function prototypes -----------------------------------------------*/
void startConversion(uint8_t pin);
void sendLCD(void);
void sendUART(char array);
/* Functions ---------------------------------------------------------*/
/* Main --------------------------------------------------------------*/
/* Read ADC result and transmit via UART. */
int main(void)
{
    /* External 5 V ADC reference selection
    ADMUX |= _BV(REFS0);
    ADMUX &= ~(_BV(REFS1));
    */

    // first = 2;
    // uval_h = 400;
    // ival_h = 830;
    // uval_l = 0;
    // ival_l = 6;

    // Internal 1,1 V ADC reference selection
    ADMUX |= _BV(REFS0)|_BV(REFS1);
    // Enabling ADC, interrupt, select prescaling by 128 (f=125kHz)
    ADCSRA |= _BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0)|_BV(ADIE);

    lcd_init(LCD_DISP_ON);

    // uint8_t customChars[] = {
    //     //BUT logo
	//     0b00000,
    //     0b11000,
    //     0b00111,
    //     0b00110,
    //     0b00100,
    //     0b00100,
    //     0b00000,
    //     0b00000
    // };

    // lcd_command(1<<LCD_CGRAM);

    // int i;

    // for (i=0; i<8; i++) {
    //     lcd_data(customChars[i]);
    // }

    // // LCD display
    // lcd_clrscr();
    // lcd_puts("LCD test ... OK");
    // _delay_ms(1000);
    // lcd_clrscr();
    // lcd_puts("Initializing ...");
    // lcd_gotoxy(0,1);
    // lcd_puts("VUT FEKT 2019  ");
    // lcd_putc(0);
    // _delay_ms(2000);
    // lcd_clrscr();
    

    //Timers
    /* Timer0
     * TODO: Timer0 clock source and enable overflow 
     *       interrupt */
    // TIM_config_prescaler(TIM0, TIM_PRESC_256);
    // TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);

    /* Timer1
     * TODO: Timer1 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    /* Timer2
     * TODO: Configure Timer2 clock source and enable overflow 
     *       interrupt */
    TIM_config_prescaler(TIM2, TIM_PRESC_32);
    TIM_config_interrupt(TIM2, TIM_OVERFLOW_ENABLE);

    // UART: asynchronous, 8-bit data, no parity, 1-bit stop
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));
    
    uart_puts("UART testing\r\n");

    
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    for (;;) {
    }
    // Will never reach thisccept instructions
    return (0);
}

/* Interrupts --------------------------------------------------------*/
/* Timer1 overflow interrupt routine.
 * Start LCD write and UART transmitting. */

// ISR(TIMER0_OVF_vect)
// {
//     if (measuring == NO_MEAS_MODE) {
//     //     cnt = 0;
//         sendUART();
//     }
// }

ISR(TIMER1_OVF_vect)
{
    sendLCD();
    //sendUART();
}

/* Timer2 overflow interrupt routine.
 * Start ADC conversion. */
ISR(TIMER2_OVF_vect)
{
    startConversion(U_MEAS_MODE);
    measuring = U_MEAS_MODE;
}

/* -------------------------------------------------------------------*/
/* ADC complete interrupt routine.
 * Update UART transmiter. */
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
                measuring = NO_MEAS_MODE;
                cnt = 0;
            }
            break;
        case NO_MEAS_MODE:
            // array[0] = uval*128;
            // lcd_gotoxy(0,1);
            // lcd_putc(array[0]);
            // array[1] = uval%128;
            // lcd_gotoxy(5,1);
            // lcd_putc(array[1]);
            // array[2] = ival%128;
            // lcd_gotoxy(10,1);
            // lcd_putc(array[2]);
            //sendUART(array[0]);
            break;
        default:
            break;
    }
 
}

void startConversion(uint8_t mode) {
    switch(mode) {
        case U_MEAS_MODE:
            // Select pin PA1
            ADMUX |= _BV(MUX0);
            ADMUX &= ~(_BV(MUX1));//&_BV(MUX2)&_BV(MUX3));
            break;
        case I_MEAS_MODE:
            // Select pin PA2
            ADMUX |= _BV(MUX1);
            ADMUX &= ~(_BV(MUX0));//&_BV(MUX2)&_BV(MUX3));
            break;
        default:
            return;
    }
    ADCSRA |= _BV(ADSC);    // Start conversion
}

void sendLCD(void) {
    lcd_clrscr();
    itoa(uval, string, 10);
    lcd_gotoxy(0, 0);
    lcd_puts("U=");
    lcd_puts(string);
    lcd_puts("V;");
    itoa(ival, string, 10);
    lcd_gotoxy(10, 0);
    lcd_puts("I=");
    lcd_puts(string);
    lcd_puts("A");


    array[0] = 2&uval/128&ival/128; //Zaměnit s něčím použitelným!!!
    lcd_gotoxy(0,1);
    lcd_putc(array[0]);
    array[1] = uval%128;
    lcd_gotoxy(5,1);
    lcd_putc(array[1]);
    array[2] = ival%128;
    lcd_gotoxy(10,1);
    lcd_putc(array[2]);
    return;
}

void sendUART(char array) {
    uart_putc(array);
    // uart_putc(array[0]);
    // uart_putc(array[1]);
    // uart_putc(array[2]);
    return;
}

// for (i; i<3; i++) {
    //     if (uart_string[i] == '\0') {
    //         uart_string[i] = ' ';
    //     }
    // }

    // itoa(first, uart_string, 16);
    // itoa(uval_l, &uart_string[1], 16);
    // itoa(ival_l, &uart_string[2], 16);
    // itoa(uval_h, &uart_string[3], 16);
    // itoa(ival_h, &uart_string[4], 16);