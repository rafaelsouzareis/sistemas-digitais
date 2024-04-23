
	.global atraso

atraso:

	push r30
	push r31

	/*Caso o Parametro seja zero, terminar a execução da rotina*/
	sbiw r24, 0
	breq fim

start: 
	ldi r30, 124
	ldi r31, 12
	loop:
	sbiw  z, 1
	breq inicio
	rjmp loop

inicio:
	sbiw r24, 1
	breq fim
	rjmp start

fim:
	/*Retirar da pilha e terminar a execução*/
	pop r31
	pop r30
	RET
	
