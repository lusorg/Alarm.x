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

#include "./config.h"
#include "./delay.h"
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
    
    System_startup();
    LCD_Init();
    RF_Init_RF(); 
    UART_Init();
    
    unsigned char sensor_state = 0x00;
    
    SMS_delete();
    LCD_send("Hello World",0,1);
    LCD_send("GSM WAIT",1,1);
    delay_s(20);
    SMS_delete();
    delay_s(20);
    LCD_send("SMS Power On",0,1);
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

        LCD_send("Ask Waiting",0,1);
        LCD_send("sensor status",1,1);
        delay_ms(5000);
        
        //Sensor MSG
        /* Alarm_state  | DEFINITION
        *              |
        *      1       |    Alarm clear 
        *      2       |    Alarmed by local sensor
        *      3       |    Alarmed by local sensor, sound timeout
        *      4       |    Alarmed by request (request from controller)
        *      5       |    Alarmed by request, sound timeout
        */ 
        sensor_state = Send_Message(0x02, 1);
             
        /*CONTROLER MSG
        * code    | Definition
        *    1    | request status 
        *    2    | disable Alarm 
        *    3    | enable Alarm by request
        */
        if(sensor_state == 0x01){
            ALARM = 0b1;
            SMS_send("INTRUTION DETECTED !!! Alarm Detected on sensor 1");
            LCD_send("ALARM on sensor 1",0,1);
            LCD_send("Wait for disarm",1,1);

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
                SMS_send("Unknown SMS received, please retry");
                SMS_delete();
                goto sms_loop;
            }
        }
    }//while 1
}//Main



