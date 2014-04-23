/*
 * funciones.h
 *
 *  Created on: 12/04/2014
 *      Author: utnso
 */

#ifndef FUNCIONES_H_
#define FUNCIONES_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef struct pares{
		int desplazamiento;
		int longitudLinea;
} Pares;

typedef struct {
	t_hash_element **elements;
	int elements_amount;
} diccionario;

typedef struct pcb {
		int ID;
	    int* CODE, STACK, CursorSTACK;
	    //estaba int* indiceEtiquetas, pero por lo que dice la consigna: el indice de etiquetas constara de la serializacion
	    //de un diccionario que ontiene el id de cada funcion o etiqueta...
	    diccionario* indiceEtiquetas;
	    int* ProgramCounter;
	    int TamanioContext;
	    Pares indiceCodigo[];
} PCB;




#endif /* FUNCIONES_H_ */
