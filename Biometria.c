#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define TTY "/dev/ttyS0"

int uart0_fd;

void ctrl_c(int sig){
	puts("Fechando " TTY "...");
	serialClose(uart0_fd);
	exit(-1);
}

int realizar_Chamada(int id_recebido){
	
	int fd1, fd2, id, nao_encontrado = 0;
	char c_id[2], c_matricula[9], c, actual_time[50];

	fd1 = open("TurmaI.txt, O_RDONLY");
	
	if(fd1 < 0){
		printf("Erro ao abrir arquivo da turma.\n");
		serialClose(uart0_fd);
		return -1;
	}
	
	while(1){

		if(read(fd1,c_id,2) == 0){
			nao_encontrado++;
			break;
		}
		else if(atoi(c_id) == id_recebido){
			read(fd1,&c,1);
			read(fd1,c_matricula,9);
			break;
		}
		else{
			read(fd1,&c,1);
			read(fd1,c_matricula,9);
			read(fd1,&c,1);
		}

	}

	if(nao_encontrado == 1){
		printf("Id nao identificado.\n");
		close(fd1);
		return -1;
	}

	close(fd1);

	printf("A matricula encontrada para o id %d e: %s\n", id_recebido, c_matricula);

	time_t curtime;
	struct tm *loc_time;

	curtime = time(NULL);
	loc_time = localtime(&curtime);

	sprintf(actual_time,"%s",asctime(loc_time));
	printf("Horario do registro: %s",actual_time);
	
	printf("Computando chamada para %s...\n",c_matricula);
	fd2 = open("Chamada.txt", O_WRONLY | O_APPEND);

	write(fd2, c_matricula, 9);
	c = ' ';
	write(fd2, &c, 1);
	write(fd2, actual_time, strlen(actual_time));
	c = '\n';
	write(fd2, &c, 1);
	close(fd2);

	printf("Presenca registrada.\n");
	return 0;

}

int main (int argc, char *argv[]){

	unsigned char c_id;
	int id_recebido;

	uart0_fd = serialOpen(TTY, 9600);
	if(uart0_fd == -1){
		printf("Erro abrindo a UART. ");
		printf("Garanta que ela nao esteja sendo usada por outra aplicacao.\n");
		exit(1);
	}	
	if(wiringPiSetup() == -1){
		puts("Erro em wiringPiSetup().");
		serialClose(uart0_fd);
		exit(1);
	}
	
		puts(TTY "aberto.");
		puts("UART configurada");
		system("stty -F " TTY);
		puts("");
		serialFlush(uart0_fd);

		while(1){
			c_id = serialGetchar(uart0_fd);
			id_recebido = c_id - '0';	
			if(realizar_Chamada(id_recebido) < 0){
				printf("Erro ao tentar computar presenca\n");
				return -1;
				}
		}

	return 0;
}