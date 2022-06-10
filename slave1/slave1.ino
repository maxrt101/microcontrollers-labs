
#include "../common.h"
#include "Wire.h"

SlaveState state;

ISR(USART_TX_vect) {
  delay(100);
  PORT &= ~(1 << PD2);
}

void setWriteMode() {
  PORT |= 1 << PD2;
}

void setup() {
  Wire.begin();

  state.data[DT_HOUR] = 50;
  state.data[DT_MONTH_DAY] = 5;
  state.data[DT_WEEK_DAY] = 12;
  state.data[DT_YEAR] = 7;

  state.writeData();
  delay(1000);

  DDR  = 0b00000111;
  PORT = 0b11111000;

  Serial.begin(RS_232_SPEED);
  UCSR0B |= (1 << UCSZ02) | (1 << TXCIE0);
  UCSR0A |= (1 << MPCM0);

  delay(1);

  address = SLAVE1_ADDRESS;
}

void loop() {
  byte data = ~PIN;
  data >>= 3;

  if (Serial.available()) {
    byte inByte = Serial.read();
    if (state.isAddress && state.address == inByte) {
        state.isAddress = false;
        state.isCommand = true;
        UCSR0A &= ~(1 << MPCM0);
      }
    } else if (state.isCommand) {
      state.command = inByte;
      state.isCommand = false;
      if (state.command = COMMAND_BYTE) {
        state.isAddress = true;
        setWriteMode();
        state.writeDataToUART();
      }
    }
  }
}
