/*
 * Primitivas.c
 *
 *  Created on: 16/04/2014
 *      Author: utnso
 */

#include <stdio.h>



signed int* definirVariable(t_nombre_variable identificador_variable){
	identificador_variable = malloc(sizeof(signed int));
	if(identificador_variable!=NULL){
		return identificador_variable;
	}
	else{
		printf("No se puede reservar memoria");
		return NULL;
	}
}

signed int* obtenerPosicionVariable(t_nombre_variable identificador_variable){
	return 0;
}

signed int dereferenciar(signed int* direccion_variable){  //extrae el valor de la variable ubicada en direccion_variable
	signed int aux;
	aux=*direccion_variable;
	return aux;
}

void asignar(signed int* direccion_variable, signed int valor){ //copia el valor en la variable ubicada en direccion_variable
	direccion_variable=&valor;
}

signed int obtenerValorCompartida(t_nombre_compartida variable){ //deberia solicitar el valor de la variable al kernel
	return 0;
}

signed int asignarValorCompartida(t_nombre_compartida variable, signed int valor){ //deberia solicitar al kernel que asigne valor a la variable
	return 0;
}
