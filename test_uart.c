#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <errno.h>
#include <string.h>

#define TTY "/dev/ttyS0"

int uart0_fd;

void ctrl_c(int sig){
	puts("Fechando" TTY "...");
	serialClose(uart0_fd);
	exit(-1);
}

int main(void){
	unsigned char arduino_input = 1;
	int serial_status;
	signal(SIGINT, ctrl_c);
	uart0_fd = serialOpen(TTY, 9600);
	if(uart0_fd == -1){
		puts("Erro abrindo a UART. Garanta que ela nao esteja");
		puts("sendo usada por outra aplicacao.");
	}

	if(wiringPiSetup() == -1){
		puts("Erro em wiringPiSetup().");
		return -1;
	}
	puts(TTY " aberto.");
	puts("UART configurada.");
	system("stty -F" TTY);
	puts("");
	serialFlush(uart0_fd);
	while(1){

		arduino_input = serialGetchar(uart0_fd);
		serial_status = serialDataAvail(uart0_fd);
		printf("Arduino returnou: %d\n", arduino_input);
		if(serial_status == -1){
			printf("Erro = %s\n", arduino_input);
		}
		else{
			printf("%d bytes disponiveis\n", arduino_input);
		}
	}
	serialClose(uart0_fd);
	return 0;
}