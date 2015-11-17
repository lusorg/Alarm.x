/* 
 * File:   newmain.c
 * Author: cardias
 *
 * Created on September 14, 2015, 10:41 PM
 */

unsigned char Dev_Address = 0x02;

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


main() {
    
    System_startup();
    LCD_Init();
    RF_Init_RF(); 
    UART_Init();
    
    ALARM = 1;
    delay_s(1);
    ALARM = 0;
    
    unsigned char Alarm_state   = 0;  //Alarm_state of the Sensor
    unsigned char rx_value      = 0;  //Value received from RF
    unsigned int  alarm_timeout = 0;  //Counter to be used in the buzzer timeout

    
    while(1){
        
    //Sensor MSG
    /* Alarm_state  | DEFINITION
     *              |
     *      1       |    Alarm clear 
     *      2       |    Alarmed by local sensor
     *      3       |    Alarmed by local sensor, sound timeout
     *      4       |    Alarmed by request (request from controller)
     *      5       |    Alarmed by request, sound timeout
     */ 
        
        // LED Indicators
        LED_2 = ~SENS_0;
        if (Alarm_state > 1)
            LED_3 = 0; // ON SENSOR BOARD LED INVERTED
        else
            LED_3 = 1; // ON SENSOR BOARD LED INVERTED
        
        // Alarm BUZZER If Alarm state  
        if((Alarm_state == 2 || Alarm_state == 4) && alarm_timeout < 1000) {
            ALARM = 1; // Enable BUZZER
            alarm_timeout = alarm_timeout + 1;   
        }
        else
            ALARM = 0; // Disable Buzzer
        
        
        
        //Sensor reaction
        if(SENS_0 == 1 && Alarm_state != 3)
            Alarm_state = 2;
        
        
        /*CONTROLER MSG
        * code    | Definition
        *    1    | request status 
        *    2    | disable Alarm 
        *    3    | enable Alarm by request
        */
        
        rx_value = RF_receive();
        if(rx_value != 0){
            if(rx_value == 1){ // ASKING STATE
               delay_ms(1000);  // Wait other side ready
               RF_transmit(0x01,Alarm_state);
            }
            else if(rx_value == 2){ // Reset alarm state
                Alarm_state   = 1; // Clear Alarm state
                alarm_timeout = 0; // Reset Timeout
                delay_ms(1000);// Wait other side ready
                RF_transmit(0x01,Alarm_state);
            }
            else if(rx_value == 0x03){ // ENABLE alarm by request
                Alarm_state = 4; //Alarmed by request (request from controller)
                delay_ms(1000);
                RF_transmit(0x01,Alarm_state);
            }

        }
    }
}



