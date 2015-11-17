/* 
 * File:   newmain.c
 * Author: cardias
 *
 * Created on September 14, 2015, 10:41 PM
 */

#define UART_buf_size  16
unsigned char Dev_Address = 0x02;
char  UART_buffer[UART_buf_size]; //16 char buffers

#include <stdio.h>
#include <stdlib.h>
#include <pic18f4520.h>
#include <xc.h>
#include <string.h>

#include "./delay.h"
#include "./configBit.h"
#include "./lcd.h"
#include "./uart_GSM.h"

#include "./RF.h"

#include "./defines.h"


main() {
    
    WDTCONbits.SWDTEN = 0; //turn off watch dog timer
    
    OSCCONbits.IRCF   = 0b111; //Set to 8MHZ
    OSCTUNEbits.PLLEN = 0b1;   //Enable PLL
   
    TRISAbits.TRISA0 = 0; // RA0 to output
    ADCON1bits.PCFG0 = 0b1;  // set to ANALOG OFF
    ADCON1bits.PCFG1 = 0b1;  // set to ANALOG OFF
    ADCON1bits.PCFG2 = 0b1;  // set to ANALOG OFF
    ADCON1bits.PCFG3 = 0b1;  // set to ANALOG OFF
   
    TRISBbits.TRISB0 = 0; // RB0 to output
    TRISBbits.TRISB1 = 0; // RB1 to output
    TRISBbits.TRISB2 = 0; // RB2 to output
    TRISBbits.TRISB3 = 0; // RB3 to output
    
    TRISCbits.TRISC4 = 1;   //SENS_0 input
    
    // Alarm output 
    TRISBbits.TRISB4 = 0; // RA0 to output
    
    // Enable interrupt
    PIE1bits.RCIE   = 0b1; // uart receive interupt
    INTCONbits.PEIE = 0b1; // periperial interrupt enable
    INTCONbits.GIE  = 0b1; // Global interupt enable
    
    LCD_Init();
    RF_Init_RF();   // configure ports of RF
    UART_Init();
    
    unsigned char counter = 0;
    unsigned char Alarm_state =0;
    unsigned char rx_value = 0;


    // LCD integer to string
    char number[7];
    unsigned char rx_value = 0b00000000;
    unsigned char timeout_cnt = 0;
    unsigned char iii = 0;
    
    delay_s(5);
    
    while(1){
        LED_2 = ~SENS_0;
        LED_3 = ~Alarm_state;
        ALARM = Alarm_state;
        
        if(SENS_0){
            Alarm_state = 1;
            
        }
        rx_value = RF_receive();
        if(rx_value != 0){
            if(rx_value == 0b00000001){ // ASKING STATE
                delay_ms(1000);
                if(Alarm_state == 1){
                    RF_transmit(0x01,Alarm_state);
                }
                else{
                    RF_transmit(0x01,0x55);
                }
            }
            else if(rx_value == 0x02){ // Reset alarm state
                Alarm_state = 0b0;
                delay_ms(1000);
                RF_transmit(0x01,0x55);
            }
            else if(rx_value == 0x03){ // ENABLE alarm state
                Alarm_state = 0b1;
                delay_ms(1000);
                RF_transmit(0x01,0x55);
            }

        }
    }
}



