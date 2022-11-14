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

Guideline
============

The following power-up sequence should be observed by the user’s application firmware when writing characters to the LCD:

1. After any reset operation wait _1000_ milliseconds to allow the LCD to begin normal operation. The cursor on the LCD will be positioned at the top row on the
left-most column.
2. Configure SPI1 module on your _dsPIC30F_ device to operate in 8-bit Master mode. The serial clock may be set for any frequency up to 1 MHz.
3. To write an **ASCII** character to the LCD at the location pointed to by the cursor, load the SPIBUF register with the ASCII character byte.
4. After the character is displayed on the LCD, the cursor is automatically relocated to the next position on the LCD.
5. To reposition the cursor to another column on any of the two rows, write the address of the desired location to the SPIBUF register. Addresses in the first row
of the LCD range from _0x80_ to _0x8F_, while addresses on the second row range from _0xC0_ through _0xCF_.
6. After 16 characters are written to the first row on the LCD, it is necessary for the user’s application to write the address _0xC0_ of the second row to the SPIBUF in order to roll the cursor over to the second row.
7. The user application must wait for a minimum of (8 bits / SPI Frequency) between writing two successive characters or addresses.

Autors and contact
===================
- __Ali Yousefi__  *email: <aliy98@outlook.com>

- __Mohammad Reza Haji Hosseini__ *email: - <mrhhosseini75@gmail.com>
