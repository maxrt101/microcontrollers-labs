
const int kButton = 32; // PC5 Button Pin Number

void setup() {
   DDRA = 0xff; // Set all A pins to output mode
   PORTA = 0;   // Set all A pins to LOW
   pinMode(kButton, INPUT_PULLUP);
}

void loop() {
   if (digitalRead(kButton) == LOW) {
      int i = 7;
      PORTA = 1;
      delay(1000);
      while (i >= 0) {
        PORTA <<= i--;
        delay(1000);
        PORTA >>= i--;
        delay(1000);
      }
      PORTA = 0;
   }
}
