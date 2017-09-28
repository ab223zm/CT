
.include "m2560def.inc"

.org 0x00
rjmp start

.org INT0addr
jmp interrupt_0
.org INT1addr
jmp interrupt_1

.org 0x72

start:
ldi r16, HIGH(RAMEND)	; MSB part av address to RAMEND
out SPH, r16			; store in SPH
ldi r16, LOW(RAMEND)	; LSB part av address to RAMEND
out SPL, r16			; store in SPL

ldi r20, 0x00
out DDRD, r20			; all zero's to DDRD, input

ldi r20, 0xFF
out DDRB, r20			; all one's to DDRB, outputs

ldi r17, 0xA0			; temp for eor
ldi r18, 0x0A			; temp for eor 

ldi r20, 0x03			; INT0 and INT1 enabled
out EIMSK, r20

ldi r20, 0x08			; INT1 falling edge, INT0 rising edge
sts EICRA, r20

sei						; global interrupt enable

main:
ldi r22, 0x3C			
out PORTB, r22			; normal light-up of LEDs

rjmp main

;turning right
interrupt_0:
ldi r22, 0x37			; starting ring for turning right
out PORTB, r22
ror r22					; shifting which LEDs are on
eor r22, r17			; XOR r22 with r17 to get back bits 4-7
out PORTB, r22			; outing to the LEDs

reti

;turning left
interrupt_1:
ldi r22, 0xEC			; starting ring for turning left
out PORTB, r22
rol r22					; shifting which LEDs are on
eor r22, r18			; XOR r22 with r18 to get back bits 0-3
out PORTB, r22			; outing to the LEDs
/*
ldi r19, 200
delay_int:
	dec r19
	cpi r19,0
	brne delay_int
	*/
reti

