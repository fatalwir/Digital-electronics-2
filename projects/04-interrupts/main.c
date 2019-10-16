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
#include <avr/interrupt.h>

#include <gpio.h>
#include <timer.h>

/* Macro defines -----------------------------------------------------*/
#define set_bit(aPORT, aPIN)      aPORT |= _BV(aPIN)
#define clear_bit(aPORT, aPIN)    aPORT &= ~_BV(aPIN)

/* Typedef -----------------------------------------------------------*/
/* Define ------------------------------------------------------------*/
#define LED_PIN_1       PB1

/* Variables ---------------------------------------------------------*/
uint16_t duty = 0;
bool way = false;

/* Functions ---------------------------------------------------------*/
int main(void){
    GPIOConfigOutput(&DDRB, LED_PIN_1);
    sei();
    TIM_config_prescaler(TIM0, TIM_PRESC_256);
    TIM_config_interrupt(TIM0, TIM_OVERFLOW_ENABLE);

    TCCR1A |= (1 << COM1A1);
    // set non-inverting mode
    TCCR1A |= (1 << WGM11) | (1 << WGM10);
    // set 10bit phase corrected PWM Mode
    TCCR1B |= (1 << CS11);

    while (true){
    }

    return (0);
}

ISR(TIMER0_OVF_vect) {
    if(!way){
        duty++;
        if(duty==65535)
            way=!way;
    }else {
        duty--;
        if(duty==0)
            way=!way;
    }
    OCR1A = duty;
    return;
}