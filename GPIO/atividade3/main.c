/*EA871 
Nome: Rafael Souza dos Reis
RA: 186168
Turma: T
Atividade 3 - Contador 
*/

/*Bibliotecas*/
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define F_CPU 16000000UL

/*Registradores utilizados - DDRC, DDRD, PINC e PORTD*/
unsigned char *pt_ddrc = (unsigned char *) 0x27;
unsigned char *pt_pinc = (unsigned char *) 0x26;
unsigned char *pt_portc = (unsigned char *) 0x28;
unsigned char *pt_ddrd = (unsigned char *) 0x2A;
unsigned char *pt_portd = (unsigned char *) 0x2B;
unsigned char *ucsr0b = 0xC1;

int main(void)
{
	/*Direção dos Registradores
	Habilitar todos os pinos de C como entrada
	Habilitar todos os pinos (exceto o primeiro) de C como saída*/
	*pt_ddrc = *pt_ddrc & 0x00;
	*pt_ddrd = *pt_ddrd | 0xFE;
	*ucsr0b &= ~(0x08);
	
	/*ativar o resistor pull-up na porta 1 c*/
	*pt_portc = *pt_portc | 0x01;
	
	/*Máscara para fazer comparação do conteúdo que está em PINC*/
	char mask_pinc = 0x01;
	
	/*Auxiliares
	Cuidado com as variáveis pressionado e pressionando - são diferentes!
	*/
	int pressionado = 0, contador = 0, pressionando;

	/*Vetor display 
	Acionar os pinos que fazem os números de 0-9 se formarem no display de 7 segmentos*/
	char display[10] = {0x7E, 0x0C, 0xB6, 0x9E, 0xCC, 0xDA, 0xFA, 0x0E, 0xFE, 0XCE};
	
	/*Estado inicial*/
	*pt_portd = display[0];
	
	
	
	while (1)
	{	/*Verificar se o botão foi pressionado*/
		if((*pt_pinc & mask_pinc) == 0){
			_delay_ms(500);
			if((*pt_pinc & mask_pinc) == 0){
				pressionado = 1;
			}
		}
		
		/*Se o botão foi pressionado trocar o estado atual do pino de saída*/
		if(pressionado == 1){
			
			contador = (contador + 1)%10;
			*pt_portd = display[contador];
			
		/*Não mudar o estado de pressionado caso o usuário estiver mantendo o botão pressionado*/	
		
			pressionando = 1;		
			while(pressionando == 1){
				if((*pt_pinc & mask_pinc) == 0x00){
					_delay_ms(500);
					if((*pt_pinc & mask_pinc) == 0x00){
						pressionando = 1;
					}
				}
				else
				pressionando = 0;	
			} 	
			pressionado = 0;		
		}
	}
}