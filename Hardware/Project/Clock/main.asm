  .include "m328Pdef.inc"

  ldi r16, 0xFF
  out DDRD, r16

  ldi r16, 0xFF
  out DDRB, r16
;  ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
;  out TCCR0B, r16 
  ldi r16, 0x00 ; seconds (ones)
  ldi r17, 0x00 ; seconds (tens)
  ldi r18, 0x05 ; minutes (ones)
  ldi r19, 0x04 ; minutes (tens)
  ldi r20, 0x03 ; hours (ones)
  ldi r21, 0x00 ; hours (tens)
loop:



    ldi r28, 0x08          ; Load r18 with a value to count down (128 iterations)
  ldi r29, 0x04          ; Load r19 with 128 (inner loop counter)
  ldi r30, 0x02          ; Load r20 with 128 (middle loop counter)

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
  ldi r23, 0x00
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

;  mov r22, r27
;  eor r22, r24
;  ;add r23, r22
;  lsl r24
;  mov r25, r28
;  eor r25, r24
;  and r22, r25
;  ;add r23, r22
;  lsl r24
;  mov r25, r29
;  eor r22, r24
;  add r23, r22
;  lsl r24
;  mov r22, r30
;  eor r22, r24
;  add r23, r24
;  
;  mov r22, r23
;  lsl r22
;  lsl r22
;  lsl r22
;  lsl r22

  ldi r24, 0x01	

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





cpi r16, 0x00
breq second_tens ; might have to replace ret with rjmp loop


 rjmp loop 


 
  


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




  ; B = !XYZ
  mov r26, r28
  eor r26, r24
  and r26, r29
  and r26, r30

  lsl r26
  lsl r26

  add r17, r26


;
;  ; (X && !Y)
;  mov r26, r29
;  eor r26, r24
;  and r26, r28
;
;  ; (!X && Y && Z)
;  mov r25, r28
;  eor r25, r24
;  and r25, r29
;  and r25, r30
;
;  or r26, r25
;
;  lsl r26
;  lsl r26
;
;  add r17, r26
;
  ; C = X'YZ' + Y'Z

  mov r26, r28
  eor r26, r24
  mov r25, r30
  eor r25, r24
  and r25, r26
  
  add r17, r25

  and r25, r29
  

  mov r15, r29
  eor r15, r24

  and r26, r15
  and r26, r30

  or r26, r25
  lsl r26

  add r17, r26
  

;  ; X'YZ'
;  mov r26, r28
;  eor r26, r24
;  and r26, r29
;  mov r25, r30
;  eor r25, r24
;  and r26, r25
;
;  ; Y'Z
;  mov r15, r29
;  eor r15, r24
;  and r15, r30
;
;  or r26, r15
;
;  lsl r26
;
;  add r17, r26
;
  ; D = Y'Z' + X'Z'

;  ; Y'Z'
;  mov r26, r29
;  eor r26, r24
;  mov r25, r30
;  eor r25, r24
;  and r26, r25
;
;  ; X'Z'
;  mov r15, r28
;  eor r15, r24
;  and r25, r15
;
;  or r26, r25
;
;  add r17, r26
;
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
  
  mov r26, r29
  eor r26, r24
  and r26, r30

  lsl r26
  add r20, r26

  mov r26, r29
  eor r26, r24

  mov r25, r30
  eor r25, r24
  and r26, r25

  add r20, r26

;  ; A = !W
;  mov r26, r27
;  eor r26, r24 
;  add r20, r26
;
;  ; B = (!W && X && !Z) || (W && !X && !Z)
;
;  ; (!W && X && !Z)
;  and r26, r28 
;  mov r25, r30
;  eor r25, r24 
;  and r26, r25
;
;  ; (W && !X && !Z)
;  and r25, r27
;  mov r15, r28
;  eor r15, r24 
;  and r25, r15
;
;  or r26, r25
;
;  lsl r26 
;  add r20, r26
;  ; C = (!W && Y && !Z) || (!X && Y && !Z) || (W && X && !Y && !Z)
;
;  ; (!W && Y && !Z)
;  mov r26, r27 
;  eor r26, r24 
;  and r26, r29
;  mov r25, r30
;  eor r25, r24 
;  and r26, r25 
;
;  ; (!X && Y && !Z)
;  ;and r25, r29
;
;  mov r15, r28 
;  eor r15, r24
;  and r15, r29
;  and r15, r25
;  ;and r25, r15
;
;  or r26, r15 
;
;  ; (W && X && !Y && !Z)
;
;  ;  mov r25, r30
;  ;  eor r25, r24 
;  ;  and r25, r27
;  ;  and r25, r28
;  ;  mov r15, r29 
;  ;  eor r15, r24 
;  ;  and r25, r15
;
;  mov r15, r29
;  eor r15, r24
;  and r15, r25
;  and r15, r27
;  and r15, r28
;
;  or r26, r15
;
;  lsl r26
;  lsl r26
;
;  add r20, r26
;
;  ; D = (W && X && Y && !Z) || (!W && !X && !Y && Z)
;
;  ; (W && X && Y && !Z)
;  mov r26, r30
;  eor r26, r24 
;  and r26, r27 
;  and r26, r28
;  and r26, r29
;
;  ; (!W && !X && !Y && Z)
;  mov r25, r27
;  eor r25, r24 
;  and r25, r30
;  mov r15, r28
;  eor r15, r24 
;  and r25, r15
;  mov r15, r29
;  eor r15, r24 
;  and r25, r15
;
;  or r26, r25
;
;  lsl r26
;  lsl r26
;  lsl r26
;
;  add r20, r26
;
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






