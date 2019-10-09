/*
 * ---------------------------------------------------------------------
 * Authors:     Ondrej Kolar, Vojtech Herbrych
 *              Dept. of Radio Electronics, Brno Univ. of Technology
 * Created:     2019-10-09
 * Platform:    ATmega328P, AVR 8-bit Toolchain 3.6.2
 * ---------------------------------------------------------------------
 */

#ifndef GPIO_H_INCLUDED
    #define GPIO_H_INCLUDED


#include <avr/io.h>

void GPIOConfigOutput(volatile uint8_t *regName, uint8_t pinNum);
void GPIOConfigInputNoPull(volatile uint8_t *regName, uint8_t pinNum);
void GPIOConfigInputPullup(volatile uint8_t *regName, uint8_t pinNum);
void GPIOWrite(volatile uint8_t *regName, uint8_t pinNum, uint8_t pinVal);
void GPIOToggle(volatile uint8_t *regName, uint8_t pinNum);
uint8_t GPIORead(volatile uint8_t *regName, uint8_t pinNum);

#endif