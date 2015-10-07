/* 
 * File:   newmain.c
 * Author: cardias
 *
 * Created on September 14, 2015, 10:41 PM
 */



#include <stdio.h>
#include <stdlib.h>
#include <pic18f4520.h>
#include <xc.h>
#include <string.h>


#include "./configBit.h"
#include "./delay.h"
#include "./RF.h"
//#include "./lcd.h"
//#include "./uart.h"


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

void LED_send(void){
    LED_0 = 0;
    LED_1 = 1;
    LED_2 = 1;
    LED_3 = 1;
    delay_ms(75);
        
    LED_0 = 1;
    LED_1 = 0;
    LED_2 = 1;
    LED_3 = 1;
    delay_ms(75);

    LED_0 = 1;
    LED_1 = 1;
    LED_2 = 0;
    LED_3 = 1;
    delay_ms(75);

    LED_0 = 1;
    LED_1 = 1;
    LED_2 = 1;
    LED_3 = 0;
    delay_ms(75);
        
    LED_0 = 1;
    LED_1 = 1;
    LED_2 = 0;
    LED_3 = 1;
    delay_ms(75);

    LED_0 = 1;
    LED_1 = 0;
    LED_2 = 1;
    LED_3 = 1;
    delay_ms(75);

 }




main() {
    
    WDTCONbits.SWDTEN = 0; //turn off watch dog timer
    
    OSCCONbits.IRCF   = 0b111; //Set to 8MHZ
    OSCTUNEbits.PLLEN = 0b1;   //Enable PLL  
   
    TRISBbits.TRISB0 = 0; // RB0 to output
    TRISBbits.TRISB1 = 0; // RB1 to output
    TRISBbits.TRISB2 = 0; // RB2 to output
    TRISBbits.TRISB3 = 0; // RB3 to output
    
    RF_Init_RF();   // configure ports of RF
    RF_Init_RF12(); // configure RF module
    
    
    unsigned char counter = 0;
    while(1){
        counter = counter +1;
        LED_0 = 0;
        LED_1 = 0;
        LED_2 = 0;
        LED_3 = 0;
        RF_transmit(counter);
        LED_0 = 1;
        LED_1 = 1;
        LED_2 = 1;
        LED_3 = 1;
        delay_ms(5000);
    }
    
}



