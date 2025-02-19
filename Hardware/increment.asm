  .include "m328Pdef.inc"

  ldi r16, 0xFF ; identifying all pins as ouput in register D
  out DDRD, r16 ; declaring pins as output in register D

  ldi r16, 0x00 ; identifying all pins as input in register B
  out DDRB, r16 ; declaring pins as input in register B


  ldi r17, 0b00000000
  ldi r18, 0b00000000
  ldi r19, 0b00000000
  ldi r20, 0b00000000

  .def W = r17
  .def X = r18
  .def Y = r19
  .def Z = r20

  .def A = r21
  .def B = r22
  .def C = r23
  .def D = r24

; X-------------------------------------------------------------------X ;
; A = !W
  ldi r25, 0b00000001
  eor r25, r17

  mov A, r25 
; X-------------------------------------------------------------------X ;

; B = (!W && X && !Z) || (W && !X && !Z)


; -> First Half

  ; !W
  ldi r25, 0b00000001
  eor r25, r17
  
  ; (!W) && X
  AND r25, r18 

  ; !Z
  ldi r26, 0b00000001
  eor r26, r20

  ; (!W && X) && (!Z)
  AND r25, r26

; -> Second Half

  ; !X
  ldi r26, 0b00000001
  eor r26, r18
  
  ; W && (!X)
  AND r26, r17 

  ; !Z
  ldi r27, 0b00000001
  eor r27, r20

  ; (W && !X) && (!Z)
  AND r26, r27

; -> Combining

  OR r25, r26
  mov B, r25

; X-------------------------------------------------------------------X ;


; D = (W && X && Y && !Z) || (!W && X && !Y && !Z)


; -> Second Half

  ; !W
  ldi r25, 0b00000001
  eor r25, r17
  
  ; (!W) && X
  AND r25, r18 

  ; !Y
  ldi r27, 0b00000001
  eor r27, r19

  ; !Z
  ldi r26, 0b00000001
  eor r26, r20

  ; (!Y) && (!Z)
  AND r26, r27

  ; (!W && X) && (!Y && !Z)
  AND r25, r26

; -> Second Half

  ; !Z
  ldi r26, 0b00000001
  eor r26, r20
  
  ; !Z && Y
  AND r26, r19 

  ; (!Z && Y) && X
  AND r26, r18

  ; (!Z && Y && X) && W
  AND r26, r17

; -> Combining

  OR r25, r26
  mov D, r25

; X-------------------------------------------------------------------X ;

; C = (!W && Y && !Z) || (!X && Y && !Z) || (W && X && !Y && !Z)


; -> First Half

  ; !W
  ldi r25, 0b00000001
  eor r25, r17
  
  ; (!W) && Y
  AND r25, r19 

  ; !Z
  ldi r26, 0b00000001
  eor r26, r20

  ; (!W && Y) && (!Z)
  AND r25, r26

; -> Second Half

  ; !X
  ldi r26, 0b00000001
  eor r26, r18
  
  ; (!X) && (Y)
  AND r26, r18 

  ; !Z
  ldi r27, 0b00000001
  eor r27, r20

  ; (Y && !X) && (!Z)
  AND r26, r27

; -> Third Half

  ; !Z
  ldi r27, 0b00000001
  eor r27, r20
  
  ; !Y
  ldi r28, 0b00000001
  eor r28, r19
  
  ; !Z && !Y
  AND r27, r28


  ; (!Z && !Y) && X
  AND r27, r18

  ; (!Z && !Y && X) && W
  AND r27, r17
; -> Combining

  OR r26, r27
  OR r25, r26

  mov C, r25

; X-------------------------------------------------------------------X ;


