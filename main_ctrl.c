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

    // LCD integer to string
    //itoa(number, rx_value, 10)
    
    while(1){
        delay_ms(5000);
        RF_transmit(0b00100001); // Request status to sensor 1
        
        char number[7];
        unsigned char rx_value = 0b00000000;
        loop_request_status:
        while(rx_value == 0b00000000){
            rx_value = RF_receive();
        }
        /*
        if(rx_value == 0xFF)
            goto loop_request_status; // error in transmition request again
        */ 
        if(rx_value == 0b00100001){ // check if sensor is alarmed
            LCD_send("ALARM!!!", 0, 1);
            ALARM = 1;
            delay_ms(5000);
            ALARM = 0;
            LCD_send("Re-Armed", 0, 1);
            RF_transmit(0b00100010); // Disable alarm state of Sensor 1   
        }
        
        if (strcmp(old_Uart_string, UART_STRING) != 0 && UART_buff_pos == (UART_buf_size + 1)){
            LCD_send(UART_STRING, 1, 1);
            UART_Write_Text(UART_STRING);
            strcpy(old_Uart_string, UART_STRING);
        }

    }
    
}



