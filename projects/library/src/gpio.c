/*
 * ---------------------------------------------------------------------
 * Authors:     Ondrej Kolar, Vojtech Herbrych
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2019-10-09
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 */
 
#include "gpio.h"


void GPIOConfigOutput(volatile uint8_t *regName, uint8_t pinNum){
    *regName |= _BV(pinNum);
}

void GPIOConfigInputNoPull(volatile uint8_t *regName, uint8_t pinNum){
    *regName &= ~_BV(pinNum);
    *(regName+1) &= ~_BV(pinNum);
}

void GPIOConfigInputPullup(volatile uint8_t *regName, uint8_t pinNum){
    *regName &= ~_BV(pinNum);
    *(regName+1) |= _BV(pinNum);
}

void GPIOWrite(volatile uint8_t *regName, uint8_t pinNum, uint8_t pinVal){
    if(pinVal != 0){
        *regName |= _BV(pinNum);
    }else {
        *regName &= ~_BV(pinNum);
    }
}

void GPIOToggle(volatile uint8_t *regName, uint8_t pinNum){
    *regName ^= _BV(pinNum);
}

uint8_t GPIORead(volatile uint8_t *regName, uint8_t pinNum){
    return (((*regName)>>(pinNum))&1);
}