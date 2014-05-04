/*
 * cpu.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "stack.h"
#include <stdio.h>

int main (int argc, char **argv){
	//Esto es una prueba
	t_valor_variable a = 'A';
	t_valor_variable b = 2048;
	t_valor_variable c = -156;
	t_stack* pila=CREATE_STACK(1000);
	PUSH_SIZE_CHECK(&a,pila,0);
	PUSH_SIZE_CHECK(&b,pila,5);
	PUSH_SIZE_CHECK(&c,pila,10);
	t_valor_variable P,PO,POOP,POOOP;
	printf("top_index vale %d\n",pila->top_index);
	printf("el valor en esa pos es %d\n",pila->elementos[pila->top_index]);
	printf("el valor en pos 5 es %d\n", pila->elementos[5]);
	printf("el valor en pos 0 es %c\n", pila->elementos[0]);
	P=POP_SIZE_CHECK (pila);
	PO=POP_SIZE_CHECK (pila);
	POOP=POP_SIZE_CHECK (pila);
	printf("El valor es %d\n",P);
	printf("El valor (char) es %c\n",PO);
	printf("El valor %c (char) esta en la posicion %p\n",POOP,&POOP);
	POOOP=POP_SIZE_CHECK(pila);
	printf("El valor es %d\n",POOOP);
	DESTROY_STACK(pila);

	return 0;
}
