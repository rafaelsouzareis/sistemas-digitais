/*
 atividade4.asm
 Created: 06/09/2019 20:02:47
 Author : Rafael Souza dos Reis
*/
	/*Configurar pino 5 de B como saída*/ 
	sbi DDRB, 5

	/*Rotina de Pisca*/

pisca:

	/*Fazer o LED acender*/
	sbr r23, 32
	out PORTB, r23
	call atraso

	/*Fazer o LED apagar*/
	clr r23
	out PORTB, r23
	call atraso 

	rjmp pisca


/*Rotina de Atraso*/

Atraso:
	/*Armazenar na pilha os registradores que serão utilizados para a rotina de atraso*/
	push r24
	push r25
	push r30
	push r31

	/*Passagem do Parametro*/
	ldi r24, 244
	ldi r25, 1

	/*Caso o Parametro seja zero, terminar a execução da rotina*/
	sbiw r25:r24, 0
	breq fim

start: 
	ldi r30, 124
	ldi r31, 12
	loop:
	sbiw  z, 1
	breq inicio
	rjmp loop

inicio:
	sbiw r25:r24, 1
	breq fim
	rjmp start

fim:
	/*Retirar da pilha e terminar a execução*/
	pop r31
	pop r30
	pop r25
	pop r24
	ret
	
