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

#define ALARM  LATBbits.LATB4

#define LCD_LED     LATCbits.LATC1
#define LCD_PIN_RS  LATCbits.LATC2
#define LCD_PIN_E   LATCbits.LATC3

#define UART_buf_size  128
//INPUTS
#define   SDO       PORTDbits.RD7

//OUTPUTS
#define   SCK       LATDbits.LATD6    
#define   SDI       LATDbits.LATD5    
#define   nSEL      LATDbits.LATD4

unsigned char RF_RXBUF[8];


char  UART_buffer[UART_buf_size] = "--------------------------------------------------------------------------------------------------------------------------------"; //16 char buffers

