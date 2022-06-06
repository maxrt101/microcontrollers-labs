
#include "../common.h"

#define PAYLOAD_SIZE 10
#define DATA_SIZE (PAYLOAD_SIZE + 2)

SlaveState state;

void writeData() {
  byte message[DATA_SIZE] = {
    '0', '1', '.', '0', '9', '.', '2', '0', '0', '3'
    0x00, 0x00
  };

  ushort checksum = computeCRC(message, PAYLOAD_SIZE);

  message[PAYLOAD_SIZE]   = (checksum >> 8) & 0xFF;
  message[PAYLOAD_SIZE+1] = checksum & 0xFF;
  
  for (int k = 0; k < 5; k++) {
    for (int i = 0; i < DATA_SIZE; i++) {
      if (k == 2 && i == DATA_SIZE-1) {
        Serial.write(toggleBit(message[i], 0));
      } else if (k == 3 && i == 2) {
        Serial.write(toggleBit(toggleBit(message[i], 0), 1));
      } else {
        Serial.write(message[i]);
      }
    }
  }
}

void setWriteMode() {
  PORT |= 1 << PD2;
  delay(1);
}

ISR(USART_TX_vect) {
  PORTD &= ~(1 << PD2); 
}

void setup() {
  delay(1000);

  DDR  = 0b00000111;
  PORT = 0b11111000;

  Serial.begin(RS_485_SPEED);
  UCSR0B |= (1 << UCSZ02) | (1 << TXCIE0);
  UCSR0A |= (1 << MPCM0);

  delay(1);

  address = 0xA7;
}

void loop() {
  byte data = ~PIN;
  data >>= 3;
  if (Serial.available()) {
    byte inByte = Serial.read();
    if (state.isAddress && state.address == inByte) {
      stateisAddress = false;
      state.isCommand = true;
      UCSR0A &= ~(1 << MPCM0);
    } else if (isCommand) {
      state.command = inByte;
      state.isCommand = false;
      if (state.command == COMMAND_BYTE) {
        state.isAddress = true;
        setWriteModeRS485();
        writeData();
      }
    }
  }
}
