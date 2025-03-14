.include "m328Pdef.inc"

ldi r16, 0xFF
out DDRD, r16
ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
out TCCR0B, r16 
; ldi r20, 0b00001000
ldi r16, 0x00 ; seconds (ones)
loop:
  lsl r16
  lsl r16
  out PORTD, r16
  lsr r16
  lsr r16
  rcall second_ones
  ; cpi r16, 0b00000110 ; checking seconds tens
  ; breq eqal_tens

  ldi r24, 0b01000000 ;times to run the loop = 64 for 1 second delay
  rcall PAUSE 		;call the PAUSE label
  rjmp loop

second_ones:

; W, X, Y, Z = r27, r28, r29, r30
 
  ldi r18, 0b00000001
  ; W
  mov r27, r16
  and r27, r18
  
  ; X
  lsr r16
  mov r28, r16
  ; lsl r18
  and r28, r18
  ; Y
  lsr r16
  mov r29, r16
  ;lsl r18
  and r29, r18
  ; Z
  lsr r16
  mov r30, r16
  ;lsl r18
  and r30, r18 

  ;ldi r18, 0b00000001
  clr r16

  ; A = !W
  mov r26, r27
  eor r26, r18 
  add r16, r26

  ; B = (!W && X && !Z) || (W && !X && !Z)

  ; (!W && X && !Z)
  and r26, r28 
  mov r25, r30
  eor r25, r18 
  and r26, r25

  ; (W && !X && !Z)
  and r25, r27
  mov r15, r28
  eor r15, r18 
  and r25, r15

  or r26, r25

  lsl r26 
  add r16, r26

  ; C = (!W && Y && !Z) || (!X && Y && !Z) || (W && X && !Y && !Z)

  ; (!W && Y && !Z)
  mov r26, r27 
  eor r26, r18 
  and r26, r29
  mov r25, r30
  eor r25, r18 
  and r26, r25 

  ; (!X && Y && !Z)
  ;and r25, r29

  mov r15, r28 
  eor r15, r18
  and r15, r29
  and r15, r25
  ;and r25, r15
  
  or r26, r15 

  ; (W && X && !Y && !Z)

;  mov r25, r30
;  eor r25, r18 
;  and r25, r27
;  and r25, r28
;  mov r15, r29 
;  eor r15, r18 
;  and r25, r15

  mov r15, r29
  eor r15, r18
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
  eor r26, r18 
  and r26, r27 
  and r26, r28
  and r26, r29

  ; (!W && !X && !Y && Z)
  mov r25, r27
  eor r25, r18 
  and r25, r30
  mov r15, r28
  eor r15, r18 
  and r25, r15
  mov r15, r29
  eor r15, r18 
  and r25, r15

  or r26, r25

  lsl r26
  lsl r26
  lsl r26

  add r16, r26
  
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

