/* 
 * File:   newmain.c
 * Author: cardias
 *
 * Created on September 14, 2015, 10:41 PM
 */


unsigned char Dev_Address = 0x01;


#include <stdio.h>
#include <stdlib.h>
#include <pic18f4520.h>
#include <xc.h>
#include <string.h>

#include "./defines.h"
#include "./delay.h"
#include "./configBit.h"
#include "./lcd.h"
#include "./uart_GSM.h"
#include "./RF.h"



unsigned char Send_Message(unsigned char addr, unsigned char msg){
    
    // LCD integer to string
    char number[8];
    
    unsigned long wait_rx_cnt    = 0;
    unsigned long wait_rx_max    = 100000;
    unsigned char tx_retries_max = 3;
    unsigned char tx_retries_cnt = 0;
    unsigned char rx_value       = 0;
    
    transmit:
    LCD_send("TX delay retry:",0,1);
    itoa(number,tx_retries_cnt,10);
    LCD_send(number,1,1);
    delay_ms(1000);
    
    // CHECK RE-TRANSMIT ZONE
    tx_retries_cnt = tx_retries_cnt + 1;
    if (tx_retries_cnt > tx_retries_max){
        LCD_send("FAILED TO COM",0,1);
        LCD_send("FAILED TO COM",1,1);
        delay_ms(5000);
        return 0xFF;   // FAILED TO COMUNICATE WITH SENSOR
    }
    LCD_send("Transmitting",0,1);
    LCD_send("Transmitting",1,1);
    
    
    RF_transmit(addr,msg); // Request sensor
   
    // reset actual read status
    rx_value = 0x00;
    wait_rx_cnt = 0;
    
    LCD_send("WAIT Rx",0,1);
    LCD_send("WAIT Rx",1,1);
    
    while (rx_value == 0x00 &&  wait_rx_cnt < wait_rx_max){
        wait_rx_cnt = wait_rx_cnt + 1;
        rx_value = RF_receive();   
    }
    
    if ((rx_value == 0xFF) || (rx_value == 0x00)) {  // bad RECEPTION or nothing received
        delay_ms(500);
        LCD_send("TIMEOUT",0,1);
        LCD_send("TIMEOUT",1,1);    
        goto transmit;
    }
        
    LCD_send("Value to send:",0,1);
    itoa(number,rx_value,10);
    LCD_send(number,1,1);
    delay_ms(100);
    
    return rx_value;
}

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
    
    unsigned char sensor_state = 0x55;
    
    SMS_delete();
    LCD_send("GSM WAIT",1,1);
    delay_s(20);
    SMS_delete();
    delay_s(20);
    LCD_send("send msg",1,1);
    
    
    
    SMS_send("Alarm Power On");
    SMS_delete();
    

    while(1){
        if(strstr(UART_buffer,"+CMTI: \"SM\"") != NULL){
            UART_clean_buffer();
            SMS_read();
         }
         if(strstr(UART_buffer,"Hello") != NULL){
             SMS_send("Yes I am ALive");
             UART_clean_buffer();
             SMS_delete();
         }

        LCD_send("delay to ask",0,1);
        LCD_send("sensor status",1,1);
        delay_ms(5000);  
        sensor_state = Send_Message(0x02,0x01);
        
        if(sensor_state == 0x01){
            ALARM = 0b1;
            SMS_send("INTRUTION DETECTED !!! Alarm Detected on sensor 1");
            LCD_send("ALARM on sensor 1",0,1);
            LCD_send("Reset on 2sec",1,1);

            sms_loop:    
            while(strstr(UART_buffer,"+CMTI: \"SM\"") == NULL);
            UART_clean_buffer();
            SMS_read();
            if(strstr(UART_buffer,"Off") != NULL){
                UART_clean_buffer();
                SMS_delete();

                ALARM = 0b0;
                sensor_state = Send_Message(0x02,0x02);  // RESET sensor state
                SMS_send("Alarm disabled");
            }
            else{
                goto sms_loop;
            }
        }
    }
 /*
    
    int counter = 0;
    while(1){
        counter = counter +1;
        if(counter == 100){
            SMS_read();
            delay_s(1);
            if(strstr(UART_buffer,"off") != NULL){
                LCD_send("       FOUND    ",1,0);
            }
            else{
                LCD_send("   NOT FOUND    ",1,0);
            }
            SMS_delete();
            counter = 0;
        }
               
        LCD_send(UART_buffer,0,0);
        

    }
  */
    
    
}



