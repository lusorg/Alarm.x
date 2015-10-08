/* 
 * File:   delay.h
 * Author: cardias
 *
 * Created on September 18, 2015, 5:20 PM
 */

#define _XTAL_FREQ 32000000

void delay_us(int repeat){
    while(repeat--){
        __delay_us(1);
    }
}


void delay_ms(int repeat){
    while(repeat--){
        __delay_ms(1);
    }
}




