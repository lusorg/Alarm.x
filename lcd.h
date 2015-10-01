/* 
 * File:   lcd.h
 * Author: cardias
 *
 * Created on September 18, 2015, 5:16 PM
 */

#define LCD_LED     LATCbits.LATC1
#define LCD_PIN_RS  LATCbits.LATC2
#define LCD_PIN_E   LATCbits.LATC3

void LCD_send_nibble(unsigned char nibble)
{
    // Set RD0-3 without affecting RD4-7
    LATD = (LATD & 0xF0) + nibble;
    delay_ms(1);
    // Note: data is latched on falling edge of pin E
    LCD_PIN_E = 0;
    delay_ms(1);
    LCD_PIN_E = 1;
    delay_ms(1); // Enough time even for slowest command
}
  
// Send a command byte (i.e. with pin RS low)
void LCD_send_command_byte(unsigned char byte)
{
    LCD_PIN_RS = 0;
    LCD_send_nibble(byte >> 4);
    LCD_send_nibble(byte & 0xF);
}
  
// Send a data byte (i.e. with pin RS high)
void LCD_send_data_byte(unsigned char byte)
{
    LCD_PIN_RS = 1;
    LCD_send_nibble(byte >> 4);
    LCD_send_nibble(byte & 0xF);
}


void LCD_send(unsigned char *text, char line, char clear_line )
{
    int n;
    if (line == 1)
        LCD_send_command_byte(0x02); // go back
    else
        LCD_send_command_byte(0xC0); // go back
    
    // clear line
    if (clear_line == 0b1){
        for (n=0 ; n < 16 ; ++n)
            LCD_send_data_byte(' ');
        if (line == 1)
            LCD_send_command_byte(0x02); // go back
        else
            LCD_send_command_byte(0xC0); // go back
    }
    
    int string_size = 0;
    
    string_size = strlen(text);
    if (string_size > 16)
        string_size = 16;
    
    for (n=0 ; n<string_size ; ++n)
        LCD_send_data_byte(text[n]);
    
}

void LCD_Init(){
    
    TRISD = 0b00000000; // Set RD0-7 as digital outputs
    
    TRISCbits.RC1 = 0b0;  //LCD_LED
    TRISCbits.RC2 = 0b0;  //LCD_PIN_RS
    TRISCbits.RC3 = 0b0;  //LCD_PIN_E
         
    // Let's just write to the LCD and never read!
    // We'll wait 2ms after every command since we can't
    // check the busy flag.
    LCD_PIN_RS = 0;
    LCD_PIN_E = 1;
      
    //LCD power-up delay

    delay_ms(1000);
     
    // Initialisation
    delay_ms(20); // must be more than 15ms
    LCD_send_nibble(0b0011);
    delay_ms(5); // must be more than 4.1ms
    LCD_send_nibble(0b0011);
    delay_ms(1); // must be more than 100us
    LCD_send_nibble(0b0011);
    delay_ms(5); // must be more than 4.1ms
    LCD_send_nibble(0b0010); // select 4-bit mode
     
    // Display settings
    LCD_send_command_byte(0b00101000); // N=0 : 2 lines (half lines!), F=0 : 5x7 font
    LCD_send_command_byte(0b00001000); // Display: display off, cursor off, blink off
    LCD_send_command_byte(0b00000001); // Clear display
    LCD_send_command_byte(0b00000110); // Set entry mode: ID=1, S=0
    LCD_send_command_byte(0b00001100); // Display: display on, cursor on, blink on
      
    // Define two 8 character strings
    const char line1[] = "-=HELLO  WORLD=-";
      
    LCD_send(line1,1,0);
    LCD_send(line1,2,0);
      
    LCD_LED = 1;
    delay_ms(500);
    LCD_LED = 0;
    delay_ms(500);
    LCD_LED = 1;
    delay_ms(500);
}