/*
 * ---------------------------------------------------------------------
 * Author:      Tomas Fryza
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2018-10-23
 * Last update: 2019-11-01
 * Platform:    ATmega328P, 16 MHz, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 * Description:
 *    Analog-to-digital conversion with displaying result on LCD and 
 *    transmitting via UART.
 * 
 * Note:
 *    Peter Fleury's UART library.
 */

/* Includes ----------------------------------------------------------*/
#include <stdlib.h>             // itoa() function
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include "uart.h"

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define UART_BAUD_RATE 9600

#define BTN_NOT 0
#define BTN_SEL 1
#define BTN_LEFT 2
#define BTN_DOWN 3
#define BTN_UP 4
#define BTN_RIGHT 5


/* Variables ---------------------------------------------------------*/
uint16_t last_btn_val = 0;
/* Function prototypes -----------------------------------------------*/
void button_handle(uint16_t val);

/* Functions ---------------------------------------------------------*/
/**
 *  Brief:  Main program. Read ADC result and transmit via UART.
 *  Input:  None
 *  Return: None
 */
int main(void)
{
    // LCD display
    lcd_init(LCD_DISP_ON);
    lcd_puts("LCD testing");

    /* ADC
     * TODO: Configure ADC reference, clock source, enable ADC module, 
     *       and enable conversion complete interrupt */
    ADMUX |= _BV(REFS0);
    ADCSRA |= _BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0)|_BV(ADIE);

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

/**
 *  Brief: Timer1 overflow interrupt routine. Start ADC conversion.
 */
ISR(TIMER1_OVF_vect)
{
    ADCSRA |= _BV(ADSC);
}

/**
 *  Brief: ADC complete interrupt routine. Update LCD and UART 
 *         transmiter.
 */
ISR(ADC_vect)
{
    uint16_t val = ADC;
    char num[4];
    lcd_gotoxy(0, 1);
    lcd_puts("                ");
    lcd_gotoxy(0, 1);
    lcd_puts(itoa(val, num, 10));
    button_handle(val);
}

void button_handle(uint16_t val) {
    if(val>830) {
        //nothing
        if(last_btn_val != BTN_NOT) {
            lcd_gotoxy(10, 1);
            lcd_puts("NOT   ");
            uart_puts("\033[97;40m");
            uart_puts("Nothing\r\n");
        }
        last_btn_val = BTN_NOT;
    }else if(val>530) {
        //select
        if(last_btn_val != BTN_SEL) {
            lcd_gotoxy(10, 1);
            lcd_puts("SELECT");
            uart_puts("\033[92;42m");
            uart_puts("Select\033[97;40m\r\n");
        }
        last_btn_val = BTN_SEL;
    }else if(val>330) {
        //left
        if(last_btn_val != BTN_LEFT) {
            lcd_gotoxy(10, 1);
            lcd_puts("LEFT  ");
            uart_puts("\033[91;41m");
            uart_puts("Left\033[97;40m\r\n");
        }
        last_btn_val = BTN_LEFT;
    }else if(val>180) {
        //down
        if(last_btn_val != BTN_DOWN) {
            lcd_gotoxy(10, 1);
            lcd_puts("DOWN  ");
            uart_puts("\033[96;46m");
            uart_puts("Down\033[97;40m\r\n");
        }
        last_btn_val = BTN_DOWN;
    }else if(val>50) {
        //up
        if(last_btn_val != BTN_UP) {
            lcd_gotoxy(10, 1);
            lcd_puts("UP    ");
            uart_puts("\033[95;45m");
            uart_puts("Up\033[97;40m\r\n");
        }
        last_btn_val = BTN_UP;
    }else {
        //right
        if(last_btn_val != BTN_RIGHT) {
            lcd_gotoxy(10, 1);
            lcd_puts("RIGHT ");
            uart_puts("\033[93;43m");
            uart_puts("Right\033[97;40m\r\n");
        }
        last_btn_val = BTN_RIGHT;
    }

}