/* Contador de bits
 *
 * Este programa conta o numero de bits em um byte
 */

#include <stdio.h>

int main() {

	 unsigned char entrada;
	 unsigned int tmp;
 	 unsigned int n_bits;
	 unsigned int d;
	 /* Ler entrada em hexadecimal */
	 scanf("%x", &tmp);
	 entrada = (unsigned char)tmp;

	 n_bits = 0;
         
	 /*Utilizar mascara hexadecimal, alocar os bits da entrada para a esquerda e comparar  com a mascara*/
	 for(int loop = 1; loop <= 8; loop++){
	 
	 	if (loop == 1)
			 d = entrada & 0x80;
		

		else {
			entrada = entrada << 1;
		      	d = entrada & 0x80;			
		}

	 	if (d == 0x80)

			n_bits++;
	 }

	 /* Saída dos dados*/
	 printf("%d\n", n_bits);
	 return 0;
}
