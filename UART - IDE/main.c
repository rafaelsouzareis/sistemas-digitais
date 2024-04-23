/*
 * atividade6.c
 *
 * Created: 27/09/2019 23:36:56
 * Author : Rafael Souza dos Rei
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define F_CPU 16000000UL


int main(void)
{
  
	UBRR0H = 0;
	UBRR0L = 25; /*BAUD configurado para enviar 38,4K bits/s*/
	UCSR0A = 0x00; /*Zerar todas as flags de estado e velocidade de transmiss�o normal*/
	UCSR0B = 0x08 /*0b00001000*/;/*Habilitar somente transmiss�o de dados; desabilitar todas as interrup��es*/
	UCSR0C = 0x36 /*0b00110110*/;/*Modo de opera��o Ass�ncrono; Habilitar bit de paridade �mpar; 1 bit de parada; transmiss�o de 8 bits por frame*/
  
	char msg[] = " Out of the night that covers me,\n Black as the Pit from pole to pole,\n I thank whatever gods may be\n For my unconquerable soul.\n In the fell clutch of circumstance\n I have not winced nor cried aloud.\n Under the bludgeonings of chance\n My head is bloody, but unbowed.\n Beyond this place of wrath and tears\n Looms but the Horror of the shade,\n And yet the menace of the years\n Finds, and shall find, me unafraid.\n It matters not how strait the gate,\n How charged with punishments the scroll.\n I am the master of my fate:\n I am the captain of my soul.";
  
	char msg1[] = "\n Mensagem enviada com sucesso! \n";
  
	int i = 0;
  
    while (1){  
	  i = 0;
      /*transmiss�o da mensagem*/
      while(msg[i]){
        /*enquanto TXCO e UDRE0 n�o estiverem setadas, n�o carregar novo valor em UDR0*/
        while(UCSR0A & 0x60 != 0x60){
        }
        UDR0 = msg[i];
        i++; 
        _delay_ms(1);
      }

      i = 0;
      /*Transmiss�o da mensagem de confirma��o de envio*/
      while(msg1[i]){
        /*enquanto TXCO e UDRE0 n�o estiverem setadas, n�o carregar novo valor em UDR0*/
        while(UCSR0A & 0x60 != 0x60){
        }
        UDR0 = msg1[i];
        i++; 
        _delay_ms(1);
      }
      _delay_ms(5000);
    }
}

