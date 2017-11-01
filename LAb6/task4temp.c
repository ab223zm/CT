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
	char towrite[100];
		
	//first two lines, faze 1
	char* firstLine ="\rAO0001";
	char* secondLine="\rAO0001                        ";
	char* lastLine ="\rBO0001";
	int i,j;
	int checksum=0;
	
	while(1){
		char mssg = uart_read();
		if (mssg[0]=='1'){
			checksum=8;
			for (i=1;i<sizeof(mssg);i++)
				checksum+=mssg[i];
			checksum=checksum%256;
			sprintf(towrite,"%s%02X\n", firstLine,mssg,checksum);
			for (i=0; i<(sizeof(towrite)+1);i++)
				uart_trans(towrite[i]);
		}
		else if (mssg[0]=="2"){
			checksum=31;
			for (i=1,i<sizeof(mssg);i++)
				checksum+=mssg[i];
			checksum=checksum%256;
			sprintf(towrite,"%s%02X\n", secondLine,mssg,checksum);
			for (i=0; i<(sizeof(towrite)+1);i++)
			uart_trans(towrite[i]);
		}
	}
	
	//deafult broadcasting command
	char *temp ="\rZD0013C\n";
	for(i=0;i<9;i++)
		uart_trans(temp[i]);
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

//read function
char uart_read(void){
	while(!(UCSR1A & (1<<RXC1)));
	return UDR1;
}

