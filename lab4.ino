#define F_CPU 4915200UL

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define DIGIT         PORTC
#define SEGMENT       PORTB
#define SEC_INCREMENT 0.053
#define SAVE_COUNT    10

typedef unsigned long ulong;
typedef unsigned int  uint;

enum Button {
  B1 = 0b00011100;
  B2 = 0b00011010;
  B3 = 0b00010110;
  B4 = 0b00001110;
};

enum Segment {
  S1 = 0b00000100;
  S2 = 0b00001000;
  S3 = 0b00010000;
  S4 = 0b00100000;
  S5 = 0b01000000;
  S6 = 0b10000000;
};

const uint kDigits[10] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x7, 0x7f, 0x6f};

struct State {
  bool isRunning = false;
  bool isButtonPressed = false;
  
  uint segment = 1;
  double seconds = 0;

  ulong savedTime[SAVE_COUNT];
  uint savedTimeIndex = 0;

  void printTime() {
    ulong hour = (ulong)seconds / 3600;
    ulong min  = ((ulong)seconds / 60) % 60;
    ulong sec  = (ulong)seconds % 60;

    switch (segment) {
      case 1: {
        SEGMENT = S1;
        DIGIT = kDigits[hour / 10];
        break;
      }
      case 2: {
        SEGMENT = S2;
        DIGIT = kDigits[hour % 10];
        break;
      }
      case 3: {
        SEGMENT = S3;
        DIGIT = kDigits[min / 10];
        break;
      }
      case 4: {
        SEGMENT = S4;
        DIGIT = kDigits[min % 10];
        break;
      }
      case 5: {
        SEGMENT = S5;
        DIGIT = kDigits[sec / 10];
        break;
      }
      case 6: {
        SEGMENT = S6;
        DIGIT = kDigits[sec % 10];
        break;
      }
    }

    _delay_ms(100);
      
    segment++;
    
    if (segment > 6) segment = 1;
  }

  void buzz() {
    DDRD  |= (1 << 0);
    PORTD |= (1 << 0);
    delay(500);
    DDRD  &= ~(1 << 0);
    PORTD &= ~(1 << 0);
  }

  void checkButton() {
    if (PINA != 0b00011110) {
      if (isButtonPressed) {
        return;
      }
      
      isButtonPressed = true;
      
      switch (PINA) {
        case B1: button1(); break;
        case B2: button2(); break;
        case B3: button3(); break;
        case B4: button4(); break;
      }
    } else {
      isButtonPressed = false;
    }
  }

  void button1() {
    if (isRunning) {
      isRunning = false;
    } else {
      isRunning = true;
      seconds = 0;
    }
  }

  void button2() {
    if (savedTimeIndex < SAVE_COUNT) {
      savedTime[savedTimeIndex++] = seconds;
    }
  }

  void button3() {
    savedTimeIndex = 0;
  }

  void button4() {
    if (!isRunning) {
      double tmp = seconds;
      for (uint i = 0; i < savedTimeIndex; i++) {
        seconds = savedTime[i];
        _delay_ms(500);
      }
      seconds = tmp;
    }
  }

} state;

ISR(TIMER0_OVF_vect) {
  if (state.isRunning == true) {
    state.seconds += SEC_INCREMENT;
    if ((ulong)state.seconds % 60 == 0) {
      state.buzz();
    }
  }
}

ISR(TIMER2_OVF_vect) {
  state.printTime();
}

int main() {
  DDRC = 0xff;       // Digit
  DDRB = 0b11111100; // Segment
  // Buttons
  DDRA = 0;
  PORTA = 0b00011110;
  
  cli();

  // Timer0 Phase Correct & /1024 with overflow interrupt
  TCCR0 |= (1 << CS02) | (1 << CS00);
  TCNT0 = 0;
  TIMSK |= (1 << TOIE0);

  // Timer2 CTC & /1024 
  TCCR2 |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  TCNT2 = 0;
  TIMSK |= (1 << TOIE2);

  sei();

  while (1) {
    state.checkButton();
  }
}
