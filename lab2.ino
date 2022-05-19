
typedef unsigned char byte;

const int  kAlgo1Button = 32;
const int  kAlgo2Button = 30;
const byte kAlgo1Code = 0xA1;
const byte kAlgo2Code = 0xA2;
const int  kDelay = 1150;

void setup() {
  DDRA = 0xff;
  PORTA = 0;
  Serial.begin(9600);
  pinMode(kAlgo1Button, INPUT_PULLUP);
  pinMode(kAlgo2Button, INPUT_PULLUP);
}

void loop() {
  if (Serial.available()) {
    int inByte = Serial.read();
    if (inByte == kAlgo1Code) {
      int i = 4;
      PORTA = 0x81;
      delay(kDelay);
      while (i--) {
        PORTA = ((PORTA & 0xf0) >> 1) | ((PORTA & 0xf) << 1);
        delay(kDelay);
      }
      PORTA = 0;
      inByte=0;
    } else if (inByte == kAlgo2Code) {
      int i = 7;
      PORTA = 1;
      delay(kDelay);
      while (i >= 0) {
        PORTA <<= i--;
        delay(kDelay);
        PORTA >>= i--;
        delay(kDelay);
      }
      PORTA = 0;
      inByte=0;
    }
    Serial.print(inByte);
  }

  if(digitalRead(kAlgo1Button) == LOW) {
    Serial.write(kAlgo1Code);
    delay(650);
  }

  if(digitalRead(kAlgo2Button) == LOW) {
    Serial.write(kAlgo2Code);
    delay(650);
  }
}
