/*;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 1DT301, Computer Technology I
; Date: 2017-10-28
; Author:
; Student name 1: Ruth Dirnfeld
; Student name 2: Alexandra Bjäremo
;
; Lab number: 6
; Title: CyberTech Wall Display
;
; Hardware: STK600, CPU ATmega2560
;
; Function: Retrieving and displaying characters from Putty
; on the selected line of the CyberTech Wall Display. 
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
; Changes in program: 2017-10-31, 2017-11-01
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
	int i,j;
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
	char line = uart_read();
	if (line=='0'){
		temp ="\rAO0001";
		sprintf(towrite,"%s%02X\n", temp);
		while(1){
			for (i=8;i<32;i++)
				towrite[i]=uart_read();
		}
	}
	else if (line=='1'){
		temp ="\rAO0001";
		sprintf(towrite,"%s%02X\n", temp);
		while(1){
			for (i=8;i<32;i++)
				towrite[i]=" ";
			for (i=32;i<49;i++)
				towrite[i]=uart_read();
		}
	}
	else if (line==2){
		temp ="\rBO0001";
		sprintf(towrite,"%s%02X\n", temp);
		while(1){
			for (i=8;i<32;i++)
				towrite[i]=uart_read();
		}
	}		
			
	for (i=0; i<(sizeof(towrite)+1);i++)
		checksum+=towrite[i];
	checksum = checksum%256;
	sprintf(towrite,"%s%02X\n", checksum);
			
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
// read function	    
char uart_read(void){
	while(!(UCSR1A & (1<<RXC1)));
	return UDR1;	
}
