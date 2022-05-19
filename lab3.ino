#include <LiquidCrystal.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BUZZER     A7
#define SAVE_COUNT 60

enum Key {
  K1 = B11101110,
  K2 = B11011110,
  K3 = B10111110,
  KA = B01111110,

  K4 = B11101101,
  K5 = B11011101,
  K6 = B10111101,
  KB = B01111101,

  K7 = B11101011,
  K8 = B11011011,
  K9 = B10111011,
  KC = B01111011,

  KF = B11100111,
  K0 = B11010111,
  KE = B10110111,
  KD = B01110111,
};

struct KeyPadState {
  volatile byte *ddr, *port, *pin;
  bool isPressed = false;

  void init(byte* pddr, byte* pport, byte* ppin) {
    ddr  = pddr;
    port = pport;
    pin  = ppin;
    *ddr =  0b00001111;
    *port = 0b11110000;
  }

  char getKey() {
    char key = 0;
    if (*pin != 0b11110000) {
      if (isPressed) {
        return 0;
      }
      isPressed = true;
      *port = 0b11111110;
      for (int i = 0; i < 5; i++) {
        if (((*port = ~(1 << i))) != *pin) break;
      }

      switch (*pin) {
        case K1: key = '1'; break;
        case K2: key = '2'; break;
        case K3: key = '3'; break;
        case K4: key = '4'; break;
        case K5: key = '5'; break;
        case K6: key = '6'; break;
        case K7: key = '7'; break;
        case K8: key = '8'; break;
        case K9: key = '9'; break;
        case K0: key = '0'; break;
        case KA: key = 'A'; break;
        case KB: key = 'B'; break;
        case KC: key = 'C'; break;
        case KD: key = 'D'; break;
        case KE: key = 'E'; break;
        case KF: key = 'F'; break;
        default: {
          isPressed = false;
        }
      }
      *port = 0b11110000;
    } else {
      isPressed = false;
    }
    return key;
  }
} keyPadState;

struct Time {
  byte hour, minute, second;

  Time() {
    reset();
  }

  void reset() {
    hour = 0;
    minute = 0;
    second = 0;
  }

  bool isZero() {
    return hour == 0 && minute == 0 && second == 0;
  }

  bool addSecond() {
    if (++second == 60) {
      second = 0;
      if (++minute == 60) {
        minute = 0;
        if (++hour == 24) {
          hour = 0;
        }
      }
      return true;
    }
    return false;
  }

  const char* toString() const {
    static char str[9] = {0};
    snprintf(str, 9, "%02d:%02d:%02d", hour, minute, second);
    str[8] = 0;
    return str; // HH:MM:SS
  }

} timePoints[SAVE_COUNT], *timePoint = timePoints;

enum State {
  STATE_START,
  STATE_STOP,
  STATE_MENU
};

struct Timer {
  State state = STATE_STOP;
  int viewedTimePoint = 0;
  LiquidCrystal lcd = LiquidCrystal(A6, A5, A4, A0, A1, A2, A3);

  void start() {
    state = STATE_START;
    printTime();
  }

  void stop() {
    state = STATE_STOP;
    printTime();
  }

  void toggleStart() {
    if (state == STATE_START) {
      stop();
    } else if (state == STATE_STOP || state == STATE_MENU) {
      start();
    }
  }

  void buzz(int ms) {
    digitalWrite(BUZZER, HIGH);
    delay(ms);
    digitalWrite(BUZZER, LOW);  
  }

  void printTime() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(timePoint->toString());
    if (state == STATE_START) {
      printStart();
    } else {
      printStop();
    }
  }

/*
----------------
MENU     CNT: 10
00:00:00 CUR: 05
*/

  void printMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MENU");
    lcd.setCursor(9, 0);
    lcd.print("CNT: ");
    lcd.print(timePoint - timePoints);
    lcd.setCursor(0, 1);
    if (viewedTimePoint > 0) {
      lcd.print(timePoints[viewedTimePoint-1].toString());
    } else {
      lcd.print(Time().toString());
    }
    lcd.setCursor(9, 1);
    lcd.print("CUR: ");
    lcd.print(viewedTimePoint);
  }

  void printNotMenu() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NOT IN MENU");
  }

  void printStart() {
    lcd.setCursor(0, 1);
    lcd.print("PRESS D TO STOP");
  }

  void printStop() {
    lcd.setCursor(0, 1);
    lcd.print("PRESS D TO START");
  }

  void printReset() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RESET");
  }

  void printSave() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAVE");
  }

  void printResetSaved() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RESET SAVE");
  }

  void printOverflow() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SAVE OVERFLOW");
  }

  void printInvalidPage() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("INVALID PAGE");
  }

  void printInvalidInput() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("INVALID INPUT");
  }

  void printSearch(const char* page) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SEARCH");
    lcd.setCursor(0, 1);
    lcd.print(String(page));
  }

} timer;

ISR(TIMER5_COMPA_vect) {
  if (timer.state == STATE_START) {
    if (timePoint->addSecond()) {
      timer.buzz(400);
    }
    timer.printTime();
  }
}

void setup() {
  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  noInterrupts();
  TCCR5A = WGM50;
  TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50); // CTC mode & Prescaler @ 1024
  TIMSK5 = (1 << OCIE5A);                            // Permition for interruption
  OCR5A = 0x3D08;                                    // 1 sec (16MHz AVR)
  interrupts();
  
  timer.lcd.begin(16, 2);
  keyPadState.init(&DDRC, &PORTC, &PINC);
  timer.stop();
}

void loop() {
  if (char key = keyPadState.getKey()) {
    switch (key) {
      case 0:
        break;
      case 'A': {
        if (timer.state == STATE_MENU) {
          timer.viewedTimePoint = 0;
          timer.stop();
        } else {
          timer.state = STATE_MENU;
          timer.printMenu();
        }
        break;
      }
      case 'B': {
        if (timer.state == STATE_MENU) {
          if (timer.viewedTimePoint > SAVE_COUNT) {
            timer.printOverflow();
            delay(200);
            timer.viewedTimePoint = 0;
          } else {
            if (timer.viewedTimePoint >= timePoint - timePoints) {
              timer.viewedTimePoint = 0;
              timer.printInvalidPage();
            } else {
              timer.viewedTimePoint++;
            }
            delay(200);
          }
          timer.printMenu();
        } else {
          timer.printNotMenu();
          delay(200);
        }
        break;
      }
      case 'C': {
        timer.stop();
        timePoint->reset();
        timer.printReset();
        delay(200);
        timer.printTime();
        break;
      }
      case 'D': {
        timer.toggleStart();
        break;
      }
      case 'E': {
        if (!timePoint->isZero()) {
          if (timePoint - timePoints + 1 < SAVE_COUNT) {
            Time tmp = *timePoint;
            timePoint++;
            *timePoint = tmp;
            timer.printSave();
            delay(200);
            timer.printTime();
          } else {
            timer.printOverflow();
            timer.buzz(300);
          }
        }
        break;
      }
      case 'F': {
        Time tmp = *timePoint;
        timePoint = timePoints;
        *timePoint = tmp;
        timer.printResetSaved();
        timer.viewedTimePoint = 0;
        delay(200);
        timer.printTime();
        break;
      }
      default: {
        if (timer.state == STATE_MENU) {
          char number[3] = {key, 0, 0};
          timer.printSearch(number);
          int numberIndex = 1;
          while (1) {
            if (char numKey = keyPadState.getKey()) {
              if (numKey == 0) {
                continue;
              } else if (numKey == 'E') {
                char* ptr = NULL;
                int parsed = strtol(number, &ptr, 10);
                if (parsed > timePoint - timePoints) {
                  timer.printInvalidPage();
                  timer.buzz(300);
                  timer.printMenu();
                } else {
                  timer.viewedTimePoint = parsed;
                  timer.printMenu();
                  timer.buzz(200);
                  delay(100);
                  timer.buzz(200);
                  timer.printMenu();
                }
                break;
              } else if (isdigit(numKey)) {
                if (numberIndex < 2) {
                  number[numberIndex++] = numKey;
                  timer.printSearch(number);
                } else {
                  timer.printInvalidInput();
                  timer.buzz(300);
                  timer.printMenu();
                  break;
                }
              } else {
                timer.printInvalidInput();
                timer.buzz(300);
                timer.printMenu();
                break;
              }

            }
          }
        }
      }
    }
  }
}