/*
 * ---------------------------------------------------------------------
 * Authors:     Ondrej Kolar, Vojtech Herbrych
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2019-10-169
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 */
 
#include "timer.h"


void TIM_config_prescaler(uint8_t  timer_name ,uint8_t  presc_val) {
    switch(timer_name) {
        case TIM0:
            switch(presc_val) {
                case TIM_PRESC_1:
                    TCCR0B &= ~_BV(CS02) & ~_BV(CS01);
                    TCCR0B |=  _BV(CS00);
                    return;
                case TIM_PRESC_8:
                    TCCR0B &= ~_BV(CS00) & ~_BV(CS02);
                    TCCR0B |=  _BV(CS01);
                    return;
                case TIM_PRESC_64:
                    TCCR0B &= ~_BV(CS02);
                    TCCR0B |=  _BV(CS01) |  _BV(CS00);
                    return;
                case TIM_PRESC_256:
                    TCCR0B &= ~_BV(CS00) & ~_BV(CS01);
                    TCCR0B |=  _BV(CS02);
                    return;
                case TIM_PRESC_1024:
                    TCCR0B &= ~_BV(CS01);
                    TCCR0B |=  _BV(CS00) |  _BV(CS02);
                    return;
                default:
                    return;
            }
            return;

        case TIM1:
            switch(presc_val) {
                case TIM_PRESC_1:
                    TCCR1B &= ~_BV(CS12) & ~_BV(CS11);
                    TCCR1B |=  _BV(CS10);
                    return;
                case TIM_PRESC_8:
                    TCCR1B &= ~_BV(CS10) & ~_BV(CS12);
                    TCCR1B |=  _BV(CS11);
                    return;
                case TIM_PRESC_64:
                    TCCR1B &= ~_BV(CS12);
                    TCCR1B |=  _BV(CS11) |  _BV(CS10);
                    return;
                case TIM_PRESC_256:
                    TCCR1B &= ~_BV(CS10) & ~_BV(CS11);
                    TCCR1B |=  _BV(CS12);
                    return;
                case TIM_PRESC_1024:
                    TCCR1B &= ~_BV(CS11);
                    TCCR1B |=  _BV(CS10) |  _BV(CS12);
                    return;
                default:
                    return;
            }
            return;
        
        case TIM2:
            switch(presc_val) {
                case TIM_PRESC_1:
                    TCCR2B &= ~_BV(CS22) & ~_BV(CS21);
                    TCCR2B |=  _BV(CS20);
                    return;
                case TIM_PRESC_8:
                    TCCR2B &= ~_BV(CS22) & ~_BV(CS20);
                    TCCR2B |=  _BV(CS21);
                    return;
                case TIM_PRESC_32:
                    TCCR2B &= ~_BV(CS22);
                    TCCR2B |=  _BV(CS21) |  _BV(CS20);
                    return;
                case TIM_PRESC_64:
                    TCCR2B &= ~_BV(CS20) & ~_BV(CS21);
                    TCCR2B |=  _BV(CS22);
                    return;
                case TIM_PRESC_128:
                    TCCR2B &= ~_BV(CS21);
                    TCCR2B |=  _BV(CS22) |  _BV(CS20);
                    return;
                case TIM_PRESC_256:
                    TCCR2B &= ~_BV(CS20);
                    TCCR2B |=  _BV(CS22) |  _BV(CS21);
                    return;
                case TIM_PRESC_1024:
                    TCCR2B |=  _BV(CS22) | _BV(CS21)| _BV(CS20);
                    return;
                default:
                    return;
            }
            return;

        default:
            return;
    }   

}


void TIM_config_interrupt(uint8_t  timer_name ,uint8_t  interr_mode) {
    switch(timer_name){
        case TIM0:
            switch(interr_mode){
                case TIM_OVERFLOW_DISABLE:
                    TIMSK0 &= ~_BV(TOIE0);
                    return;

                case TIM_OVERFLOW_ENABLE:
                    TIMSK0 |=  _BV(TOIE0);
                    return;

                default:
                    return;

            }
            return;

        case TIM1:
            switch (interr_mode){
                case TIM_OVERFLOW_DISABLE:
                    TIMSK1 &= ~_BV(TOIE1);
                    return;

                case TIM_OVERFLOW_ENABLE:
                    TIMSK1 |=  _BV(TOIE1);
                    return;

                default:
                    return;
            }
            return;
            
        case TIM2:
            switch (interr_mode){
                case TIM_OVERFLOW_DISABLE:
                    TIMSK2 &= ~_BV(TOIE2);
                    return;

                case TIM_OVERFLOW_ENABLE:
                    TIMSK2 |=  _BV(TOIE2);
                    return;

                default:
                    return;
            }
            return;
            
        default:
            return;
    }   

}