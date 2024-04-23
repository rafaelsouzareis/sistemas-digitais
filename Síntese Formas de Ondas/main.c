#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <math.h>

volatile int inicio = 0, i, j, overflow = 0, transmitido = 0, mensagem_enviada = 0, contadormax = - 1, contador, led13 = 0, onda;
char c;

/*Mensagens*/
char msg1[] = "Onda senoidal\n";
char msg2[] = "Onda triangular\n";
char msg3[] = "Onda dente-de-serra\n";
char msg4[] = "Onda AM\n";
char msg5[] = "Nenhuma forma de onda selecionada\n";


/*Vetores forma de onda*/
const unsigned int senoidal[200] = { 0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 13, 15, 17, 19, 22, 24, 26, 29, 31, 34, 37, 40, 43, 46, 49, 52, 55, 59, 62, 66, 69, 73, 76, 80, 84, 88, 91, 95, 99, 103, 107, 111, 115, 119, 123, 127, 131, 135, 139, 143, 147, 151, 155, 159, 163, 166, 170, 174, 178, 181, 185, 188, 192, 195, 199, 202, 205, 208, 211, 214, 217, 220, 223, 225, 228, 230, 232, 235, 237, 239, 241, 242, 244, 246, 247, 248, 249, 250, 251, 252, 253, 253, 254, 254, 254 ,255, 254, 254, 254, 253, 253, 252, 251, 250, 249, 248, 247, 246, 244, 242, 241, 239, 237, 235, 232, 230, 228, 225, 223, 220, 217, 214, 211, 208, 205, 202, 199, 195, 192, 188, 185, 181, 178, 174, 170, 166, 163, 159, 155, 151, 147, 143, 139, 135, 131, 127, 123, 119, 115, 111, 107, 103, 99, 95, 91, 88, 84, 80, 76, 73, 69, 66, 62, 59, 55, 52, 49, 46, 43, 40, 37, 34, 31, 29, 26, 24, 22, 19, 17, 15, 13, 12, 10, 8, 7, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0};
const unsigned int dente_serra[200] = {0, 1, 3, 4, 5, 6, 8, 9, 10, 11, 13, 14, 15, 17, 18, 19, 20, 22, 23, 24, 26, 27, 28, 29, 31, 32, 33, 34, 36, 37, 38, 40, 41, 42, 43, 45, 46, 47, 48, 50, 51, 52, 54, 55, 56, 57, 59, 60, 61, 62, 64, 65, 66, 68, 69, 70, 71, 73, 74, 75, 77, 78, 79, 80, 82, 83, 84, 85, 87, 88, 89, 91, 92, 93, 94, 96, 97, 98, 99, 101, 102, 103, 105, 106, 107, 108, 110, 111, 112, 113, 115, 116, 117, 119, 120, 121, 122, 124, 125, 126, 128, 129, 130, 131, 133, 134, 135, 136, 138, 139, 140, 142, 143, 144, 145, 147, 148, 149, 150, 152, 153, 154, 156, 157, 158, 159, 161, 162, 163, 164, 166, 167, 168, 170, 171, 172, 173, 175, 176, 177, 179, 180, 181, 182, 184, 185, 186, 187, 189, 190, 191, 193, 194, 195, 196, 198, 199, 200, 201, 203, 204, 205, 207, 208, 209, 210, 212, 213, 214, 215, 217, 218, 219, 221, 222, 223, 224, 226, 227, 228, 230, 231, 232, 233, 235, 236, 237, 238, 240, 241, 242, 244, 245, 246, 247, 249, 250, 251, 252, 254};
const unsigned int triangular[200] = {0, 2, 5, 7, 10, 12, 15, 17, 20, 22, 25, 28, 30, 33, 35, 38, 40, 43, 45, 48, 51, 53, 56, 58, 61, 63, 66, 68, 71, 73, 76, 79, 81, 84, 86, 89, 91, 94, 96, 99, 102, 104, 107, 109, 112, 114, 117,119, 122, 124, 127, 130, 132, 135, 137, 140, 142, 145, 147, 150, 153, 155, 158, 160, 163, 165, 168, 170, 173, 175, 178, 181, 183, 186, 188, 191, 193, 196, 198, 201, 204, 206, 209, 211, 214, 216, 219, 221, 224, 226, 229, 232, 234, 237, 239, 242, 244, 247, 249, 252, 252, 249, 247, 244, 242, 239, 237, 234, 232, 229, 226,224, 221, 219, 216, 214, 211, 209, 206, 204, 201, 198, 196, 193, 191, 188, 186, 183, 181, 178, 175, 173, 170, 168, 165, 163, 160, 158, 155, 153, 150, 147, 145, 142, 140, 137, 135, 132, 130, 127, 124, 122, 119, 117, 114, 112, 109, 107, 104, 102, 99, 96, 94, 91, 89, 86, 84, 81, 79, 76, 73, 71, 68, 66, 63, 61, 58, 56, 53, 51, 48, 45, 43, 40, 38, 35, 33, 30, 28, 25, 22, 20, 17, 15, 12, 10, 7, 5, 2, 0}; 
unsigned char am[200] = {128, 141, 155, 168, 181, 192, 203, 211, 218, 223, 226, 226, 223, 218, 211, 201, 189, 176, 161, 145, 128, 110, 93, 76, 60, 46, 34, 23, 16, 10, 8, 9, 13, 19, 29, 41, 55, 71, 89, 108, 127, 147, 166, 185, 202, 217, 230, 241, 249, 253, 255, 253, 249, 241, 230, 217, 202, 185, 166, 147, 128, 108, 89, 71, 55, 41, 29, 19, 13, 9, 8, 10, 16, 23, 34, 46, 60, 76, 93, 110, 127, 145, 161, 176, 189, 201, 211, 218, 223, 226, 226, 223, 218, 211, 203, 192, 181, 168, 155, 141, 128, 114, 102, 91, 81, 72, 65, 60, 57, 55, 56, 58, 62, 67, 73, 81, 90, 99, 108, 118, 127, 137, 145, 153, 160, 166, 171, 175, 177, 178, 178, 177, 174, 171, 166, 161, 155, 148, 142, 135, 128, 121, 114, 108, 102, 97, 93, 89, 87, 86, 85, 86, 87, 89, 93, 97, 102, 108, 114, 121, 127, 135, 142, 148, 155, 161, 166, 171, 174, 177, 178, 178, 177, 175, 171, 166, 160, 153, 145, 137, 128, 118, 108, 99, 90, 81, 73, 67, 62, 58, 56, 55, 57, 60, 65, 72, 81, 91, 102, 114};
unsigned char zeros[200] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


ISR(TIMER2_OVF_vect){
	overflow = 1;
	
	/*Incremetar contador maximo para controle do led do pino 13*/
	if(contadormax != -1){
		contador++;
	}
	
}

/*Interrupção RX*/
ISR(USART_RX_vect){
	c = UDR0;
}
/*Interrupção TX*/
ISR(USART_TX_vect){
	transmitido = 1;
	mensagem_enviada = 0;
}

/*Transmissão serial*/
void transmitir_msg(char msg[]){
	i = 0;
	UDR0 = msg[i];
	while(msg[i]){
		if(transmitido == 1){
			i++;
			UDR0 = msg[i];
			transmitido = 0;
		}
	}
	c = 0;
	mensagem_enviada = 1;
}

/*Configuração da UART*/
void configurar_uart(){
	UBRR0H = 0;
	UBRR0L = 103;  /*BAUD: 9,6k bits/s*/
	UCSR0A = 0x00; /*Velocidade de transmissão: Normal*/
	UCSR0B = 0xD8; /*Habilitar: RX e TX*/
	UCSR0C = 0x0E; /*Modo de operação: Assíncrono - Bit de paridade: Nenhum - Bit de parada: 2 - Bit por frame: 8 bits*/
}
/*Configuração do temporizador*/
void configurar_temporizador(){
	OCR2A = 0;
	TCCR2A = 0b10000011;   /*Operar no modo fast pwm*/
	TCCR2B = 0b00000001;   /*prescale clk/1*/
	TIMSK2 = 0x01;         /*Habilitar a interrupção qnd gerado overflow*/
}
int main(void){

	DDRB = 0xFF;
	PORTB = 0x00;	
	configurar_temporizador();	
	configurar_uart();
	sei();                            /*Habilitar todas as interrupções*/
	
	while (1){
		
		if(overflow == 1){
			
			/*Selecionar formas de onda*/
			if(onda == 1){
				OCR2A = senoidal[j];
			}
			
			else if(onda == 2){
				OCR2A = triangular[j];
			}
			
			else if(onda == 3){
				OCR2A = dente_serra[j];
			}
			
			else if(onda == 4){
				OCR2A = am[j];
			}
			
			else if(onda == 5){
				OCR2A = zeros[j];
				
			}

			j++;
			
			if(j == 201){
				j = 0;
			}
			overflow = 0;
		}	
		
		/*Acender e apagar o LED 5 de PORTB*/
		if(contador == contadormax){
			if(led13 == 0){
				PORTB = 0xFF;
				led13 = 1;
			}
			else{
				PORTB = 0x00;
				led13 = 0;
			}
			contador = 0;
		}

		/*Onda Senoidal*/
		if((c == 's')&&(mensagem_enviada == 0)){
			transmitir_msg(msg1);
			contadormax = 62500; /*gerar 1 segundo de atraso*/
			contador = 0;
			j = 0;
			onda = 1; 
		}
		/*Onda Triangular*/
		else if((c == 't')&&(mensagem_enviada == 0)){
			transmitir_msg(msg2);
			contadormax = 31250; /*gerar 0,5 segundo de atraso*/
			contador = 0;
			j = 0;
			onda = 2;
		}
		/*Onda Dente de Serra*/
		else if((c == 'd')&&(mensagem_enviada == 0)){
			transmitir_msg(msg3);
			contadormax = 15625; /*gerar 0,25 segundo de atraso*/
			contador = 0;
			j = 0;
			onda = 3;
		}
		/*Onda AM*/
		else if((c == 'a')&&(mensagem_enviada == 0)){
			transmitir_msg(msg4);
			contadormax = 7812; /*gerar 0,125 segundo de atraso*/
			contador = 0;
			j = 0;
			onda = 4;
		}
		/*Comando incorreto*/
		else{
			if((mensagem_enviada == 0) &&(c != 0)){
				transmitir_msg(msg5);
				contadormax = -1; /*Não gerar nenhum atraso para o led do pino 13*/
				onda = 5;
			}
		}
		
	}
}
