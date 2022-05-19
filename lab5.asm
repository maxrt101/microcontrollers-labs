.include "m2560def.inc"

.def btn_1_div_cnt = r17
.def btn_1_mul_cnt = r18
.def btn_2_cnt     = r19
.def btn_1_pressed = r20
.def btn_2_pressed = r21

.DSEG                     ; Data Segment

.CSEG                     ; Code Segment
.org 0x0000               ; RESET
  jmp reset
.org 0x0064               ; T5 ISR
  jmp TIMER5_ISR


TIMER5_ISR:               ; Timer (T5) ISR
  cpi   btn_1_pressed,    1
  brne  btn_1_pressed_end
  call  algo1
btn_1_pressed_end:

  cpi   btn_2_pressed,    1
  brne  btn_2_pressed_end
  call  algo2
btn_2_pressed_end:

  reti


reset:                    ; Reset ISR Handler
  clr   r17
  clr   r18
  clr   r19
  clr   r20
  clr   r21

  ; Setup Stack
  ldi   r16,              low(RAMEND)
  out   SPL,              r16
  ldi   r16,              high(RAMEND)
  out   SPH,              r16

  ; Init PORTA
  ldi   r16,              0xFF
  out   DDRA,             r16
  out   DDRF,             r16

  ; Init PORTF
  ldi   r16,              0
  out   PORTA,            r16
  out   PORTF,            r16

  ; Init Buttons (PC5, PC7)
  out   DDRC,             r16
  ldi   r16,              0xFF
  out   PORTC,            r16

  ; Setup Timer (T5)
  cli
  clr   r16
  sts   TCCR5A,           r16
  sts   TCCR5B,           r16
  ldi   r16,              (1 << CS51) | (1 << CS50)
  sts   TCCR5B,           r16
  clr   r16
  ldi   r16,              (1 << TOIE5)
  sts   TIMSK5,           r16
  sei

  ldi   btn_1_div_cnt,    128
  ldi   btn_1_mul_cnt,    1


main:                     ; Main Function (automatically executed after reset)
  ; Button 1 check (PC7)
  sbic  PINC,             7
  jmp   btn_1_check_end
  ldi   btn_1_pressed,    1
  ldi   btn_1_div_cnt,    128
  ldi   btn_1_mul_cnt,    1
btn_1_check_end:

  ; Button 2 check (PC5)
  sbic  PINC,             5
  jmp   btn_2_check_end
  ldi   btn_2_pressed,    1
  ldi   btn_2_cnt,        1
btn_2_check_end:

  rjmp  main


algo1:                    ; Algorithm 1 Implementation
  mov   r16,              btn_1_div_cnt
  or    r16,              btn_1_mul_cnt  
  out   PORTF,            r16

  ldi   r16,              4
  rcall delay

  ldi   r16,              0
  out   PORTF,            r16

  lsl   btn_1_mul_cnt
  lsr   btn_1_div_cnt

  cpi   btn_1_div_cnt,    8
  brne  algo1_cnt_max
  ldi   btn_1_pressed,    0
  ldi   btn_1_div_cnt,    128
  ldi   btn_1_mul_cnt,    1
algo1_cnt_max:
  ret


algo2:                    ; Algorithm 2 Implementation
  out   PORTA,            btn_2_cnt

  ldi   r16,              4
  rcall delay

  ldi   r16,              0
  out   PORTA,            r16

  lsl   btn_2_cnt

  cpi   btn_2_cnt,        0
  brne  algo2_cnt_max
  ldi   btn_2_pressed,    0
  ldi   btn_2_cnt,        1
algo2_cnt_max:
  ret


; r16 - times to repeat the delay
delay:                    ; Delays execution by ~100ms for r16=1
  push  r24
  push  r25
  push  r26
  push  r27

  mov   r27,              r16

delay_outer_loop:
  ldi   r24,              0xFF
  ldi   r25,              0xFF
  ldi   r26,              1
delay_loop:
  subi  r24,              1
  sbci  r25,              0
  sbci  r26,              0
  brcc  delay_loop

  subi  r27,              1
  brcc  delay_outer_loop

  pop r24
  pop r25
  pop r26
  pop r27

  ret

.ESEG                     ; EEPROM Segment
