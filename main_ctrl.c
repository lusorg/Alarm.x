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
#include "./lcd.h"
#include "./uart.h"
#include "./RF.h"

#define   ALARM      LATBbits.LATB4
/*
 * 
 * TRIS register (data direction register)
 * PORT register (reads the levels on the pins of the device)
 * LAT register (output latch)
 */


main() {
    
    WDTCONbits.SWDTEN = 0; //turn off watch dog timer
    
    OSCCONbits.IRCF   = 0b111; //Set to 8MHZ
    OSCTUNEbits.PLLEN = 0b1;   //Enable PLL  
   
    TRISAbits.TRISA0 = 0; // RA0 to output
    ADCON1bits.PCFG0 = 0b1;  // set to ANALOG OFF
    ADCON1bits.PCFG1 = 0b1;  // set to ANALOG OFF
    ADCON1bits.PCFG2 = 0b1;  // set to ANALOG OFF
    ADCON1bits.PCFG3 = 0b1;  // set to ANALOG OFF
    
    
      // Enable interrupt
    PIE1bits.RCIE   = 0b1; // uart receive interupt
    INTCONbits.PEIE = 0b1; // periperial interrupt enable
    INTCONbits.GIE  = 0b1; // Global interupt enable
    
    // Alarm output 
    TRISBbits.TRISB4 = 0; // RA0 to output
    
    
    UART_Init();
    LCD_Init();
    RF_Init_RF();   // configure ports of RF
    RF_Init_RF12(); // configure RF module
    
    // IO ports
    
    ALARM = 1;
    LED = 1;
    delay_ms(50);
    LED = 0;
    delay_ms(50);
    LED = 1;
    delay_ms(50);
    LED = 0;
    delay_ms(50);
    LED = 1;
    delay_ms(50);
    LED = 0;
    delay_ms(50);
    ALARM = 0;

    while(1){
        
        char number[7];
        unsigned char rx_value;
        
        
        rx_value = RF_receive();
        
        if(rx_value != 0){
            //itoa(number, rx_value, 10);
            //LCD_send(number, 0, 1);
            if (rx_value == 0b01000001){
                    LCD_send("ALARM!!!", 0, 1);
                    ALARM = 1;
                    delay_ms(300);
                    ALARM = 0;
            }
            if (rx_value == 0b01000000){
                    LCD_send("OK", 0, 1);
                    ALARM = 0;
            }

        }

      
        if (strcmp(old_Uart_string, UART_STRING) != 0 && UART_buff_pos == (UART_buf_size + 1)){
            LCD_send(UART_STRING, 1, 1);
            UART_Write_Text(UART_STRING);
            strcpy(old_Uart_string, UART_STRING);
        }

    }
    
}



