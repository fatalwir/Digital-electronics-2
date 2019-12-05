
# Project - AVR program

## Description
Voltage is measured on A1 pin of Arduino UNO. Current must be measured as voltage drop on a shunt resistor connected between A2 and GND pins. Internal 1.1V reference is used by internal 10-bit ADC, therefore input voltages shouldn't exceed this value. For measuring bigger voltages adequate resistor divider is needed. Current range is defined by shunt resistor value. Raw binary data are then sent to PC over the UART which was chosen to run at baud rate 76800. This baud rate has low error rate with 16MHz crystal according to [https://www.robotroom.com/Asynchronous-Serial-Communication-2.html](https://www.robotroom.com/Asynchronous-Serial-Communication-2.html).

### Schematic
```
 U (Voltage input terminal)
 ○─┐                         
  ┌┴┐                        
  | |R₁                      
  └┬┘                            I (Current input terminal)
   ├────○ A1 (Arduino)           ○────┬────○ A2 (Arduino)
  ┌┴┐                                ┌┴┐
  | |R₂                              | |R₃
  └┬┘                                └┬┘
  ─┴─                                ─┴─
  GND                                GND
```
#### Choosing resistor values
Because of the Java application recalculation constants, it's necessary to choose input voltage range 10 times bigger than current range. For example 20 V and 2 A. Therefore you can calculate:
  - ```R₃ = Uref/Imax = 1.1V/2A = 0.55ohm``` (Let's choose 0.5ohm instead.)  

Then you can calculate constant which is needed in Java application later.
  - ```C = R₃*((2^n)-1)/Uref = 0.5ohm*1023/1.1V = 465```  

As a last step you have to design voltage divider by the following rule:
  - ```10*R₂/(R₁+R₂) = R₃```
  - For example: ```R₁ = 1kohm, R₂= 19kohm```


### UART protocol
The communication protocol between Arduino and PC was designed for easy decoding.

#### Arduino -> PC
  - First byte has MSB=1 for easy distinguishing that it's the first one. Other bytes have MSB=0. It also carries 3 upper bits of voltage value and also 3 upper bits of current value.
  - Second byte contains 7 lowest bits of voltage value.
  - Third byte contains 7 lowest bits of current value

So these bytes are sent like this: ```0b10uuuiii```, ```0b0uuuuuuu```, ```0b0iiiiiii```.

#### PC -> Arduino
First byte is a control one. Again, the MSB=1 for easy distinguishing that it's the control byte. It also contains information wether the LCD should be cleared or not and where a cursor should be placed.  
  
```0b1cyyxxxx```
  - c - Clear LCD bit (1 - clear LCD before putting new characters)
  - yy - Row in which the cursor should be placed
  - xxxx - Column in which the cursor should be placed

Then Arduino receives any character bytes. Thanks to the ASCII table, every character should have MSB=0 so neither of them couldn't be distinguished as a control byte.
