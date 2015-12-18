/* 
 * File:   main_ctrl.c
 * Author: cardias
 *
 * Created on September 14, 2015, 10:41 PM
 */



char number[8];
unsigned int n;


unsigned char Dev_Address = 0x01;

#define n_Sensor 1
unsigned char sensor_state[n_Sensor];
unsigned char last_sensor_state[n_Sensor];

unsigned char Alarm_ON = 1;

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

unsigned char Send_Message(unsigned char addr, unsigned char msg) {

    // LCD integer to string

    unsigned long wait_rx_cnt = 0;
    unsigned long wait_rx_max = 100000;
    unsigned char tx_retries_max = 3;
    unsigned char tx_retries_cnt = 0;
    unsigned char rx_value = 0;

transmit:
    LCD_send("TX delay retry:", 0, 1);
    itoa(number, tx_retries_cnt, 10);
    LCD_send(number, 1, 1);
    delay_ms(1000);

    // CHECK RE-TRANSMIT ZONE
    tx_retries_cnt = tx_retries_cnt + 1;
    if (tx_retries_cnt > tx_retries_max) {
        LCD_send("FAILED TO COM", 0, 1);
        LCD_send("FAILED TO COM", 1, 1);
        delay_ms(200);
        return 0xFF; // FAILED TO COMUNICATE WITH SENSOR
    }
    LCD_send("Transmitting", 0, 1);
    LCD_send("Transmitting", 1, 1);


    RF_transmit(addr, msg); // Request sensor

    // reset actual read status
    rx_value = 0x00;
    wait_rx_cnt = 0;

    LCD_send("WAIT Rx", 0, 1);
    LCD_send("WAIT Rx", 1, 1);

    while (rx_value == 0x00 && wait_rx_cnt < wait_rx_max) {
        wait_rx_cnt = wait_rx_cnt + 1;
        rx_value = RF_receive();
    }

    if ((rx_value == 0xFF) || (rx_value == 0x00)) { // bad RECEPTION or nothing received
        delay_ms(200);
        LCD_send("TIMEOUT", 0, 1);
        LCD_send("TIMEOUT", 1, 1);
        goto transmit;
    }

    LCD_send("Rx of RF", 0, 1);
    itoa(number, rx_value, 10);
    LCD_send(number, 1, 1);
    delay_ms(100);

    return rx_value;
}

void show_buffer() {
    LCD_send_command_byte(0x02); // go back
    LCD_send_data_byte(UART_buffer[0]);
    LCD_send_data_byte(UART_buffer[1]);
    LCD_send_data_byte(UART_buffer[2]);
    LCD_send_data_byte(UART_buffer[3]);
    LCD_send_data_byte(UART_buffer[4]);
    LCD_send_data_byte(UART_buffer[5]);
    LCD_send_data_byte(UART_buffer[6]);
    LCD_send_data_byte(UART_buffer[7]);
    LCD_send_data_byte(UART_buffer[8]);
    LCD_send_data_byte(UART_buffer[9]);
    LCD_send_data_byte(UART_buffer[10]);
    LCD_send_data_byte(UART_buffer[11]);
    LCD_send_data_byte(UART_buffer[12]);
    LCD_send_data_byte(UART_buffer[13]);
    LCD_send_data_byte(UART_buffer[14]);
    LCD_send_data_byte(UART_buffer[15]);
    LCD_send_command_byte(0xC0); // go back
    LCD_send_data_byte(UART_buffer[16]);
    LCD_send_data_byte(UART_buffer[17]);
    LCD_send_data_byte(UART_buffer[18]);
    LCD_send_data_byte(UART_buffer[19]);
    LCD_send_data_byte(UART_buffer[20]);
    LCD_send_data_byte(UART_buffer[21]);
    LCD_send_data_byte(UART_buffer[22]);
    LCD_send_data_byte(UART_buffer[23]);
    LCD_send_data_byte(UART_buffer[24]);
    LCD_send_data_byte(UART_buffer[25]);
    LCD_send_data_byte(UART_buffer[26]);
    LCD_send_data_byte(UART_buffer[27]);
    LCD_send_data_byte(UART_buffer[28]);
    LCD_send_data_byte(UART_buffer[29]);
    LCD_send_data_byte(UART_buffer[30]);
    LCD_send_data_byte(UART_buffer[31]);
    delay_s(1);
}

main() {

    for (n = 0; n < n_Sensor; n++) {
        sensor_state[n] = 0xFE; // STARTUP DUMMY STATES
        last_sensor_state[n] = 0xFE;
    }

    System_startup();
    LCD_Init();
    RF_Init_RF();
    UART_Init();

    delay_s(10);
    UART_Write_Text("AT\r");
    delay_s(2);
    UART_Write_Text("AT+IPR=2400\r");
    delay_s(2);
    UART_Write_Text("AT&W\r");
    delay_s(2);

    SMS_delete();
    LCD_send("Hello World", 0, 1);
    LCD_send("GSM WAIT", 1, 1);
    delay_s(1);
    SMS_delete();
    delay_s(1);
    LCD_send("SMS Power On", 0, 1);
    SMS_send("Alarm Power On");
    SMS_delete();

    //timer control
    //INTCONbits.TMR0IF = 0;
    //T0CONbits.TMR0ON=1;

    while (1) {

        /*CONTROLER MSG
         * code    | Definition
         *    1    | request status 
         *    2    | disable Alarm 
         *    3    | enable Alarm by request
         *   10    | Sensor OFF
         */

        if (strstr(UART_buffer, "+CMTI: \"SM\"") != NULL) {
            SMS_read();
            ///////////////////////////////////////////////////////////////////
            if (strstr(UART_buffer, "HELLO") != NULL) {
                delay_s(1);
                //show_buffer();
                SMS_send("Yes I am ALive");
            }                ///////////////////////////////////////////////////////////////////            
            else if (strstr(UART_buffer, "STATUS") != NULL) {
                delay_s(1);
                //show_buffer();
                SMS_send("STATUS:");
                for (n = 0; n < n_Sensor; n++) {
                    sensor_state[n] = Send_Message((n + 2), MSG_RQST_STAT);
                    itoa(number, sensor_state, 10);
                    SMS_send(number);
                }
            }                ///////////////////////////////////////////////////////////////////            
            else if (strstr(UART_buffer, "ALARM") != NULL) {
                delay_s(1);
                //show_buffer();
                for (n = 0; n < n_Sensor; n++) {
                    Send_Message((n + 2), MSG_RQST_ALRM);
                }
            }                ///////////////////////////////////////////////////////////////////            
            else if (strstr(UART_buffer, "OFF") != NULL) {
                delay_s(1);
                //show_buffer();
                for (n = 0; n < n_Sensor; n++) {
                    Send_Message((n + 2), MSG_ALRM_OFF);
                }
                Alarm_ON = 0;
                SMS_send("ALARM OFF");
                //!!!!!!!!!!!!!!!!!!!!!!!!!! OFF LOCAL
            }                ///////////////////////////////////////////////////////////////////            
            else if (strstr(UART_buffer, "ON") != NULL) {
                delay_s(1);
                //show_buffer();
                Alarm_ON = 1;
                for (n = 0; n < n_Sensor; n++) {
                    Send_Message((n + 2), MSG_DSBL_ALRM);
                }
            }                ///////////////////////////////////////////////////////////////////            
            else {
                delay_s(1);
                show_buffer();
                LCD_send("Unknown command", 0, 1);
                LCD_send("Please retry", 1, 1);
                SMS_send("Unknown command please retry");
            }
            UART_clean_buffer();
            SMS_delete();
        }


        //Sensor MSG
        // Alarm_state  | DEFINITION
        //              |
        //      1       |    Alarm clear 
        //      2       |    Alarmed by local sensor
        //      3       |    Alarmed by local sensor, sound timeout
        //      4       |    Alarmed by request (request from controller)
        //      5       |    Alarmed by request, sound timeout
        //     10       |    Sensor OFF 
        //

        if (Alarm_ON == 1) {
            LCD_send("Get Sensor Status", 0, 1);
            LCD_send("in 5 Seconds", 1, 1);
            delay_s(1);
            LCD_send("in 4 Seconds", 1, 1);
            delay_s(1);
            LCD_send("in 3 Seconds", 1, 1);
            delay_s(1);
            LCD_send("in 2 Seconds", 1, 1);
            delay_s(1);
            LCD_send("in 1 Seconds", 1, 1);
            delay_s(1);

            for (n = 0; n < n_Sensor; n++) {
                sensor_state[n] = Send_Message([n+2], MSG_RQST_STAT); //Request status

                if (last_sensor_state[n] != sensor_state[n]) {
                    if (sensor_state[n] == 0xFF) {
                        SMS_send("Failed to communicate with sensor");
                        LCD_send("RX ERROR", 0, 1);
                        LCD_send("RX ERROR", 1, 1);
                    } else if (sensor_state[n] == ST_ALRM_CLEAR) {
                        SMS_send("Alarm clear in sensor");
                        LCD_send("Alarm Clear", 0, 1);
                        LCD_send("Alarm Clear", 0, 1);
                    } else if (sensor_state[n] == ST_ALRM_SENS) {
                        SMS_send("INTRUTION DETECTED !!! Alarm Detected on sensor 1");
                        LCD_send("Alarm on sensor 1", 0, 1);
                        LCD_send("Wait for disarm", 1, 1);
                    } else if (sensor_state[n] == ST_ALRM_SENS_TMT) {
                        SMS_send("INTRUTION DETECTED !!! Alarm sound Timeout");
                        LCD_send("Alarm on sens 1", 0, 1);
                        LCD_send("Sound Timeout", 1, 1);
                    } else if (sensor_state[n] == ST_ALRM_REQ) {
                        SMS_send("Alarm By Request !!!");
                        LCD_send("Alarm By Request", 0, 1);
                        LCD_send("Wait for disarm", 1, 1);
                    } else if (sensor_state[n] == ST_ALRM_REQ_TMT) {
                        SMS_send("Alarm By Request !!! Alarm sound Timeout");
                        LCD_send("Alarm By Request", 0, 1);
                        LCD_send("Sound Timeout", 1, 1);
                    } else if (sensor_state[n] == ST_ALRM_OFF) {
                        SMS_send("Alarm OFF ON SENSOR !!!");
                        LCD_send("SENSOR OFF", 0, 1);
                        LCD_send("SENSOR OFF", 1, 1);
                    }
                }
                last_sensor_state[n] = sensor_state[n];
            }
        }
    }//while 1
}//Main



