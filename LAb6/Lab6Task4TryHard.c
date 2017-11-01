/*;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 1DT301, Computer Technology I
; Date: 2017-10-17
; Author:
; Student name 1: Ruth Dirnfeld
; Student name 2: Alexandra Bjäremo
;
; Lab number: 6
; Title: CyberTech Wall Display
;
; Hardware: STK600, CPU ATmega2560
;
; Function: Displaying characters on all the CyberTech Wall Display lines
; and changing the last line after 5 seconds.
;
; Input ports: TX, RX on PIND2, respective PIND3.
;
; Output ports: CyberTech Wall Display connected to serial port RS232
;
; Subroutines: Initialization and transmission routines for the display
; Included files: m2560def.inc
;
; Other information: Clock set at 1,83MHz
;
; Changes in program: None.
;<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include<avr/io.h>
#include<stdio.h>
#include <stdlib.h>
#define F_CPU 1830000UL
#include <util/delay.h>

//#define BAUD 2400
//#define UBRR_VAL 47

void uart_int(void);
void uart_trans(unsigned char data);

int main(void){
	uart_int();
	char towrite[200];
	int i;
	int checksum=0;
	
	//Faze 1: selection message
	char* temp ="\rAO0001Select line 0-2";
	for (i=0;i<(sizeof(temp)+1);i++){
		checksum+=temp[i];
		}
	checksum=checksum%256;
	sprintf(towrite,"%s%02X\n", temp, checksum);
	for(i=0;i<(sizeof(towrite)+3;i++){
		uart_trans(towrite[i]);
		}
	    
	//deafult broadcasting command
	temp ="\rZD0013C\n";
	for(i=0;i<9;i++){
		uart_trans(temp[i]);
	}
	
	//Faze 2: character input
	checksum=0;
	
	
	//default broadcasting command
	temp ="\rZD0013C\n";
	for(i=0;i<9;i++){
		uart_trans(temp[i]);
		}
	return 0;
}
//initialization function
void uart_int(void){
	UBRR1L=25;
	UCSR1B =(1<<TXEN1) | (1<<RXEN1);
	
}
//transmission function
void uart_trans(unsigned char data){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = data;
}
	    
char uart_read(void){
	
}
