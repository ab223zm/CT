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
	
	//first two lines, faze 1
	char* temp ="\rAO0001Computer Science,201712345678Computer Technology";
	int i;

	int checksum=0;
	for (i=0;i<55;i++){
		checksum+=temp[i];
		}
	checksum=checksum%256;
	sprintf(towrite,"%s%02X\n", temp, checksum);
	for(i=0;i<58;i++){
		uart_trans(towrite[i]);
		}
		
	checksum=0;
	//last line, faze 1
	temp="\rBO0001Assignment#6";
	for (i=0;i<20;i++){
		checksum+=temp[i];
	}
	checksum=checksum%256;
	sprintf(towrite,"%s%02X\n", temp, checksum);
	for(i=0;i<23;i++){
		uart_trans(towrite[i]);
	}
	//deafult broadcasting command
	temp ="\rZD0013C\n";
	for(i=0;i<9;i++){
		uart_trans(temp[i]);
	}
	
	_delay_ms(5000);
	//faze 2, changing last line
	checksum=0;
	temp="\rBO0001Ruth and Alex";
	for (i=0;i<21;i++){
		checksum+=temp[i];
		}
	checksum=checksum%256;
	sprintf(towrite,"%s%02X\n", temp, checksum);
	for(i=0;i<24;i++){
		uart_trans(towrite[i]);
		}
	
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
