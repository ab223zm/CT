#include<avr/io.h>
#define F_CPU 1843200UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#define BAUD 2400
#define UBRR_VAL 47

void uart_int(void);
void uart_trans(unsigned char data);
void printout(char memory, char* inchar);
char* customconcat(char* inchar1, char* inchar2);
char uart_read();
void broadcast();

int main(void) {
	uart_int();
	char displThis [2][30];
	char message[30];
	int j=0;
	char mssg;
	char line='A';
	display(line, "Enter chars on PuTTY");
	broadcast();

	while (1)
	{
		mssg=uart_read();
		if(mssg=='1'|| mssg=='2'|| mssg=='3'){
			if (mssg=='1'){
				displThis[0][j]=mssg;
				message[j]=displThis[0][j];
				displThis[0][j++];
			}
			else if (mssg=='2'){
				displThis[1][j]=mssg;
				message[j]=displThis[0][j];
				displThis[1][j++];
			}
			else {
				displThis[2][j]=mssg;
				message[j]=displThis[2][j];
				displThis[2][j++];
				line='B';
			}
		display(line,message);
		}
		
	return 0;
}

void uart_int(void) {
	UBRR1L=UBRR_VAL;
	UCSR1B =(1<<TXEN1) | (1<<RXEN1);
}
void uart_trans(unsigned char data ) {
	while(!(UCSR1A & (1<<UDRE1))); // wait while register is free
	UDR1=data; // load data in the register
}
char uart_read(){
	while(!(UCSR1A & (1<<RXC1)));
	return UDR1;
}

void display(char line, char message) {
	int i;
	int checksum=0;
	char displmssg[30]="\rAO001";
	address[2]=line;
	sprintf(dispmssg, "%s%02X\n",message);
	for(i=0; i<sizeof(dispmssg);i++){
		checksum+=dispmssg[i];
	}
	checksum%=256;
	
	char towrite[100];
	sprintf(towrite, "%s%02X\n", dispmssg, checksum);
	for(i=0; i<sizeof(towrite); i++){
		uart_trans(towrite[i]);
	}
}

void broadcast(){
	int i;
	char* temp="\rZD0013C\n";
	for(i=0; i<9;i++){
		uart_trans(temp[i]);
	}
}
/*
#include<avr/io.h>
#include<stdio.h>
#include <stdlib.h>
#define F_CPU 1830000UL
#include <util/delay.h>

//#define BAUD 2400
//#define UBRR_VAL 47

void uart_int(void);
void uart_trans(unsigned char data);
char uart_read();

int main(void){
	uart_int();
	char towrite[100];
	int i;
	int checksum=0;
	
	
	//Faze 1: selection message
	char* temp ="\rAO0001Select line 0-2";
	for (i=0;i<(sizeof(temp)+1);i++){
		checksum+=temp[i];
	}
	checksum=checksum%256;
	sprintf(towrite,"%s%02X\n", temp, checksum);
	for(i=0;i<(sizeof(towrite)+3);i++){
		uart_trans(towrite[i]);
	}
	
	//deafult broadcasting command
	temp ="\rZD0013C\n";
	for(i=0;i<9;i++){
		uart_trans(temp[i]);
	}
	
	//Faze 2: character input
while (1) {
	checksum=0;
	char line = uart_read();
	if (line=='0'){
		temp ="\rAO0001";
		sprintf(towrite,"%s%02X\n", temp);
		//while(1){
			for (i=8;i<32;i++)
			towrite[i]=uart_read();
		//}
		//return 0;
	}
	else if (line=='1'){
		temp ="\rAO0001";
		sprintf(towrite,"%s%02X\n", temp);
		//while(1){
			for (i=8;i<32;i++)
			towrite[i]=" ";
			for (i=32;i<49;i++)
			towrite[i]=uart_read();
		//}
		//return 0;
	}
	else if (line==2){
		temp ="\rBO0001";
		sprintf(towrite,"%s%02X\n", temp);
		//while(1){
			for (i=8;i<32;i++)
			towrite[i]=uart_read();
		//}
		//return 0;
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
}
//initialization function
void uart_int(void){
	UBRR1L=47;
	UCSR1B =((1 << RXEN1) | (1 << TXEN1));
	
}
//transmission function
void uart_trans(unsigned char data){
	while(!(UCSR1A & (1<<UDRE1)));
	UDR1 = data;
}
// read function
char uart_read(){
	while(!(UCSR1A & (1<<RXC1)));
	return UDR1;
}
*/
