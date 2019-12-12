
# Project - Java application

## Description
Raw data received from Arduino via UART are processed (Volts calculated from raw data and averaged). Then they are displayed in a window and sent back to Arduino which displays them on LCD. There is also option to save not averaged values to CSV file for further usage.

### Settings
  - Baud rate - needs to be set to same value as Arduino
  - Avg. fact. - number of samples fora averaging before displaying them
  - Ref. const. - a constant for recalculation raw data, calculation formula located in [AVR code description](/Digital-electronics-2/projects/00-projectX)
  - Log to CSV - the application logs data to log.csv file in actual folder (from which the program was started) when checked
