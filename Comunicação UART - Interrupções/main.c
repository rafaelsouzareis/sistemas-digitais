#include <avr/io.h>
#include <stdio.h> 
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>


/*Definindo em quais pinos serão as saída das cores*/
unsigned char *pt_ddrb = (unsigned char *) 0x24;
unsigned char *pt_portb = (unsigned char *) 0x25;


char cores[8]= {0x00,0x04,0x02,0x01,0x06,0x03,0x05,0x07};/*Copmbinação das saídas que geram as cores solicitadas no LED RGB*/
char buffer[11]; /*Buffer que armazena os valores recebidos em RX*/
volatile int escrever  = 0, apagar = 0, cheio = 0, inicio = 0, i, vazio, transmitido = 0; /*Controladores do buffer*/
volatile char c; /*Variavel auxiliar para conseguirmos jogar o conteudo de UDR0 no buffer*/

/*Mensagens*/
char mensagem[]; 
char msg0[] = "Vazio!\n";
char msg1[] = "Comando: Acender LED - cor vermelha\n";		
char msg2[] = "Comando: Acender LED - cor verde\n";
char msg3[] = "Comando: Acender LED - cor azul\n";
char msg4[] = "Comando: Acender LED - cor amarela\n";			
char msg5[] = "Comando: Acender LED - cor ciano\n";
char msg6[] = "Comando: Acender LED - cor magenta\n";
char msg7[] = "Comando: Acender LED - cor branca\n";
char msg8[] = "Comando incorreto\n";
char msg9[] = "Vazio!\n";

/*Interrupção RX para colocar UDR0 no buffer*/			
ISR(USART_RX_vect){
		
	c = UDR0;
	
	if((escrever + 1)%11 != apagar){
		buffer[escrever] = c;
		escrever++;
		if(escrever == 11 && apagar != 0){
			escrever = 0;
		}
	}
}

/*Interrupção TX para informar para a função de transmissão se os caracteres da msg foram enviados*/
ISR(USART_TX_vect){
	transmitido = 1;
}

/*Transmissão da Mensagem*/
void transmitir_msg(char msg[]){
	i = 0;
	UDR0 = msg[i];
	/*Só terminar a transmissão da mensagem quando enviar todos os caracteres*/
	while(msg[i]){
		/*Só passar para o prossimo caractere da mensagem quando a varialvel transmitido foi igual a 1, que acontece qnd acontece uma interrupção em Tx*/
		if(transmitido == 1){
			i++;
			UDR0 = msg[i];
			transmitido = 0;
		}
	}
}

/*Configuração da UART*/
void configurar_uart(){

	UBRR0H = 0;
	UBRR0L = 103; /*BAUD configurado para enviar 9,6k bits/s*/
	UCSR0A = 0x00; /*Zerar todas as flags de estado e velocidade de transmissão normal*/
	UCSR0B = 0xD8 /*0b11011000*/;/*Habilitar transmissão e recepção de dados*/
	UCSR0C = 0x0E /*0b00001110*/;/*Modo de operação Assíncrono; sem bit de paridade; 2 bit de parada; transmissão de 8 bits por frame*/
}

/*Remover Elementos do Buffer*/
void retirar_buffer(){
		if(apagar != escrever){
			apagar++;
			if(apagar == 11 && escrever != 11){
				apagar = 0;
			}
		}
}


int main(void){
	
	/*configurar saída de 0-2 do portb como saída*/
	*pt_ddrb = 0x07;
	
	configurar_uart();
	/*habilitar todas as interrupções*/
	sei();
	
	while (1){
			
			/*retirar elementos do buffer*/
			retirar_buffer();
			
			
			_delay_ms(200);
			
			/*Buffer Vazio*/
			if(escrever == apagar){
				transmitir_msg(msg9);
				*pt_portb = cores[0];
			}
			/*Vermelho*/
			else if(buffer[apagar] == 'r'){
				*pt_portb = cores[1];
				transmitir_msg(msg1);
			}
			/*Verde*/
			else if(buffer[apagar] == 'g'){
				*pt_portb = cores[2];
				transmitir_msg(msg2);
			}
			/*Azul*/
			else if(buffer[apagar] == 'b'){
				*pt_portb = cores[3];
				transmitir_msg(msg3);
			}
			/*Amarelo*/
			else if(buffer[apagar] == 'y'){
				*pt_portb = cores[4];
				transmitir_msg(msg4);
			}
			/*Ciano*/
			else if(buffer[apagar] == 'c'){
				*pt_portb = cores[5];
				transmitir_msg(msg5);
			}
			/*Magenta*/
			else if(buffer[apagar] == 'm'){
				*pt_portb = cores[6];
				transmitir_msg(msg6);
			}
			/*Branco*/
			else if(buffer[apagar] == 'w'){
				*pt_portb = cores[7];
				transmitir_msg(msg7);
			}
			/*Comando incorreto*/
			else{
				*pt_portb = cores[0];
				transmitir_msg(msg8);	
			}	
	}
}


