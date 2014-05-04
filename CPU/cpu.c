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
	int a = 'A';
	int b = 2048;
	int c = -156;
	t_stack* pila=CREATE_STACK(1000);
	PUSH(&a,pila);
	PUSH(&b,pila);
	PUSH(&c,pila);
	int P,PO,POOP;
	P=POP (pila);
	POOP=POP (pila);
	printf("El valor %d esta en la posicion %p\n",POOP,&POOP);
	printf("El valor es %d\n",P);
	PO=POP (pila);
	printf("El valor es %c\n",PO);
	return 0;
}
