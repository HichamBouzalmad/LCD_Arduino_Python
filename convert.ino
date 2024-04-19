#define CS A3
#define CD A2
#define WR A1
#define RD A0

#define CS_ACTIVE   PORTC &= ~B00001000
#define CS_IDLE     PORTC |= B00001000
#define CD_CMD      PORTC &= ~B00000100
#define CD_DATA     PORTC |= B00000100 
#define WR_ACTIVE   PORTC &= ~B00000010
#define WR_IDLE     PORTC |= B00000010
#define RD_ACTIVE   PORTC &= ~B00000001
#define RD_IDLE     PORTC |= B00000001

void Write(uint8_t Address){
  WR_ACTIVE;
  PORTD = (PORTD & B00000011) | ((Address)&B11111100);
  PORTB = (PORTB & B11111100) | ((Address)&B00000011);
  WR_IDLE;
  }
  
uint8_t Read(){
  RD_ACTIVE;
  asm("NOP");
  asm("NOP");
  uint8_t result = (PIND & B11111100) | (PINB & B00000011);
  RD_IDLE;
  return result;
  }

void Set_output(){
  DDRB |= B00000011; // 8-9 OUTPUT
  DDRD |= B11111100; // 2-7 OUTPUT
  }

void Set_input(){
  DDRB &= ~B00000011; // 8-9 INPUT
  DDRD &= ~B11111100; // 2-7 INPUT
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  DDRC |= B11111111; // A0-A5 OUTPUT
  PORTC = B00001111; // CS to IDLE
  Set_output();
  // reset
  CS_ACTIVE;
  CD_CMD;
  // Soft reset
  Write(0x01);
  delay(200);
  // Sleep out
  Write(0x11);
  delay(10);
  Write(0x36);
  CD_DATA;
  Write(0x48);
  CD_CMD;
  // Pixel Format Set 
  Write(0x3A);
  // Pixel Format Set DATA
  CD_DATA;
  Write(0x55);
  // DISPLAY ON
  CD_CMD;
  Write(0x29);
  // COL Set
  Write(0x2A);
  //SC[15:8]
  CD_DATA;
  Write(0x00);
  CD_CMD;
  //SC[7:0]
  CD_DATA;
  Write(0x00);
  CD_CMD;
  //EC[15:8]
  CD_DATA;
  Write(0x00);
  CD_CMD;
  //EC[7:0]
  CD_DATA;
  Write(0xF0);
  CD_CMD;
  // PAGE Set
  Write(0x2B);
  //SP[15:8]
  CD_DATA;
  Write(0x00);
  CD_CMD;
  //SP[7:0]
  CD_DATA;
  Write(0x00);
  CD_CMD;
  //EP[15:8]
  CD_DATA;
  Write(0x01);
  CD_CMD;
  //EP[7:0]
  CD_DATA;
  Write(0x40);
  CD_CMD;
  // Memory Write
  Write(0x2C);
  CD_DATA;
}

void loop() {
  // put your main code here, to run repeatedly:
  
  while (Serial.available()) {
    // Read the two bytes from serial
    uint8_t receivedValue[2];
    Serial.readBytes(receivedValue,2);
    // Write the received bytes to the lcd screen
    Write(receivedValue[1]);
    Write(receivedValue[0]);
  }

  //CD_CMD;
  
}
