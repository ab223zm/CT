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
	temp="\rBO0001Assignment#6";
	for (i=0;i<20;i++){
		checksum+=temp[i];
	}
	checksum=checksum%256;
	sprintf(towrite,"%s%02X\n", temp, checksum);
	for(i=0;i<23;i++){
		uart_trans(towrite[i]);
	}
	
	temp ="\rZD0013C\n";
	for(i=0;i<9;i++){
		uart_trans(temp[i]);
	}
		
	_delay_ms(5000);

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
	
	//*********
	temp ="\rZD0013C\n";
	for(i=0;i<9;i++){
		uart_trans(temp[i]);
		}
	return 0;
}

void uart_int(void){
	UBRR1L=25;
	UCSR1B =(1<<TXEN1) | (1<<RXEN1);
	
}
void uart_trans(unsigned char data){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = data;
}
