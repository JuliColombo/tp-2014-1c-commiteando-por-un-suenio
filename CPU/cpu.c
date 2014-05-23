/*
 * cpu.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */
#include <commons/collections/dictionary.h>
#include "parser/metadata_program.h"
#include "stack.h"
#include <stdio.h>
#include "primitivas.h"

int main (int argc, char **argv){
	//Esto es una prueba
	/*t_valor_variable a = 'A';
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
	DESTROY_STACK(pila);*/


	char* programa = malloc(511);
	strcpy(programa,"#!/usr/bin/ansisop \n begin \n # primero declaro las variables \n variables a, b \n a = 20 \n print a \n end \n");
	t_medatada_program* metadata;
	metadata = metadatada_desde_literal(programa);
	printf("cantidad de instrucciones: %d\n", metadata->instrucciones_size);
	printf("Cantidad de etiquetas %i\n",metadata->cantidad_de_etiquetas);
	printf("----------- Funcion impresion -------- \n");
	printf("Cantidad de etiquetas %i\n",metadata->cantidad_de_etiquetas);
	printf("Cantidad de funciones %i\n",metadata->cantidad_de_funciones);
	printf("Etiquetas %s\n",metadata->etiquetas);
	printf("Tamaño del mapa serializado de etiquetas %i\n",metadata->etiquetas_size);
	printf("Tamaño del mapa serializado de instrucciones %i\n",metadata->instrucciones_size);
	printf("El numero de la primera instruccion es %i\n",metadata->instruccion_inicio);

	t_dictionary* dic = dictionary_create();




	return 0;
}
