/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 * Edit:        Ondrej Kolar, Vojtech Herbrych
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-16
 * Last update: 2019-10-30
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Decimal counter with data output on LCD display.
 * 
 * Note:
 *    Modified version of Peter Fleury's LCD library with R/W pin 
 *    connected to GND. Newline symbol "\n" is not implemented, use
 *    lcd_gotoxy() function instead.
 */

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>             // itoa() function
#include "timer.h"
#include "lcd.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
/* Variables ---------------------------------------------------------*/
uint8_t n = 0;
char s[33];

/* Function prototypes -----------------------------------------------*/

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Shows decimal values ​​on LCD display.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    /* LCD display
     * TODO: See Peter Fleury's online manual for LCD library 
     * http://homepage.hispeed.ch/peterfleury/avr-software.html
     * Initialize display and test different types of cursor */
    lcd_init(LCD_DISP_ON);

    // Display string without auto linefeed
    //lcd_puts("LCD testing");

    // Display variable value in decimal, binary, and hexadecimal
    // uint8_t n = 86;
    // char s[33];
    
    // lcd_puts("Dec: ");
    // itoa (n, s, 10);
    // lcd_puts(s);
    // lcd_puts(" Hex: ");
    // itoa (n, s, 16);
    // lcd_puts(s);
    // lcd_gotoxy(0, 1);
    // lcd_puts("Bin: ");
    // itoa (n, s, 2);
    // lcd_puts(s);
    

    /* Timer1
     * Configure Timer1 clock source and enable overflow 
     * interrupt */
    TIM_config_prescaler(TIM1, TIM_PRESC_64);
    TIM_config_interrupt(TIM1, TIM_OVERFLOW_ENABLE);

    lcd_gotoxy(0, 0);
    lcd_puts("Counter: ");
    //lcd_gotoxy(0, 1);
    //lcd_puts("0x");
    //lcd_gotoxy(6, 1);
    //lcd_puts("0b");

    /* Design at least two user characters and store them in 
     * the display memory */
    uint8_t customChars[] = {
        //Left half of a ring
        0b00011,
        0b00100,
        0b01000,
        0b01000,
        0b01000,
        0b01000,
        0b00100,
        0b00011,
        //Right half of a ring
        0b11000,
        0b00100,
        0b00010,
        0b00010,
        0b00010,
        0b00010,
        0b00100,
        0b11000,
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

    lcd_gotoxy(13, 0);
    lcd_putc(2);
    lcd_putc(0);
    lcd_putc(1);

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    for (;;) {
    }

    // Will never reach this
    return (0);
}

/**
 *  Brief: Timer1 overflow interrupt routine. Increment counter value.
 */
ISR(TIMER1_OVF_vect){
    if(n==0) {
        lcd_gotoxy(10, 0);
        lcd_puts("  ");
        // lcd_gotoxy(3, 1);
        // lcd_puts(" ");
        // lcd_gotoxy(9, 1);
        // lcd_puts("         ");
        lcd_gotoxy(0, 1);
        lcd_puts("                ");
        lcd_gotoxy(0, 1);
    }
    itoa (n, s, 10);
    lcd_gotoxy(9, 0);
    lcd_puts(s);

    // itoa (n, s, 16);
    // lcd_gotoxy(2, 1);
    // lcd_puts(s);

    // itoa (n, s, 2);
    // lcd_gotoxy(8, 1);
    // lcd_puts(s);

    if((n%16)==1) {
        lcd_gotoxy(n>>4, 1);
        lcd_putc(0xFF);
    }
    n++;
}
