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

#include "./delay.h"
#include "./configBit.h"
#include "./RF.h"

#include "./defines.h"


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
    
    TRISCbits.TRISC4 = 1;   //SENS_0 input
    
    // Alarm output 
    TRISBbits.TRISB4 = 0; // RA0 to output
    
    RF_Init_RF();   // configure ports of RF
    RF_Init_RF12(); // configure RF module
    
    
    unsigned char counter = 0;
    unsigned char Alarm_state =0;
    unsigned char rx_value = 0;
    while(1){
        ALARM = Alarm_state;            
        if(SENS_0 == 1){
            Alarm_state = 1;
        }
           
        rx_value = RF_receive();
        if(rx_value == 0b00100001){ // CONTROLER REQUESTING STATUS
            if (Alarm_state == 1){
                RF_transmit(0b00100001);
            }
            else{
                RF_transmit(0b00100010);
            }        
        }
        else if (rx_value == 0b00100010){ // CONTROLER REQUESTING to Disable Alarm
            Alarm_state = 0;
        }
        
    }
    
}



