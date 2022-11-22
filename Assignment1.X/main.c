
// DSPIC30F4011 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FPR = XT                 // Primary Oscillator Mode (XT)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <p30F4011.h>
#include <stdio.h>

#define BUFFER_SIZE 60
#define ROW_SIZE 16
#define FIRST_ROW 1
#define SECOND_ROW 2
#define TIMER1 1
#define TIMER2 2
#define TIMER3 3

int cursor_counter = 0;             // global variable for cursor position
int character_counter = 0;          // global variable for number of characters received from UART2
char str[5];                        // global char variable for using in sprintf function to print characte counter in LCD
char value, prev_value;             // global char variables for current and previous values received from UART

// pre-definition of functions
int choose_prescaler(int ms, int* pr, int* tckps);
int tmr_setup_period(int timer, int ms);
int tmr_wait_period(int timer);
int tmr_wait_ms(int timer, int ms);
void put_char_SPI(char c);
void move_cursor_LCD(int row, int column);
void write_string_LCD(char* str, int max);
void clear_first_row_LCD();
void algorithm();

// defintion of circular buffer as a struct 
typedef struct{
char buffer[BUFFER_SIZE];
int readIndex;
int writeIndex;
}CircularBuffer;

volatile CircularBuffer cb;         // global instance of circular buffer structure

// function for writing into the circular buffer to send characters to UART
void write_buffer(volatile CircularBuffer* cb, char value){
    cb->buffer[cb->writeIndex] = value;
    cb->writeIndex++;
    if (cb->writeIndex == BUFFER_SIZE)
    cb->writeIndex = 0;
}

// function for reading from the circular buffer to receive characters from UART
int read_buffer(volatile CircularBuffer* cb, char* value){
    if (cb->readIndex == cb->writeIndex)
        return 0;
    *value = cb->buffer[cb->readIndex];
    cb->readIndex++;
    if (cb->readIndex == BUFFER_SIZE)
        cb->readIndex = 0;
    return 1;
}

// UART2 ISR 
void __attribute__((__interrupt__,__auto_psv__)) _U2RXInterrupt(){
    IFS1bits.U2RXIF = 0;             // reset UART2 interrupt flag
    char val = U2RXREG;              // save received value from UART in val
    write_buffer(&cb, val) ;         // write received value in circular buffer
    character_counter++;             // increase the number of received characters from UART
}

// External Interrupt 0 ISR (button S5)
void __attribute__((__interrupt__,__auto_psv__)) _INT0Interrupt(){
    IFS0bits.INT0IF = 0;             // reset INT0 interrupt flag
    U2TXREG = character_counter;     // transmit to UART2 the string
    IEC0bits.INT0IE = 0;             // disable INT0 interrupt 
    tmr_setup_period(TIMER3, 15);    // start timer3 to prevent bouncing 
}

// External Interrupt 1 ISR (button S6)
void __attribute__((__interrupt__,__auto_psv__)) _INT1Interrupt(){
    IFS1bits.INT1IF = 0;                              // reset INT1 interrupt flag
    clear_first_row_LCD();                            // clear first row of LCD
    cursor_counter = 0;                               // reset the cursor counter
    character_counter = 0;                            // reset the character counter
    move_cursor_LCD(SECOND_ROW, 11);                  // move cursor to the place where received chars number will be printed
    sprintf(str,"%d ",character_counter);             // change chars number value to string
    write_string_LCD(str, 5);                         // print chars number value (0) on LCD
    move_cursor_LCD(FIRST_ROW, character_counter);    // move back cursor to the first place in LCD
    IEC1bits.INT1IE = 0;                              // disable INT1 interrupt
    tmr_setup_period(TIMER3, 15);                     // start timer3 to prevent bouncing  
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(){
    IFS0bits.T3IF = 0;             // reset TIMER3 interrupt flag
    T3CONbits.TON = 0;             // stop timer 3
    IFS0bits.INT0IF = 0;           // reset INT0 interrupt flag
    IFS1bits.INT1IF = 0;           // reset INT1 interrupt flag
    IEC0bits.INT0IE = 1;           // enable interrupt for INT0 (button S5) 
    IEC1bits.INT1IE = 1;           // enable interrupt for INT1 (button S6)
}

int main(){
    // Circular Buffer initialization
    cb.writeIndex = 0;             // set initial writing index for circular buffer
    cb.readIndex = 0;              // set initial reading index for circular buffer

    // SPI initialization
    TRISBbits.TRISB1 = 0;          // set pin B1 as output 
    SPI1CONbits.MSTEN = 1;         // master mode
    SPI1CONbits.MODE16 = 0;        // 8-bit mode
    SPI1CONbits.PPRE = 3;          // 1:1 primary pre-scaler
    SPI1CONbits.SPRE = 6;          // 2:1 secondary pre-scaler
    SPI1STATbits.SPIEN = 1;        // enable SPI

    // UART initialization
    U2BRG = 11;                    // (7372800 / 4) / (16 * 9600) / 1
    U2MODEbits.UARTEN = 1;         // enable UART
    U2STAbits.UTXEN = 1;           // enable U1TX
    IEC1bits.U2RXIE = 1;

    // External interupts initialization
    TRISEbits.TRISE8 = 1;          // set button S5 as input                                
    TRISDbits.TRISD0 = 1;          // set button S6 as input   
    IEC0bits.INT0IE = 1;           // enable interrupt for INT0 (button S5)                 
    IEC1bits.INT1IE = 1;           // enable interrupt for INT1 (button S6)
    
    // TIMER3 interrupt initialization
    IEC0bits.T3IE = 1;             // enable interrupt for TIMER3
    
    tmr_setup_period(TIMER1, 10);  // define 100Hz working frequency for algorithm
    while(1){
        algorithm();
        tmr_wait_period(TIMER1);
    };
}

void algorithm(){
    int read = read_buffer(&cb, &value);               // read charecters from UART
    if (read == 1){                                    // condition for detecting char receive
        move_cursor_LCD(FIRST_ROW, cursor_counter);    // move cursor to desired column in first row
        // condition for detecting '\r', '\n' and end of row 
        if(((prev_value==92)&&(value=='r')) || ((prev_value==92)&&(value=='n')) || (cursor_counter==ROW_SIZE-1)){
            clear_first_row_LCD();                     // clear first row of LCD
            cursor_counter=0;                          // reset cursor position counter
        } else {
            put_char_SPI(value);                       // print received character
            cursor_counter++;                          // increase cursor position counter
        }
        move_cursor_LCD(SECOND_ROW, 0);                // move cursor to second row
        write_string_LCD("Char Recv:", 10);            // print "Char Recv:"
        move_cursor_LCD(SECOND_ROW, 11);               // move cursor after ":"
        sprintf(str,"%d",character_counter);           // change chars number value to string
        write_string_LCD(str, 5);                      // print chars number value (0) on LCD
        prev_value = value;                            // save current received char to consider it in the next loop to detect '\r' or '\n'
    }
    tmr_wait_ms(TIMER2, 7);                            // 7ms execution time
}

int choose_prescaler(int ms, int* pr, int* tckps) {
    long ticks = (7372800L / 4L) / 1000L * ms;
    long ticks_no_ps = ticks;
    if (ticks <= 65535) {
        *tckps = 0;
        *pr = ticks;
        return 0;
    }

    // pre-scaler 1:8
    ticks = ticks_no_ps / 8;
    if (ticks <= 65535) {
        *tckps = 1;
        *pr = ticks;
        return 0;
    }

    // pre-scaler 1:64
    ticks = ticks_no_ps / 64;
    if (ticks <= 65535) {
        *tckps = 2;
        *pr = ticks;
        return 0;
    }

    ticks = ticks_no_ps / 256;
    if (ticks <= 65535) {
        *tckps = 3;
        *pr = ticks;
        return 0;
    }

    return 1;
}

int tmr_setup_period(int timer, int ms) {
    switch (timer) {
        case TIMER1:
        {
            int pr, tckps;
            choose_prescaler(ms, &pr, &tckps);
            PR1 = pr;
            T1CONbits.TCKPS = tckps;
            T1CONbits.TCS = 0;
            T1CONbits.TGATE = 0;
            TMR1 = 0;
            T1CONbits.TON = 1;

            break;
        }
        case TIMER2:
        {
            int pr, tckps;
            choose_prescaler(ms, &pr, &tckps);
            PR2 = pr;
            T2CONbits.TCKPS = tckps;
            T2CONbits.TCS = 0;
            T2CONbits.TGATE = 0;
            TMR2 = 0;
            T2CONbits.TON = 1;

            break;
        }
        case TIMER3:
        {
            int pr, tckps;
            choose_prescaler(ms, &pr, &tckps);
            PR3 = pr;
            T3CONbits.TCKPS = tckps;
            T3CONbits.TCS = 0;
            T3CONbits.TGATE = 0;
            TMR3 = 0;
            T3CONbits.TON = 1;

            break;
        }
    }


    return 0;
}

int tmr_wait_period(int timer) {
    switch (timer) {
        case TIMER1:
        {
            if (IFS0bits.T1IF == 1) {
                IFS0bits.T1IF = 0;
                return 1;
            }

            while (IFS0bits.T1IF == 0);
            IFS0bits.T1IF = 0;
            break;
        }
        case TIMER2:
        {
            if (IFS0bits.T2IF == 1) {
                IFS0bits.T2IF = 0;
                return 1;
            }

            while (IFS0bits.T2IF == 0);
            IFS0bits.T2IF = 0;
            break;
        }
    }

    return 0;
}

int tmr_wait_ms(int timer, int ms) {
    switch (timer) {
        case TIMER1:
        {
            int pr, tckps;
            choose_prescaler(ms, &pr, &tckps);
            PR1 = pr;
            T1CONbits.TCKPS = tckps;
            T1CONbits.TCS = 0;
            T1CONbits.TGATE = 0;
            
            T1CONbits.TON = 0;
            IFS0bits.T1IF = 0;
            TMR1 = 0;
            T1CONbits.TON = 1;
            while (IFS0bits.T1IF == 0);
            IFS0bits.T1IF = 0;
            T1CONbits.TON = 0;
            break;
        }
        case TIMER2:
        {
            int pr, tckps;
            choose_prescaler(ms, &pr, &tckps);
            PR2 = pr;
            T2CONbits.TCKPS = tckps;
            T2CONbits.TCS = 0;
            T2CONbits.TGATE = 0;
            
            T2CONbits.TON = 0;
            IFS0bits.T2IF = 0;
            TMR2 = 0;
            T2CONbits.TON = 1;
            while (IFS0bits.T2IF == 0);
            IFS0bits.T2IF = 0;
            T2CONbits.TON = 0;
            break;
        }
    }
    
    return 0;
}

void put_char_SPI(char c){
    while(SPI1STATbits.SPITBF == 1);  // wait for previous transmissions to finish
    SPI1BUF = c;
}

void move_cursor_LCD(int row, int column){
    switch(row){
        case 1:{
            put_char_SPI(0x80 + column);
            return;
        }
        case 2:{
            put_char_SPI(0xC0 + column);
            return;
        }
    }
}

void write_string_LCD(char* str, int max){
    int i = 0;
    for ( i = 0; str[i] != '\0' && i < max; i++) {
        put_char_SPI(str[i]) ;
    }
}

void clear_first_row_LCD(){
    move_cursor_LCD(FIRST_ROW, 0);
    int i=0;
    for(i=0;i<16;i++){
        put_char_SPI(' ');
    }
}