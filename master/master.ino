
#include "../common.h"

MasterState state;

ISR(USART1_TX_vect) {
  PORT &= ~(1 << PD1); 
}

void setWriteMode() {
  PORT |= 1 << PD1;
  delay(1);
}

void setup() {
  DDR |= 1 << PD1;
  PORT &= ~(1 << PD1);

  Serial.begin(RS_485_SPEED);
  Serial1.begin(RS_232_SPEED);

  UCSR1B |= (1 << UCSZ12) | (1 << TXCIE1);
}

void loop() {  
  if (Serial.available()) {
    byte inByte = Serial.read();
    if (state.isAddress) {
      setWriteMode();
      UCSR1B |= 1 << TXB81;
      Serial1.write(inByte);
      state.isAddress = false;
      state.isCommand = true;
    } else if (state.isCommand) {
      setWriteMode();
      UCSR1B &= ~(1 << TXB81);
      Serial1.write(inByte);
      state.command = inByte;
      state.isCommand = false;
      if (state.command == COMMAND_BYTE) {
        state.isAddress = true;
      }
    } else {
      setWriteMode();
      UCSR1B &= ~(1 << TXB81);
      Serial1.write(inByte);
      state.isAddress = true;
    }
  }

  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}
