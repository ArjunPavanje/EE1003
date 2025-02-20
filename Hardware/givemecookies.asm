  .include "m328Pdef.inc"

  ; setting pins 2, 3, 4, 5 of Arduino as output
  ldi r16, 0b00111100
  out DDRD, r16

  ; setting pins 8, 9, 10, 11 of Arduino as input and pin 13 of Arduino as output
  ldi r16, 0b00100000
  out DDRB, r16


  ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
  out TCCR0B, r16 ;prescalar used = 1024. So new freq. of clock cycle = (16 MHz / 1024) = 16 kHz

  clr r27 ;output bits. we are only interested in bit 6 from the right.


  .def W = r17
  .def X = r18
  .def Y = r19
  .def Z = r20

  .def A = r21
  .def B = r22
  .def C = r23
  .def D = r24
  
  .def one = r29
  ldi one, 0b00000001
  .def temp = r25
  .def temp1 = r26

loop:
  ; reading input from portB
  in temp, PINB

  ; Putting the input recieved into registers W, X, Y, Z
  mov W, temp
  and W, one

  lsr temp
  mov X, temp
  and X, one 

  lsr temp
  mov Y, temp
  and Y, one

  lsr temp
  mov Z, temp
  and Z, one

; X-------------------------------------------------------------------X ;
  ; A = !W
  mov temp, W
  eor temp, one
  mov A, temp
; X-------------------------------------------------------------------X ;
  ; B = (!W && X && !Z) || (W && !X && !Z)

  ; First Part 
  ; !W
  mov temp, A 
  ; (!W) & X
  and temp, X
  ; !Z
  mov temp1, Z
  eor temp1, Z
  ; (!W & X) & Z
  and temp, temp1

  mov C, temp

  ; Second Part
  ; !Z & W
  and temp1, W
  ; !X
  mov temp, X
  eor temp, one
  ; (!Z & W) & !X
  and temp, temp1

  or C, temp
; X-------------------------------------------------------------------X ;
  ; C =  (!W && Y && !Z) || (!X && Y && !Z) || (W && X && !Y && !Z); 

  ;Second Part
  ; !X
  mov temp, X
  eor temp, one
  ; (!X) & Y
  and temp, Y
  ; !Z
  mov temp1, Z
  eor temp1, one
  ; (!W & Y) & (!Z)
  mov temp, temp1
  mov C, temp

  ; Third Part
  ; !Y
  mov temp, Y
  eor temp, one
  ; W && X && !Y && !Z
  and temp, temp1
  and temp, X
  and temp, W

  or C, temp

  ;First Part
  ; (!Z) & Y & !W
  and temp1, Y
  and temp1, A 

  or C, temp1

; X-------------------------------------------------------------------X ;
  ; D =   (W && X && Y && !Z) || (!W && !X && !Y && Z)

  ; First Part
  ; !Z
  mov temp, Z
  eor temp, one
  ; !Z & W & X & Y
  and temp, Y
  and temp, X
  and temp, W

  mov C, temp

  ;Second Part
  ;!X
  mov temp1, X
  eor temp1, one
  ; !Y
  mov temp, Y
  eor temp, one
  ; !W & !X & !Y & Z
  and temp, temp1
  and temp, A 
  and temp, Z

  or C, temp
; X-------------------------------------------------------------------X ;
  ; left shifting B (by 1), C (by 2), D(by 3)
  lsl B 

  lsl C
  lsl C

  lsl D
  lsl D
  lsl D

  mov temp, A 
  add temp, B 
  add temp, C 
  add temp, D 

  ; left shifting temp twice as 0, 1 pins of Arduino are not defined to be output pins
  lsl temp 
  lsl temp

  out PORTD, temp


  ldi TEMP, 0b00100000	;initializing
  eor r27, TEMP ;change the output of LED
  out PORTB, r27 

  ldi r28, 0b01000000 ;times to run the loop = 64 for 1 second delay
  rcall PAUSE 		;call the PAUSE label
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











