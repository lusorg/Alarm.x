/* 
 * File:   uart.h
 * Author: cardias
 *
 * Created on September 18, 2015, 12:39 PM
 */


#define UART_buf_size  32 
char  UART_STRING[UART_buf_size]; //16 char buffers
char  old_Uart_string[UART_buf_size];
char  UART_buff_pos = 0;   //0-15 buffer not fill
                           //16 buffer fill

 void UART_Init(){
    // Serial Interface
    TRISCbits.RC6 = 0b1;
    TRISCbits.RC7 = 0b1;
    
    RCSTAbits.SPEN = 0b1; // ENABLE IO PIN
    RCSTAbits.RX9 = 0b0; // 8 bit communication
    RCSTAbits.CREN = 0b1; // Enable receiver
    
    TXSTAbits.TX9 = 0b0; // 8 bit mode
    TXSTAbits.SYNC = 0b0; // Async
    TXSTAbits.BRGH = 0b0; // HIGH BAUD RATE GENERATOR
    TXSTAbits.TXEN = 0b1;
    
    BAUDCONbits.BRG16 = 0b0; // 8 bit generator baud
    BAUDCONbits.ABDEN = 0b0; // Disable auto baud
    SPBRG = 0b110011;  
 }

 void UART_Write_byte(char data)
{
  while(!TRMT);
  TXREG = data;
}
 
 void UART_Write_Text(char *text)
{
  int i;
  for(i=0;text[i]!='\0';i++)
    UART_Write_byte(text[i]);
}
 
 void interrupt UART_add_buffer()
{
     char string_rx[2];
     
     char byte_rx = RCREG;
     
     //check if buffer is full
     if (UART_buff_pos == (UART_buf_size)+1 ){ // if buffer is full 
         UART_buff_pos = 0; // Reset buffer size 
         strcpy(UART_STRING, "\0"); // clear buffer
     }
     
    if(byte_rx == 0x0a){   // check if newline feed close string
        UART_buff_pos = (UART_buf_size)+1;
    }
    else{
        string_rx[1] = '\0';
        string_rx[0] = byte_rx;
        strcat(UART_STRING, string_rx);
        UART_buff_pos = UART_buff_pos + 1;
    }
  
}