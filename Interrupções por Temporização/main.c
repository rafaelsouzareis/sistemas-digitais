#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>


unsigned char *pt_ddrb = (unsigned char *) 0x24;
unsigned char *pt_portb = (unsigned char *) 0x25;
volatile int i, primeiro = 1;
volatile int led_externo = 0;
volatile int led_interno = 0;
volatile int interrupcao_float = 0;
volatile int contador_led_externo = 0;
volatile int contador_led_interno = 0;
volatile int contador_mensagem = 0;
char mensagem[] = "Atividade 8 – Interrupcoes temporizadas tratam concorrencia entre tarefas! \n";

/*Interrupção associada à comparação entre OCR2A e Timer/Counter2*/
ISR(TIMER2_COMPA_vect) {
	interrupcao_float = 1;
}

/*Função para fazer piscar o led externo*/
void pisca_led_externo() {
	if (led_externo == 0) {
		*pt_portb = *pt_portb | 0x10;
		led_externo = 1;
	}
	else {
		led_externo = 0;
		*pt_portb = *pt_portb & 0x20;
	}
}
/*Função para fazer piscar o led interno*/
void pisca_led_interno() {
	if (led_interno == 0) {
		*pt_portb = *pt_portb | 0x20;
		led_interno = 1;
	}
	else {
		*pt_portb = *pt_portb & 0x10;
		led_interno = 0;
	}
}

int main() {
	
	/*Configuração da UART*/
	UBRR0H = 0;
	UBRR0L = 25; /*BAUD configurado para enviar 38,4K bits/s*/
	UCSR0A = 0x00; /*Zerar todas as flags de estado e velocidade de transmissão normal*/
	UCSR0B = 0x08 /*0b00001000*/;/*Habilitar somente transmissão de dados; desabilitar todas as interrupções*/
	UCSR0C = 0x36 /*0b00110110*/;/*Modo de operação Assíncrono; Habilitar bit de paridade ímpar; 1 bit de parada; transmissão de 8 bits por frame*/
	
	/*Configuração do Temporizador*/
	OCR2A = 0xFF;  /*Comparar o valor de OCR2A com o Timer/Counter2*/
	TCCR2A = 0x02; /*Operar no modo CTC*/
	TCCR2B = 0x04; /*Frescale clk/64*/
	TIMSK2 = 0x02; /*Habilitar a interrupção na comparação entre OCR2A com o Timer/Counter2*/
	sei();

	/*Definir pinos 12 e 13 das I/O B como saída*/
	*pt_ddrb = 0x30;

	while(1) {
		
		/*Se gerou a interrupção, adicionar nos contadores*/
		if(interrupcao_float == 1){
			
			contador_led_interno++;
			contador_led_externo++;
			contador_mensagem++;
			
			/*Contador referente ao led interno - valor 488 referente à 0,5 segundos em clk/64*/
			if(contador_led_interno == 488){
				pisca_led_interno();
				contador_led_interno = 0;
			}
			/*Contador referente ao led externo - valor 762 referente à 0,79 segundos em clk/64*/
			if(contador_led_externo == 762){
				pisca_led_externo();
				contador_led_externo = 0;
			}
			/*Essa parte do código fica responsável por transmitir a mensagem serial para o visualizador de dados - valor 4883 referente à 5 segundos em clk/64*/			
			if(contador_mensagem >= 4883){
				
				/*Primeiro elemento do texto a ser entregue*/
				if(primeiro == 1){
					UDR0 = mensagem[i];
					primeiro = 0;
				}
				/*Se a transmissão de um caracter estiver completa, enviar o próximo caracter*/
				if((UCSR0A & 0x40) == 0x40){
					i++;
					UDR0 = mensagem[i];
					UCSR0A = 0x00;
				}
				
				/*Se enviou todos os caracteres (76), iniciar o contador para passar 5 segundos para a próxima transmissão*/
				if(i == 76){
					contador_mensagem = 0;
					i = 0;
					primeiro = 1;
				}
				
			}
			interrupcao_float = 0;
		}
		
	}
}