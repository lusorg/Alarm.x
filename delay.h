/* 
 * File:   delay.h
 * Author: cardias
 *
 * Created on September 18, 2015, 5:20 PM
 */

#define _XTAL_FREQ 32000000

void delay_us(int repeat_1){
    while(repeat_1--){
        __delay_us(1);
    }
}

void delay_ms(int repeat_2){
    while(repeat_2--){
        __delay_ms(1);
    }
}

void delay_s(int repeat_3){
    while(repeat_3--){
        delay_ms(1000);
    }
}



