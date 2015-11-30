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
    
    unsigned char sensor_state  = 1; //sensor_state of the Sensor
    unsigned char rx_value      = 0;  //Value received from RF
    unsigned int  timeout       = 0;  //Counter to be used in the buzzer timeout
    unsigned int  timeout_cycle = 0;  //Counter to be used in the buzzer timeout
    
    System_startup();
    LCD_Init();
    RF_Init_RF(); 
    UART_Init();


    blink_led:
    LED_0 = 1;
    LED_1 = 1;
    LED_2 = 1;
    LED_3 = 1;
    delay_ms(900);
    LED_0 = 0;
    LED_1 = 0;
    LED_2 = 0;
    LED_3 = 0;
    delay_ms(100);
    
    if (timeout < 20){
        timeout = timeout + 1;
        goto blink_led;
    } 
    timeout = 0;  
        
    SOUND = 1;
    delay_s(1);
    SOUND = 0;
    

    while(1){
        
    //Sensor MSG
    /* sensor_state  | DEFINITION
     *              |
     *      1       |    Alarm clear 
     *      2       |    Alarmed by local sensor
     *      3       |    Alarmed by local sensor, sound timeout
     *      4       |    Alarmed by request (request from controller)
     *      5       |    Alarmed by request, sound timeout
     *     10       |    Sensor OFF
     */ 
        ///////////////////////////////////
        //
        // LED Indicators
        //
        ///////////////////////////////////
        LED_2 = ~SENS_0;
        if (sensor_state > 1 && sensor_state < 10)
            LED_3 = 0; // ON SENSOR BOARD LED INVERTED
        else
            LED_3 = 1; // ON SENSOR BOARD LED INVERTED
        
        ///////////////////////////////////
        //
        // Alarm BUZZER If Alarm state  
        //
        ///////////////////////////////////
        if(sensor_state == 2 || sensor_state == 4){ 
            SOUND = 1;
        }
        else{
            SOUND = 0; // Disable Buzzer
        }
            
        
        ///////////////////////////////////
        //
        //Sensor reaction
        //
        ///////////////////////////////////
        if(SENS_0 == 1 && sensor_state == 1){
            sensor_state = 2;
            timeout = 0;
            timeout_cycle = 0;
        }
        
        ///////////////////////////////////
        //
        // Timeout control
        //
        ///////////////////////////////////
        
        timeout = timeout + 1;
        if (timeout == 32000){
            timeout_cycle = timeout_cycle +1;
        }
        
        
        if(sensor_state == 2 || sensor_state == 4){
            if (timeout_cycle > 40) {
                sensor_state = sensor_state + 1;
                timeout_cycle = 0;
            }
        }
        
        
        
        /*CONTROLER MSG
        * code    | Definition
        *    1    | request status 
        *    2    | disable Alarm 
        *    3    | enable Alarm by request
        *   10    | Sensor OFF
        */
        
        rx_value = RF_receive();
        if(rx_value != 0){
            if(rx_value == 1){ // ASKING STATE
              
            }
            else if(rx_value == 2){ // Reset alarm state
                sensor_state  = 1; // Clear Alarm state
                timeout_cycle = 0; // Reset Timeout
            }
            else if(rx_value == 3){ // ENABLE alarm by request
                sensor_state = 4; //Alarmed by request (request from controller)
                timeout_cycle = 0; // Reset Timeout
            }
            else if(rx_value == 10){ // Sensor OFF
                sensor_state = 10; //Alarmed by request (request from controller)
            }
            delay_s(1);  // Wait other side ready 
            RF_transmit(0x01,sensor_state);

        }
    }
}



