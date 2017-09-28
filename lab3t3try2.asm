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
ldi r16, HIGH(RAMEND)	; MSB part av address to RAMEND
out SPH, r16			; store in SPH
ldi r16, LOW(RAMEND)	; LSB part av address to RAMEND
out SPL, r16			; store in SPL

ldi r20, 0xFF
out DDRB, r20

ldi r20, 0x00
out DDRD, r20

ldi r20, 0x03			; INT0 and INT1 enabled
out EIMSK, r20

ldi r20, 0x08			; INT1 falling edge, INT0 rising edge
sts EICRA, r20

ldi r24, 0x3C
out PORTB, r24

sei						; global interrupt enable


main:
	nop
rjmp main

interrupt_0:
	ldi r21, 0xEC
	out PORTB, r21

	ldi r17, 0x05

	tleft:
		out PORTB, r21
		cpi r21, 0xFC
		breq equal
		com r21
		lsl r21
		com r21
		eor r21,r17
	;rcall delay
	rjmp tleft

	equal:
		ldi r21, 0xEC
	rjmp tleft

reti

interrupt_1:
	ldi r22, 0x37
	out PORTB, r22

	ldi r23, 0xA0

	tright:
		out PORTB, r22
		cpi r22, 0x3F
		breq equal2
		com r22
		lsr r22
		com r22
		eor r22,r23
	;rcall delay
	rjmp tright

	equal2:
		ldi r22, 0x37
	rjmp tright

reti

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
