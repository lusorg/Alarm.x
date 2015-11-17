/* 
 * File:   uart.h
 * Author: cardias
 *
 * Created on September 18, 2015, 12:39 PM
 */


 void UART_Init()
 {
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
  
 SMS_send(char *text){
     UART_Write_Text("AT\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGF=1\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGS=\"968390952\"\r");
     delay_ms(500);
     UART_Write_Text(text);
     delay_ms(500);
     UART_Write_byte(0x1A);
     delay_ms(500);
     UART_Write_Text("\r");
     delay_s(5);
 }
 
  SMS_read(){
     UART_Write_Text("AT+CSCS=\"GSM\"\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGR=1\r");
     delay_s(5);
 }
  
  SMS_delete(){
     UART_Write_Text("AT+CMGD=1\r");
     delay_s(1);
     UART_Write_Text("AT+CMGD=2\r");
     delay_s(1);
     UART_Write_Text("AT+CMGD=3\r");
     delay_s(1);
     UART_Write_Text("AT+CMGD=4\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGD=5\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGD=6\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGD=7\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGD=8\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGD=9\r");
     delay_ms(500);
     UART_Write_Text("AT+CMGD=10\r");
     delay_ms(500);

 }
  
  
  
  void UART_clean_buffer(){
    int i = 0;
    for(i=0; i < UART_buf_size-1 ; i++)
        UART_buffer[1] = "-";
  }
  
   void interrupt UART_add_buffer()
{
     int i = 0;
     char byte_rx = RCREG;
     
     // SHIFT ALL BUFFER  
     if (byte_rx != '\r' && byte_rx != '\n'){
         loop_ring:
            UART_buffer[i] = UART_buffer[i+1];
            i=i+1;
            if (i != UART_buf_size-1){
                goto loop_ring;
            }
         UART_buffer[UART_buf_size-1] = byte_rx;
     }
}