/*
 * atividade5.c
 * Created: 15/09/2019 21:56:56
 * Author : Rafael Souza dos Reis
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

extern void atraso(unsigned int x); /*rotina de atraso, arquivo externo*/

/*Definindo em quais pinos serão as saída das cores*/
unsigned char *pt_ddrb = (unsigned char *) 0x24;
unsigned char *pt_portb = (unsigned char *) 0x25;

/*Variáveis que controlam o sentido do led RGB no vetor cores*/
int sentido = 0;
int posicao = 0;
int pressionado = 0;

/*Atraso inicial de 1 segundo*/
int atrasar = 1000;


/*Copmbinação das saídas que geram as cores solicitadas no LED RGB*/
char cores[8]= {0x00,0x04,0x02,0x01,0x06,0x03,0x05,0x07};



int main(void)
{
	
	/*Configurar pinos 0-2 de B como saída*/
	*pt_ddrb = *pt_ddrb | 0x07;
	
	//Configurar interrupção externa do INT0 por borda de subida
	EICRA |= (1 << ISC00);
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
	
	//Habilitar interrupção externa tipo PIN Change do PCINT8 associado ao PCIE1
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT8);
	

	//Habilitar todas as interrupções
	sei();
	
	while (1) 
    {
		/*Aqui chamamos a rotina de atraso*/
		atraso(atrasar);
		/*Aqui chamamos a rotina que muda o estado do LED RGB*/
		cor_led();
		
    }
}

/*rotina de serviços relacionada a mudança de cor do led RGB*/
int cor_led(){
	
	if(sentido == 0){
		posicao = (posicao+1)%8;		
	}
		
	else if(sentido == 1){
		posicao = posicao - 1;
		
		if(posicao == -1)
			posicao = 7;
	}
		
	*pt_portb = *pt_portb & 0x00;
	*pt_portb = *pt_portb | cores[posicao];
}


/*rotina de serviço relacionada à interrupção INT0, para diminuir pela metade o tempo de mudança de cor*/
ISR (INT0_vect)
{
	atrasar = atrasar / 2;
	
	if (atrasar < 100)
		atrasar = 1000;
			
}

/*rotina de serviço relacionada à interrupção PCINT8, para inverter o sentido de acionamento das cores do led RGB*/
ISR (PCINT1_vect)
{
	if (pressionado == 0)
		pressionado = 1;
	else
		pressionado = 0;	
	
	if (pressionado == 1){
		if(sentido == 0)
			sentido = 1;
		
		else if(sentido == 1)
			sentido = 0;
	}
}


