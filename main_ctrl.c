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
#include "./lcd.h"
#include "./uart.h"
#include "./RF.h"


/*
 * 
 * TRIS register (data direction register)
 * PORT register (reads the levels on the pins of the device)
 * LAT register (output latch)
 */

#define LED LATAbits.LATA0

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
    
    
    UART_Init();
    LCD_Init();
    RF_Init_RF();   // configure ports of RF
    RF_Init_RF12(); // configure RF module
    
    // IO ports
    LED = 1;
    delay_ms(24);
    LED = 0;
    delay_ms(24);
    LED = 1;
    delay_ms(24);
    LED = 0;
    delay_ms(24);
    LED = 1;
    delay_ms(24);
    LED = 0;
    delay_ms(24);


    while(1){
        
        LED = 0;
        delay_ms(500);
        //while(TXSTAbits.TRMT == 0){}// wait until last tx was send
        //UART_Write_Text("Hello ");
        LED = 1;
        delay_ms(500);
        //while(TXSTAbits.TRMT == 0){}// wait until last tx was send
        //UART_Write_Text("World\r\n");
  

        char number[6];
        itoa(number, RF_Read_FIFO(), 10);
        LCD_send(number, 1, 0);
        
        
        
        if (strcmp(old_Uart_string, UART_STRING) != 0 && UART_buff_pos == (UART_buf_size + 1)){
            LCD_send(UART_STRING, 2, 0);
            UART_Write_Text(UART_STRING);
            strcpy(old_Uart_string, UART_STRING);
        }

    }
    
}



