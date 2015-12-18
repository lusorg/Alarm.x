/* 
 * File:   configBit.h
 * Author: cardias
 *
 * Created on September 18, 2015, 5:22 PM
 
 * Capacitor UPW0J472MHD
 * relay rs-5

    ----------------------------   
   -| 1                     40 |-     
RA0-| 2 LED                 39 |-   
   -| 3                     38 |-   
   -| 4               SOUND 37 |-RB4    
   -| 5               LED_3 36 |-RB3   
   -| 6               LED_2 35 |-RB2   
   -| 7               LED_1 34 |-RB1   
   -| 8               LED_0 33 |-RB0   
   -| 9                     32 |-VDD    
   -| 10                    31 |-VSS    
VDD-| 11               SDO  30 |-RD7   
VSS-| 12               SCK  29 |-RD6   
   -| 13               SDI  28 |-RD5   
   -| 14               nSEL 27 |-RD4   
   -| 15                 RX 26 |-RC7   
RC1-| 16 LCD_LED         TX 25 |-RC6   
RC2-| 17 LCD_PIN_RS   SEN_1 24 |-RC5   
RC3-| 18 LCD_PIN_E    SEN_0 23 |-RC4   
RD0-| 19 LCD_D4      LCD_D7 22 |-RD3    
RD1-| 20 LCD_D5      LCD_D6 21 |-RD2
     --------------------------
     
/*CONTROLER MSG
 * 
 * code    | Definition
 *    1    | request status 
 *    2    | disable Alarm 
 *    3    | enable Alarm by request
 *    10   | Sensor OFF
 */

#define MSG_RQST_STAT 1
#define MSG_DSBL_ALRM 1
#define MSG_RQST_ALRM 1
#define MSG_ALRM_OFF  1

//Sensor MSG
// Alarm_state  | DEFINITION
//              |
//      1       |    Alarm clear 
//      2       |    Alarmed by local sensor
//      3       |    Alarmed by local sensor, sound timeout
//      4       |    Alarmed by request (request from controller)
//      5       |    Alarmed by request, sound timeout
//     10       |    Sensor OFF 
//

#define ST_ALRM_CLEAR    1
#define ST_ALRM_SENS     2
#define ST_ALRM_SENS_TMT (ST_ALRM_SENS + 1)
#define ST_ALRM_REQ       3   
#define ST_ALRM_REQ_TMT  (ST_ALRM_REQ + 1)
#define ST_ALRM_OFF       10 

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = OFF      // MCLR Pin Enable bit (RE3 input pin enabled; MCLR disabled)

// CONFIG4L
#pragma config STVREN = OFF     // Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
#pragma config LVP = OFF         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)




/*
 * 
 * TRIS register (data direction register)
 * PORT register (reads the levels on the pins of the device)
 * LAT register (output latch)
 */
#define LED_0 LATBbits.LATB0
#define LED_1 LATBbits.LATB1
#define LED_2 LATBbits.LATB2
#define LED_3 LATBbits.LATB3
#define LED   LATAbits.LATA0

#define SENS_0 PORTCbits.RC4

#define SOUND  LATBbits.LATB4

#define LCD_LED     LATCbits.LATC1
#define LCD_PIN_RS  LATCbits.LATC2
#define LCD_PIN_E   LATCbits.LATC3


//INPUTS
#define   SDO       PORTDbits.RD7

//OUTPUTS
#define   SCK       LATDbits.LATD6    
#define   SDI       LATDbits.LATD5    
#define   nSEL      LATDbits.LATD4

unsigned char RF_RXBUF[8];
#define UART_buf_size  32
unsigned char UART_buffer[UART_buf_size] = "--------------------------------";

void System_startup() {

    WDTCONbits.SWDTEN = 0; //turn off watch dog timer

    OSCCONbits.IRCF = 0b111; //Set to 8MHZ
    OSCTUNEbits.PLLEN = 0b1; //Enable PLL

    TRISAbits.TRISA0 = 0; // RA0 to output
    ADCON1bits.PCFG0 = 0b1; // set to ANALOG OFF
    ADCON1bits.PCFG1 = 0b1; // set to ANALOG OFF
    ADCON1bits.PCFG2 = 0b1; // set to ANALOG OFF
    ADCON1bits.PCFG3 = 0b1; // set to ANALOG OFF

    TRISBbits.TRISB0 = 0; // RB0 to output
    TRISBbits.TRISB1 = 0; // RB1 to output
    TRISBbits.TRISB2 = 0; // RB2 to output
    TRISBbits.TRISB3 = 0; // RB3 to output

    TRISCbits.TRISC4 = 1; //SENS_0 input

    // Alarm output 
    TRISBbits.TRISB4 = 0; // RA0 to output

    // Enable interrupt
    PIE1bits.RCIE = 0b1; // uart receive interupt
    INTCONbits.PEIE = 0b1; // periperial interrupt enable
    INTCONbits.GIE = 0b1; // Global interupt enable

    //Enable Timer0

    //T0CONbits.T0PS0 =1; //Prescaler is divide by 256
    //T0CONbits.T0PS1 =1;
    //T0CONbits.T0PS2 =1;

    //T0CONbits.PSA   = 0;      //Timer Clock Source is from Prescaler

    //T0CONbits.T0CS  = 0;     //Prescaler gets clock from FCPU (5MHz)

    //T0CONbits.T08BIT=0;   //8 BIT MODE


}