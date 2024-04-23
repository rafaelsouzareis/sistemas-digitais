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
	/*Dire��o dos Registradores
	Habilitar todos os pinos de C como entrada
	Habilitar todos os pinos (exceto o primeiro) de C como sa�da*/
	*pt_ddrc = *pt_ddrc & 0x00;
	*pt_ddrd = *pt_ddrd | 0xFE;
	*ucsr0b &= ~(0x08);
	
	/*ativar o resistor pull-up na porta 1 c*/
	*pt_portc = *pt_portc | 0x01;
	
	/*M�scara para fazer compara��o do conte�do que est� em PINC*/
	char mask_pinc = 0x01;
	
	/*Auxiliares
	Cuidado com as vari�veis pressionado e pressionando - s�o diferentes!
	*/
	int pressionado = 0, contador = 0, pressionando;

	/*Vetor display 
	Acionar os pinos que fazem os n�meros de 0-9 se formarem no display de 7 segmentos*/
	char display[10] = {0x7E, 0x0C, 0xB6, 0x9E, 0xCC, 0xDA, 0xFA, 0x0E, 0xFE, 0XCE};
	
	/*Estado inicial*/
	*pt_portd = display[0];
	
	
	
	while (1)
	{	/*Verificar se o bot�o foi pressionado*/
		if((*pt_pinc & mask_pinc) == 0){
			_delay_ms(500);
			if((*pt_pinc & mask_pinc) == 0){
				pressionado = 1;
			}
		}
		
		/*Se o bot�o foi pressionado trocar o estado atual do pino de sa�da*/
		if(pressionado == 1){
			
			contador = (contador + 1)%10;
			*pt_portd = display[contador];
			
		/*N�o mudar o estado de pressionado caso o usu�rio estiver mantendo o bot�o pressionado*/	
		
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