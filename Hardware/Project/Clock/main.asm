  .include "m328Pdef.inc"

  ldi r16, 0xFF
  out DDRD, r16

  ldi r16, 0xFF
  out DDRB, r16
;  ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
;  out TCCR0B, r16 
  ldi r16, 0x00 ; seconds (ones)
  ldi r17, 0x00 ; seconds (tens)
  ldi r18, 0x01 ; minutes (ones)
  ldi r19, 0x03 ; minutes (tens)
  ldi r20, 0x04 ; hours (ones)
  ldi r21, 0x01 ; hours (tens)
loop:



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
  ;rcall second_tens
  ;rcall hour_tens


  ;ldi r24, 0b01000000 ;times to run the loop = 64 for 1 second delay
  ;rcall PAUSE 		;call the PAUSE label


  ; Assuming a 16 MHz clock frequency (1 clock cycle = 62.5 ns)

  ldi r28, 0x08          ; Load r18 with a value to count down (128 iterations)
  ldi r29, 0x08          ; Load r19 with 128 (inner loop counter)
  ldi r30, 0x08          ; Load r20 with 128 (middle loop counter)

delay_loop:
  ; Display ones (seconds)
  lsl r16             ; Shift left r16 (seconds ones)
  lsl r16             ; Shift left r16 again
  out PORTD, r16      ; Output r16 to PORTD (seconds ones)
  ldi r25, 0x01       ; Load 0x01 into r25 for display on PORTB
  out PORTB, r25      ; Set PORTB to 0x01 (display for ones)
  lsr r16             ; Shift right r16 (preparing for next cycle)
  lsr r16             ; Shift right r16 again
  ldi r24, 0x00        ; Load 0x00 to r24 (turn off PORTB)
  out PORTB, r24       ; Output 0x00 to PORTB

  ; Display tens (seconds)
  lsl r17             ; Shift left r17 (seconds tens)
  lsl r17             ; Shift left r17 again
  out PORTD, r17      ; Output r17 to PORTD (seconds tens)
  ldi r25, 0x02       ; Load 0x02 into r25 for display on PORTB
  out PORTB, r25      ; Set PORTB to 0x02 (display for tens)
  lsr r17             ; Shift right r17 (preparing for next cycle)
  lsr r17             ; Shift right r17 again

  ldi r24, 0x00        ; Load 0x00 to r24 (turn off PORTB)
  out PORTB, r24       ; Output 0x00 to PORTB

  ; Display ones (minutes)
  lsl r18             
  lsl r18             
  out PORTD, r18      
  ldi r25, 0x04       
  out PORTB, r25      
  lsr r18             
  lsr r18            

  ldi r24, 0x00        
  out PORTB, r24       

  ; Display tens (minutes)
  lsl r19             
  lsl r19             
  out PORTD, r19      
  ldi r25, 0x08       
  out PORTB, r25      
  lsr r19             
  lsr r19            

  ldi r24, 0x00        
  out PORTB, r24

  ; Display ones (hours)
  lsl r20             
  lsl r20             
  out PORTD, r20      
  ldi r25, 0x10       
  out PORTB, r25      
  lsr r20             
  lsr r20            

  ldi r24, 0x00        
  out PORTB, r24


  ; Display ones (hours)
  lsl r21             
  lsl r21             
  out PORTD, r21 
  ldi r25, 0x20       
  out PORTB, r25      
  lsr r21             
  lsr r21            

  ldi r24, 0x00        
  out PORTB, r24
  


  dec r30; Decrement r20 (middle loop counter)
  cpi r30, 0x00
  brne delay_loop     ; If r20 is not zero, continue the loop
  dec r29             ; Decrement r19 (inner loop counter)
  cpi r29, 0x00
  brne delay_loop     ; If r19 is not zero, continue the loop
  cpi r29, 0x00
  dec r28             ; Decrement r18 (outer loop counter)
  brne delay_loop     ; If r18 is not zero, continue the loop

  ; This loop will run continuously for 1 second
  ; You can now add code here after the delay if needed
  rcall second_ones
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

;  cpi r16, 0x00
;  breq second_tens ; might have to replace ret with rjmp loop
;  if(C)
;    W = A
;  else
;    W = B
;  W = AC + B(C*)
  
;  if (second_ones == 0):
;    r22 = r17
;  else:
;    r22 = r22
;  ldi r17, r22

  ; C -> r23, C* -> r24
  ; r23, 0 if r16 is 0 1 otherwise

  ldi r23, 0x00
  ldi r24, 0xFF

; Assume R16 contains the value to check
; R23 will be set to 0x00 if R16 is 0, and 0xFF if R16 is non-zero

    ; Clear R23 (set it to 0x00)
    CLR     R23              ; R23 = 0x00

    ; Perform a bitwise AND with itself to check if R16 is zero
    ; If R16 is 0, the result of R16 AND R16 will be 0; otherwise, it will be non-zero.
    MOV     R25, R16         ; Copy R16 to R25 (temporary register)
    AND     R25, R16         ; R25 = R16 & R16 (this just checks if R16 is 0 or non-zero)

    ; Now, if R25 is non-zero (i.e., R16 was non-zero), set R23 to 0xFF
    ; We will use a bitwise OR to ensure R23 becomes 0xFF if R16 is non-zero
    OR      R23, R25         ; OR R23 with R25, if R16 was non-zero, this makes R23 = 0xFF

    ; Done!

;    MOV     r23, r16      ; Copy r16 into r23
;    AND     r23, r23      ; Perform bitwise AND of r23 with itself (results in 0 if r16 is 0)
;    COM     r23           ; Complement r23
;  MOV     r23, r16      ; Copy r16 into r23
;  OR      r23, r23      ; Perform OR of r23 with itself (sets Z flag if r16 is 0)
;  COM     r23           ; Complement r23  mov     r23, r16      ; Copy the value of r16 into r23
  
;  OR      r23, r23      ; Perform bitwise OR of r23 with itself
;  COM     r23           ; Complement r23 to invert the bits
;  AND    r23, r24     ; Mask to keep r23 as an 8-bit value  rjmp loop

  mov r24, r23
  COM r24
  
  mov r22, r17
  rcall second_tens

  and r22, r24
  and r17, r23

  add r17, r22


;
;
;  ldi r17, r22
  ;ldi r22, r16
  


 
  


second_tens:
  ldi r24, 0x01
  ; Z
  mov r30, r17
  and r30, r24

  ; Y
  lsr r17
  mov r29, r17
  ; lsl r24
  and r29, r24
  ; X
  lsr r17
  mov r28, r17
  ;lsl r24
  and r28, r24
  ; X
  lsr r17
  mov r27, r17
  ;lsl r24
  and r27, r24 

  ;ldi r24, 0x01
  clr r17

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

  add r17, r26

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

  add r17, r26

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

  add r17, r26

  cpi r17, 0x00
  breq minute_ones ; might have to replace ret with rjmp loop

  rjmp loop

minute_ones:

  ; W, X, Y, Z = r27, r28, r29, r30

  ldi r24, 0x01
  ; W
  mov r27, r18
  and r27, r24

  ; X
  lsr r18
  mov r28, r18
  ; lsl r24
  and r28, r24
  ; Y
  lsr r18
  mov r29, r18
  ;lsl r24
  and r29, r24
  ; Z
  lsr r18
  mov r30, r18
  ;lsl r24
  and r30, r24 

  ;ldi r24, 0x01
  clr r18

  ; A = !W
  mov r26, r27
  eor r26, r24 
  add r18, r26

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
  add r18, r26
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

  add r18, r26

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

  add r18, r26

  cpi r18, 0x00
  breq minute_tens ; might have to replace ret with rjmp loop

  rjmp loop

minute_tens:
  ldi r24, 0x01
  ; Z
  mov r30, r19
  and r30, r24

  ; Y
  lsr r19
  mov r29, r19
  ; lsl r24
  and r29, r24
  ; X
  lsr r19
  mov r28, r19
  ;lsl r24
  and r28, r24
  ; X
  lsr r19
  mov r27, r19
  ;lsl r24
  and r27, r24 

  ;ldi r24, 0x01
  clr r19

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

  add r19, r26

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

  add r19, r26

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

  add r19, r26

  cpi r19, 0x00
  breq hour_ones ; might have to replace ret with rjmp loop

  rjmp loop

hour_ones:

  ; W, X, Y, Z = r27, r28, r29, r30

  ldi r24, 0x01
  ; W
  mov r27, r20
  and r27, r24

  ; X
  lsr r20
  mov r28, r20
  ; lsl r24
  and r28, r24
  ; Y
  lsr r20
  mov r29, r20
  ;lsl r24
  and r29, r24
  ; Z
  lsr r20
  mov r30, r20
  ;lsl r24
  and r30, r24 

  ;ldi r24, 0x01
  clr r20

  ; A = !W
  mov r26, r27
  eor r26, r24 
  add r20, r26

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
  add r20, r26
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

  add r20, r26

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

  add r20, r26

  cpi r20, 0x00
  breq hour_tens ; might have to replace ret with rjmp loop

  rjmp loop
hour_tens:
  ldi r24, 0x01
  eor r21, r24
  ret

  ;
  ;PAUSE:	;this is delay (function)
  ;
  ;lp2:	;loop runs 64 times
  ;
  ;  IN r23, TIFR0 ;tifr is timer interupt flag (8 bit timer runs 256 times)
  ;  ldi r22, 0b00000010
  ;  AND r23, r22 ;need second bit
  ;
  ;
  ;
  ;  BREQ PAUSE 
  ;  OUT TIFR0, r22	;set tifr flag high
  ;  dec r24
  ;  brne lp2
  ;
  ;  ; seconds (ones)
  ;  lsl r16
  ;  lsl r16
  ;  ldi r25, 0x01
  ;  out PORTB, r25
  ;  out PORTD, r16
  ;  lsr r16
  ;  lsr r16 
  ;
  ;  ; seconds (tens)
  ;  lsl r17
  ;  lsl r17
  ;  ldi r25, 0x02
  ;  out PORTB, r25
  ;  out PORTD, r17
  ;  lsr r17
  ;  lsr r17
  ;
  ;  ; seconds (ones)
  ;  lsl r16
  ;  lsl r16
  ;  ldi r25, 0x01
  ;  out PORTB, r25
  ;  out PORTD, r16
  ;  lsr r16
  ;  lsr r16
  ;  ret





