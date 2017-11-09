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
	char displThis [4][30];
	//inchar, memory;
	char tmp[100];
	int line=0;
	//int lengths[4]={0, 0, 0, 0};

	memory='A';
	printout('A', "Enter chars on PuTTY>");
	printout('B', " ");
	broadcast();

	while (1)
	{
		inchar=uart_read();
		inchars[currentline][lengths[currentline]]=inchar;
		lengths[currentline]++;
		// Line change sequence .
		if(inchar=='/'|| inchar=='>'){
			if (inchar=='>') {
				inchar= uart_read();
				currentline=(int)(inchar-49)%3;
			}
			else
			currentline=(currentline+1)%3;

			if(lengths[currentline]!=0){
				memset(inchars[currentline], 0, strlen(inchars[currentline])) ;
				lengths[currentline]=0;
			}
		}

		if(currentline!=2){
			memory='A';
			strcpy(tmp, customconcat(inchars[0], inchars[1]));
			printout(memory,tmp);
			memset(tmp, 0, strlen(tmp));
		}
		else {
			memory='B';
			// strcpy ( tmp , inchars [currentline]) ;
			printout(memory, inchars[currentline]) ;
		}
		display();
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

void display(char memory, char* inchar) {
	int i, checksum;
	char temp[100]="\rAO0001";
	temp[2]=memory;
	strcat(temp,inchar);
	checksum=0;
	for(i=0; i<sizeof(temp);i++){
		checksum+=temp[i];
	}
	checksum%=256;
	char towrite[100];
	sprintf(towrite, "%s%02X\n", temp, checksum);
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
char* customconcat(char* inchar1, char* inchar2) {
	char toreturn[100];
	strcpy(toreturn, inchar1);
	if(strlen(toreturn)<24){
		int i;
		for(i=strlen(toreturn); i<24; i++){
			strcat(toreturn," ");
		}
	}
	strcat(toreturn, inchar2);
	return toreturn;
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
