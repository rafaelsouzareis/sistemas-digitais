#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>


unsigned char *pt_ddrb = (unsigned char *) 0x24;
volatile int contador = 0, crescente = 1;

/*Interrupção associada ao overflow do counter2*/
ISR(TIMER2_OVF_vect){
	/*Acender o led*/
	if ((contador < 255)&&(crescente == 1)){
		OCR2A = OCR2A + 1;
		if(contador == 254){
			crescente = 0;
		}
		contador++;
	}
	/*Apagar o led*/
	else if ((contador > 0)&&(crescente == 0)){
		OCR2A = OCR2A - 1;
		if(contador == 1){
			crescente = 1;
		}
		contador--;
	}
		
}	
	

int main() {

	
	/*Configuração do Temporizador*/
	OCR2A = 0;  /*Comparar o valor de OCR2A com o Timer/Counter2, então OCR2A deve iniciar em 0x00*/
	TCCR2A = 0b10000011; /*Operar no modo fast pwm*/
	TCCR2B = 0b00000110; /*Frescale clk/256*/
	TIMSK2 = 0x01; /*Habilitar a interrupção qnd gerado overflow*/
	DDRB = 0xFF;
	sei(); /*Habilitar todas as interrupções*/

	while(1) {		

	/*Não fazer nada, então vou por as contas aqui =)
	
	Tempo de Relógio = 1/(16E6)*256 = 0,000016 segundos
	Tempo total counter2 = 1/(16E6)*256*256 = 0,004096 segundos
	Tempo total para percorrer UCR2A de 0 a 255 (de 255 a 0 tbm vale) = 1/(16E6)*256*256*256 = 1,04 segundos
	
	*/
	}
}
