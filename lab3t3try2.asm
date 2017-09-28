; in the simulator it only goes to the left
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

ldi r24, 0x3C		
out PORTB, r24				; default lights

sei					; global interrupt enable


main:
	nop
rjmp main

;turning left (while holding?)
interrupt_0:	
	ldi r21, 0xEC			; xxx0 xx00
	out PORTB, r21

	ldi r17, 0x05			; help register

	tleft:
		out PORTB, r21		; outputting first to not skip initial position
		cpi r21, 0xFC		; checking if the ring is complete
		breq equal
		com r21			; inverting the bits
		lsl r21			; pushing one's from the right side
		com r21			; inverting again
		eor r21,r17		; XOR to get lower nibble back to 1100
	;rcall delay
	rjmp tleft			; loop it

	equal:
		ldi r21, 0xEC		; reset to initial position
	rjmp tleft

reti					; return from interrupt

;turning right(w h?)
interrupt_1:
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
	;rcall delay
	rjmp tright			; loop it

	equal2:
		ldi r22, 0x37		; reset to initial position
	rjmp tright

reti					; return from interrupt

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
