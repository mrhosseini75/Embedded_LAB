# Embedded_LAB

_The first assignment of the Embedded system in RobEng at Unige_

Device 
===============

**dsPIC30F4011 Enhanced Flash 16-bit Digital Signal Controller**

<p align="center">
<img width="500" src="https://user-images.githubusercontent.com/80394968/201785505-84ed5f1d-2dfd-40be-8d2e-e7f4d976f9d3.png" alt="500">
</p>
</p>
<p align="center">
dspic30f4011 kit
</p>

| Parametrics                   |
| -------------   ------------- |
|CPU Type	        dsPIC® DSC    |
|CPU Speed (MHz)    	30        |
|Program Memory Size(KB)	48    |
|Multiple Flash Panels	False   |  
|Temp. Range Min.(C°)	-40       |
|Temp. Range Max.(C°)	125       |
|Operation Voltage Min.(V)	2.5 |
|Operation Voltage Max.(V)	5.5 |
|Pin Count	40                  |
|Low Power	No                  |
|Number of Comparators	0       |
|Number of ADCs	1               |
|ADC Channels	9                 |
|Max ADC Resolution (bits)	10  |
|Max ADC Sampling Rate(ksps)1000|
|Number of DACs	0               |
|DAC outputs	0                 |
|Max DAC Resolution (bits)	0   |
|Hardware RTCC	No              |
|Motor Control PWM Channels	6   |
|SMPS PWM Channels	0           |
|Number of PWM Time Bases	2     |
|Output Compare Channels	4     |
|Number of CAN Modules	1       |
|Type of CAN module	CAN         |
|Crypto Engine	No              |
|Quadrature Encoder Interface (QEI)	1|
|Segment LCD	0                      |
|LCD/Graphics Interface	No           |
|Configurable Logic Cell Modules (CLC /CCL)	0|
|Peripheral Pin Select (PPS)/Pin Muxing	No   |


Requests
===============

1. Simulate an algorithm that needs 7 ms for its execution, and needs to work at 100 Hz.
2. Read characters from UART and display the characters received on the first row of the LCD.
3. When the end of the row has been reached, clear the first row and start writing again from the first row first column
4. Whenever a CR ’nr’ or LF ’nn’ character is received, clear the first row
5. On the second row, write ”Char Recv: XXX”, where XXX is the number of characters received from the UART2. (use sprintf(buffer, “%d”, value) to convert an integer to a string to be displayed
6. Whenever button S5 is pressed, send the current number of chars received to UART2
7. Whenever button S6 is pressed, clear the first row and reset the characters received counter


Autors and contact
===================
- Ali Yousefi - <aliy98@outlook.com>

- Mohammad Reza Haji Hosseini - <mrhhosseini75@gmail.com>
