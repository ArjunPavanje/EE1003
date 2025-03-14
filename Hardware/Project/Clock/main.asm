.include "m328Pdef.inc"

ldi r16, 0xFF
out DDRD, r16

ldi r16, 0xFF
out DDRB, r16
ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
out TCCR0B, r16 
; ldi r20, 0b00001000
ldi r16, 0x01 ; seconds (ones)
ldi r17, 0x00 ; seconds (tens)
ldi r18, 0x00 ; minutes (ones)
ldi r19, 0x00 ; minutes (tens)
ldi r20, 0x00 ; hours (ones)
ldi r21, 0x00 ; hours (tens)
loop:
  ; seconds (ones)
  lsl r16
  lsl r16
  ldi r24, 0x01
  out PORTB, r24
  out PORTD, r16
  lsr r16
  lsr r16

 ; seconds (tens)
  lsl r17
  lsl r17
  ldi r24, 0x02
  out PORTB, r24
  out PORTD, r17
  lsr r17
  lsr r17

; ;  minutes (ones)
;  lsl r18
;  lsl r18
;  ldi r24, 0x03
;  out PORTB, r24
;  out PORTD, r18
;  lsr r18
;  lsr r18
;
; ; minutes (tens)
;  lsl r19
;  lsl r19
;  ldi r24, 0x04
;  out PORTB, r24
;  out PORTD, r19
;  lsr r19
;  lsr r19
;
; ; hours (ones)
;  lsl r20
;  lsl r20
;  ldi r24, 0x05
;  out PORTB, r24
;  out PORTD, r20
;  lsr r20
;  lsr r20
;
; ;  hours (tens)
;  lsl r21
;  lsl r21
;  ldi r24, 0x06
;  out PORTB, r24
;  out PORTD, r21
;  lsr r21
;  lsr r21

  ;rcall second_ones
  rcall second_tens
  ;rcall hour_tens
  
  cpi r16, 0x00:
  breq second_tens ; might have to replace ret with rjmp loop

  ldi r24, 0b01000000 ;times to run the loop = 64 for 1 second delay
  rcall PAUSE 		;call the PAUSE label
  rjmp loop

second_ones:

; W, X, Y, Z = r27, r28, r29, r30
 
  ldi r24, 0x01
  ; W
  mov r27, r16
  and r27, r24
  
  ; X
  lsr r16
  mov r28, r16
  ; lsl r24
  and r28, r24
  ; Y
  lsr r16
  mov r29, r16
  ;lsl r24
  and r29, r24
  ; Z
  lsr r16
  mov r30, r16
  ;lsl r24
  and r30, r24 

  ;ldi r24, 0x01
  clr r16

  ; A = !W
  mov r26, r27
  eor r26, r24 
  add r16, r26

  ; B = (!W && X && !Z) || (W && !X && !Z)

  ; (!W && X && !Z)
  and r26, r28 
  mov r25, r30
  eor r25, r24 
  and r26, r25

  ; (W && !X && !Z)
  and r25, r27
  mov r15, r28
  eor r15, r24 
  and r25, r15

  or r26, r25

  lsl r26 
  add r16, r26
  ; C = (!W && Y && !Z) || (!X && Y && !Z) || (W && X && !Y && !Z)

  ; (!W && Y && !Z)
  mov r26, r27 
  eor r26, r24 
  and r26, r29
  mov r25, r30
  eor r25, r24 
  and r26, r25 

  ; (!X && Y && !Z)
  ;and r25, r29

  mov r15, r28 
  eor r15, r24
  and r15, r29
  and r15, r25
  ;and r25, r15
  
  or r26, r15 

  ; (W && X && !Y && !Z)

;  mov r25, r30
;  eor r25, r24 
;  and r25, r27
;  and r25, r28
;  mov r15, r29 
;  eor r15, r24 
;  and r25, r15

  mov r15, r29
  eor r15, r24
  and r15, r25
  and r15, r27
  and r15, r28

  or r26, r15

  lsl r26
  lsl r26

  add r16, r26

  ; D = (W && X && Y && !Z) || (!W && !X && !Y && Z)

  ; (W && X && Y && !Z)
  mov r26, r30
  eor r26, r24 
  and r26, r27 
  and r26, r28
  and r26, r29

  ; (!W && !X && !Y && Z)
  mov r25, r27
  eor r25, r24 
  and r25, r30
  mov r15, r28
  eor r15, r24 
  and r25, r15
  mov r15, r29
  eor r15, r24 
  and r25, r15

  or r26, r25

  lsl r26
  lsl r26
  lsl r26

  add r16, r26
  
  ret


second_tens:
  ldi r24, 0x01
  ; Z
  mov r30, r16
  and r30, r24
  
  ; Y
  lsr r16
  mov r29, r16
  ; lsl r24
  and r29, r24
  ; X
  lsr r16
  mov r28, r16
  ;lsl r24
  and r28, r24
  ; X
  lsr r16
  mov r27, r16
  ;lsl r24
  and r27, r24 

  ;ldi r24, 0x01
  clr r16

  ; A = 0 




; B = (X && !Y) || (!X && Y && Z)

  ; (X && !Y)
  mov r26, r29
  eor r26, r24
  and r26, r28

  ; (!X && Y && Z)
  mov r25, r28
  eor r25, r24
  and r25, r29
  and r25, r30

  or r26, r25

  lsl r26
  lsl r26

  add r16, r26

  ; C = X'YZ' + Y'Z

  ; X'YZ'
  mov r26, r28
  eor r26, r24
  and r26, r29
  mov r25, r30
  eor r25, r24
  and r26, r25

  ; Y'Z
  mov r15, r29
  eor r15, r24
  and r15, r30

  or r26, r15

  lsl r26

  add r16, r26

  ; D = Y'Z' + X'Z'

  ; Y'Z'
  mov r26, r29
  eor r26, r24
  mov r25, r30
  eor r25, r24
  and r26, r25

  ; X'Z'
  mov r15, r28
  eor r15, r24
  and r25, r15

  or r26, r25
  
  add r16, r26

  ret
hour_tens:
  ldi r24, 0x01
  eor r16, r24
  ret


PAUSE:	;this is delay (function)
lp2:	;loop runs 64 times
		IN r23, TIFR0 ;tifr is timer interupt flag (8 bit timer runs 256 times)
		ldi r22, 0b00000010
		AND r23, r22 ;need second bit
		BREQ PAUSE 
		OUT TIFR0, r22	;set tifr flag high
	dec r24
	brne lp2
	ret

equal_tens:
  clr r16

