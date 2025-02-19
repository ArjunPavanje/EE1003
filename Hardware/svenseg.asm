; Seven Segment Display (Assembly)

.include "./m328Pdef.inc"

; Configuring pins 2-7 of Arduino (PD2-PD7 of) as output pins
  ldi r16, 0b11111100
  out DDRD, r16

; 'ldi' stands for load immediete 
; it loads an 8-bit constant into a register

; 'out' writes the value in a register to an i/o port
; 'DDRD' stands for Data Direction Register 'D'
; 'DDRD' command is used to configure 'direction' (input/output) of pins on port 'D'
; 1-output, 0-input

; Similarly setting pin 8 of Arduino (PB0) as output pin
  ldi r16, 0b00000001
  out DDRB, r16

; Writing a number (say 5) on the seven segment Display
  ldi r17, 0b010010000
  out PortD, r17
  
  ldi r17, 0b00000001
  out PortB, r17

; Equivalent of void loop
Start:
  rjmp Start
