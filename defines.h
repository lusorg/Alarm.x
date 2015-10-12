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

#define   ALARM      LATBbits.LATB4
