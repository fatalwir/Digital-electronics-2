
# Project - AVR program

## Features

#### Description
Voltage is measured on A1 pin of Arduino UNO. Current must be measured as voltage drop on a shunt resistor connected between A2 and GND pins. Internal 1.1V reference is used by ADC, therefore input voltages shouldn't exceed this value. For measuring bigger voltages adequate resistor divider is needed. Current range is defined by shunt resistor value.


Referencni napeti ADC: Internal 1,1 V
Vzorkovani: priblizne kazdou 1 ms

UART:
    Baud rate: 76800 (err. 0,2 % pro 16 MHz clock)
    (https://www.robotroom.com/Asynchronous-Serial-Communication-2.html)

    Arduino -> PC
        Prvni bajt: 0b10uuuiii
        Druhy bajt: 0b0uuuuuuu
        Treti bajt: 0b0iiiiiii

    PC -> Arduino
        Prvni bajt: 0b1cyyxxxx
        Dal. bajty: znaky
        Konc. bajt: 0x00

    * uuu uuuuuuu - 10bitova hodnota napeti
    * iii iiiiiii - 10bitova hodnota proudu
    * c - vymazani displeje pred zobrazenim textu (0/1 - ne/ano)
    * yy - na ktery radek text zapsat
    * xxxx - na kterou pozici na radku text zapsat
