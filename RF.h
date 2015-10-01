

typedef unsigned char uchar;
typedef unsigned int uint;



//INPUTS
#define   SDO       PORTDbits.RD7

//OUTPUTS
#define   SCK       LATDbits.LATD6    
#define   SDI       LATDbits.LATD5    
#define   nSEL      LATDbits.LATD4


// Other values used default of Wirless development suite  (WDS silabs)

void RF_Write_bit( uchar value ) {
  SCK=0;
  delay_us(1);
  SDI=value;
  delay_us(1);
  SCK=1;
  delay_us(1);
}


void WriteCMD( uint CMD ){
  uchar n=16;
  SCK=0;
  nSEL=0;
  while(n > 0){
    if(CMD&0x8000)
      RF_Write_bit(1);
    else
      RF_Write_bit(0);
    CMD=CMD<<1;
    n = n - 1;
  }
  SCK=0;
  delay_us(1);
  nSEL=1;
  delay_us(1);
}


void RF_Rst_FIFO(void){
  WriteCMD(0xCA81);
  delay_us(1);
  WriteCMD(0xCA83);
  delay_us(1);
}


void RF_Init_TX(void){
  WriteCMD(0x8238); // transmiter ON | synthesizer ON | oscilator ON | receiver OFF | receiver baseband OFF
}

void RF_Stop_TX(void){
  WriteCMD(0x8208); // transmiter OFF | synthesizer OFF | oscilator ON | receiver OFF | receiver baseband OFF
  delay_ms(1);
}

void RF_Init_RX(void){
  WriteCMD(0x82D8); // transmiter OFF | synthesizer ON | oscilator ON | receiver ON | receiver baseband ON
  RF_Rst_FIFO();
}


void RF_Init_RF12(void) {
  delay_ms(500);
  WriteCMD(0x80E8); //868 | ENABLE TX Register ON | ENABLE RX FIFO
  WriteCMD(0xA654); // central freq 868.1
  WriteCMD(0x94A0); // 135 khz | Fast | LNA MAX POWER | DRSSI -103 | VDI ONN |
  WriteCMD(0x9857); // fo +df | deviation 90 | Pout -17.5d (minimum) 
  WriteCMD(0xCC77); // crystal low power mode | disbaled pll dithering
  WriteCMD(0xE196); 
  WriteCMD(0xC80E); 
  WriteCMD(0xC000);
  WriteCMD(0xC4C7); // mode auto || AFC range unlimited
  WriteCMD(0xC647); // data rate 4.789
  WriteCMD(0xC2AC); // auto || digital LPF
  WriteCMD(0xCED4);
  RF_Rst_FIFO();
  delay_ms(1);
  RF_Init_RX();
}

void RF_Init_RF(void) {
  ////////////////////////////////
  // PORT DEFINITION

  // INPUTS
  TRISDbits.TRISD7 = 1;   //SDO  

  // OUTPUTS
  TRISDbits.TRISD6 = 0;   //SCK
  TRISDbits.TRISD5 = 0;   //SDI
  TRISDbits.TRISD4 = 0;   //NSEL
  
  nSEL=1;
  SDI=1;
  SCK=0;
}

uchar RF_Data_Ready(void){
  SCK=0;
  nSEL=0;
  SDI=0;
  delay_us(1);
  SCK=1;
  delay_us(1);
  if(SDO == 1) {   //Polling SDO 
    SCK=0;
    SDI=1;
    nSEL=1;
    delay_us(1);
    return 1; 
  }
  else {
    SCK=0;
    SDI=1;
    nSEL=1;
    delay_us(1);
    return 0;
  }
}

void RF_WriteFSKbyte( uchar DATA ){
  uchar RGIT=0;
  uint temp=0xB800;
  temp = DATA + temp;
  Loop: 
  RGIT = RF_Data_Ready();
  if(RGIT==0) {
    delay_us(1);
    goto Loop;
  }
  else {
    RGIT=0;
    WriteCMD(temp);
  }
}

uchar RF_Read_FIFO(void) {
  uchar i,Result;
  SCK=0;
  SDI=0;
  nSEL=0;
  for(i=0;i<16;i++) {  //skip status bits
    SCK=1;
    delay_us(1);
    SCK=0;
    delay_us(1);
  }
  Result=0;
  for(i=0;i<8;i++) {  //read fifo data byte

    SCK=1;
    delay_us(1);
    Result=Result<<1;
    if(SDO == 1){
      Result = Result + 1;
    }
    SCK=0;
    delay_us(1);
  }
  nSEL=1;
  return(Result);
} 


unsigned char RF_receive(void){
    
    unsigned char i = 0;
    unsigned char RF_RXBUF[2];
    
    if (RF_Data_Ready() == 1){
        while(i < 2) {
            while((RF_Data_Ready() == 1)); // WAIT for next byte
            RF_RXBUF[i] = RF_Read_FIFO();
            i= i+1;
        }
        if (RF_RXBUF[0] == RF_RXBUF[1]){
            RF_Rst_FIFO(); //Clear FIFO
            return RF_RXBUF[0]; // VALID COMMAND RECEIVED
        }
        else {
            RF_Rst_FIFO(); //Clear FIFO
            return 0xFF;  // ERROR RECEIVED
        }
    }
    else{
        RF_Rst_FIFO(); //Clear FIFO
        return 0x00;  // NOTHING RECEIVED
    }
}


void RF_transmit(unsigned char address, unsigned char command){
    
    unsigned char TX_word;
    
    TX_word = address;
    TX_word = TX_word << 4;
    TX_word = (TX_word & 0xF0) & (command & 0x0F);
    
    RF_Init_TX();
    RF_WriteFSKbyte( 0xAA );
    RF_WriteFSKbyte( 0xAA );
    RF_WriteFSKbyte( 0xAA );
    RF_WriteFSKbyte( 0x2D );
    RF_WriteFSKbyte( 0xD4 );

    RF_WriteFSKbyte( TX_word );
    RF_WriteFSKbyte( TX_word );

    RF_WriteFSKbyte( 0xAA );
    RF_WriteFSKbyte( 0xAA );
    RF_WriteFSKbyte( 0xAA );
    RF_WriteFSKbyte( 0xAA );
    RF_WriteFSKbyte( 0xAA );
    RF_Init_RX();
}
