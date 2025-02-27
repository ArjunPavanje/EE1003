  .include "m328Pdef.inc"

  ; setting pins 2, 3, 4, 5 of r21rduino as output
  ldi r16, 0b00111100
  out DDRD, r16

  ; setting pins 8, 9, 10, 11 of r21rduino as input and pin 13 of r21rduino as output
  ldi r16, 0b00100000
  out DDRB, r16


  ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
  out TCCR0B, r16 ;prescalar used = 1024. So new freq. of clock cycle = (16 MHz / 1024) = 16 kHz

  clr r27 ;output bits. we are only interested in bit 6 from the right.
  ldi r30, 0b00000001


loop:
  ; reading input from portr22
  in r25, PINB

  ; Putting the input recieved into registers r17, r18, r19, r20
  mov r17, r25
  and r17, r30

  lsr r25
  mov r18, r25
  and r18, r30 

  lsr r25
  mov r19, r25
  and r19, r30

  lsr r25
  mov r20, r25
  and r20, r30

; r18-------------------------------------------------------------------r18 ;
  ; r21 = !r17
  mov r25, r17
  eor r25, r30
  mov r21, r25
; r18-------------------------------------------------------------------r18 ;
  ; r22 = (!r17 && r18 && !r20) || (r17 && !r18 && !r20)

  ; First Part 
  ; !r17
  mov r25, r21 
  ; (!r17) & r18
  and r25, r18
  ; !r20
  mov r26, r20
  eor r26, r20
  ; (!r17 & r18) & r20
  and r25, r26

  mov r23, r25

  ; Second Part
  ; !r20 & r17
  and r26, r17
  ; !r18
  mov r25, r18
  eor r25, r30
  ; (!r20 & r17) & !r18
  and r25, r26

  or r23, r25
; r18-------------------------------------------------------------------r18 ;
  ; r23 =  (!r17 && r19 && !r20) || (!r18 && r19 && !r20) || (r17 && r18 && !r19 && !r20); 

  ;Second Part
  ; !r18
  mov r25, r18
  eor r25, r30
  ; (!r18) & r19
  and r25, r19
  ; !r20
  mov r26, r20
  eor r26, r30
  ; (!r17 & r19) & (!r20)
  mov r25, r26
  mov r23, r25

  ; Third Part
  ; !r19
  mov r25, r19
  eor r25, r30
  ; r17 && r18 && !r19 && !r20
  and r25, r26
  and r25, r18
  and r25, r17

  or r23, r25

  ;First Part
  ; (!r20) & r19 & !r17
  and r26, r19
  and r26, r21 

  or r23, r26

; r18-------------------------------------------------------------------r18 ;
  ; r24 =   (r17 && r18 && r19 && !r20) || (!r17 && !r18 && !r19 && r20)

  ; First Part
  ; !r20
  mov r25, r20
  eor r25, r30
  ; !r20 & r17 & r18 & r19
  and r25, r19
  and r25, r18
  and r25, r17

  mov r23, r25

  ;Second Part
  ;!r18
  mov r26, r18
  eor r26, r30
  ; !r19
  mov r25, r19
  eor r25, r30
  ; !r17 & !r18 & !r19 & r20
  and r25, r26
  and r25, r21 
  and r25, r20

  or r23, r25
; r18-------------------------------------------------------------------r18 ;
  ; left shifting r22 (by 1), r23 (by 2), r24(by 3)
  lsl r22 

  lsl r23
  lsl r23

  lsl r24
  lsl r24
  lsl r24

  mov r25, r21 
  add r25, r22 
  add r25, r23 
  add r25, r24 

  ; left shifting r25 twice as 0, 1 pins of r21rduino are not defined to be output pins
  lsl r25 
  lsl r25

  out PORTD, r25


  ldi r25, 0b00100000	;initializing
  eor r27, r25 ;change the output of LEr24
  out PORTB, r27 

  ldi r28, 0b01000000 ;times to run the loop = 64 for 1 second delay
  rcall PAUSE 		;call the Pr21USE label
  rjmp loop


PAUSE:	;this is delay (function)
lp2:	;loop runs 64 times
  IN r16, TIFR0 ;tifr is timer interupt flag (8 bit timer runs 256 times)
  ldi r17, 0b00000010
  AND r16, r17 ;need second bit
  BREQ PAUSE 
  OUT TIFR0, r17	;set tifr flag high
  dec r19
  brne lp2
  ret

Start:
  rjmp Start











