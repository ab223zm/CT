/*
 * AVRAssembler6.asm
 *
 *  Created: 01/10/2017 08:57:06
 *   Author: alexb
 */ 


 /*
 * AVRAssembler5.asm
 *
 *  Created: 29/09/2017 00:14:28
 *   Author: alexb
 */ 

 .include"m2560def.inc"
 
.org 0x00
rjmp start

.org INT0addr
jmp interrupt_0
.org INT1addr
jmp interrupt_1

.org 0x72

start:
ldi r16, HIGH(RAMEND)			; MSB part av address to RAMEND
out SPH, r16				; store in SPH
ldi r16, LOW(RAMEND)			; LSB part av address to RAMEND
out SPL, r16				; store in SPL

ldi r20, 0xFF	
out DDRB, r20				; all one's to DDRB, output

ldi r20, 0x00
out DDRD, r20				; all zero's to DDRD, input

ldi r20, 0x03				; INT0 and INT1 enabled
out EIMSK, r20

ldi r20, 0x08				; INT1 falling edge, INT0 rising edge
sts EICRA, r20


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;figure out how to enable INT2;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


ldi r24, 0x3C		
out PORTB, r24				; default lights

ldi r30, 0x00				; help register for checking if the car is turning
ldi r31, 0x01				; help register for interrupt_0
ldi r32, 0x10				; help register for interrupt_1
ldi r33, 0x08				; help register for interrupt_2		


sei					; global interrupt enable


main:
cpi r30, 0xFE				; if true then it means the interrupt_o has been used
breq start_turning_right	
cpi r30, 0xEF				; if true then it means the interrupt_1 has been used
breq start_turning_left
cpi r30, 0xF7				; if true then it means the interrupt_2 has been used
breq breaks										
ldi r24, 0x3C				; r30 is 0x00, meaning the interrupts have either not been used 
out PORTB, r24				; or used again to stop the turning

rjmp main

start_turning_right:
ldi r22, 0x37			; 00xx 0xxx
	out PORTB, r22

	ldi r23, 0xA0			; help register

	tright:
		out PORTB, r22		; outputting first to not skip initial position
		cpi r22, 0x3F		; checking if the ring is complete
		breq equal2
		com r22			; inverting the bits
		lsr r22			; pushing one's from the left side
		com r22			; inverting again
		eor r22,r23		; XOR to get higher nibble back to 0011
	rcall delay
	rjmp tright			; loop it

	equal2:
		ldi r22, 0x37		; reset to initial position
	rjmp tright	

start_turning_left:
ldi r25, 0xEC			; xxx0 xx00
	out PORTB, r25

	ldi r17, 0x05			; help register

	tleft:
		out PORTB, r25		; outputting first to not skip initial position
		cpi r25, 0xFC		; checking if the ring is complete
		breq equal
		com r25			; inverting the bits
		lsl r25			; pushing one's from the right side
		com r25			; inverting again
		eor r25,r17		; XOR to get lower nibble back to 1100
	rcall delay
	rjmp tleft			; loop it

	equal:
		ldi r25, 0xEC		; reset to initial position
	rjmp tleft	

breaks:
ldi r26, 0x00
out PORTB, r26				; turn on all LEDs


interrupt_0:	
eor r30, r31			; result should be 0x01 (0000 0001)
com r30					; result should be 0xFE (1111 1110)

reti					; return from interrupt

interrupt_1:
eor r30, r32			; result should be 0x10 (0001 0000)
com r30					; result should be 0xEF (1110 1111)

reti					; return from interrupt

interrupt_2:
eor r30, r33			; result should be 0x08 (0000 1000)
com r30					; result should be 0xF7 (1111 0111)

delay:
    ldi  r18, 3
    ldi  r19, 138
    ldi  r21, 86	
L1:	dec  r21
    brne L1
	dec  r19
    brne L1
    dec  r18
    brne L1
    rjmp PC+1

ret


