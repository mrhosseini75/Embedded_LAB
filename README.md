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

Parametrics
------------

- _CPU Type:_	                dsPIC® DSC
- _Program Memory Size(KB):_	  48
- _Operation Voltage Min.(V):_	2.5
- _Operation Voltage Max.(V):_	5.5
- _Pin Count:_	                40
- _Segment LCD:_	              0
- _LCD/Graphics Interface:_  	No

Peripheral Features
---------------------

- High current sink/source I/O pins: 25 mA/25 mA
- Optionally pair up 16-bit timers into 32-bit timer modules
- 3-wire SPI™ modules (supports 4 Frame modes)
- I2C™ module supports Multi-Master/Slave mode and 7-bit/10-bit addressing
- Addressable UART modules with FIFO buffers
- 1 CAN module, 2.0B compliant

*Also you can use the data sheet in the repository to find more information about __dspic30f4011__

Requests
===============

1. Simulate an algorithm that needs __7 ms__ for its execution, and needs to work at __100 Hz__.
2. Read characters from UART and display the characters received on the first row of the LCD.
3. When the end of the row has been reached, clear the first row and start writing again from the first row first column
4. Whenever a _CR ’\r’_ or _LF ’\n’_ character is received, clear the first row
5. On the second row, write _”Char Recv: XXX”_, where _XXX_ is the number of characters received from the UART2. _(use sprintf(buffer, “%d”, value)_ to convert an integer to a string to be displayed
6. Whenever button __S5__ is pressed, send the current number of chars received to UART2
7. Whenever button __S6__ is pressed, clear the first row and reset the characters received counter


Autors and contact
===================
- __Ali Yousefi__  *email: <aliy98@outlook.com>

- __Mohammad Reza Haji Hosseini__ *email: - <mrhhosseini75@gmail.com>
