/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "parser/metadata_program.h"
#include "stack.h"

t_stack* pila;
//t_puntero posicion;
t_dictionary* diccionario = dictionary_create();

t_puntero calcularPosAsignacion(t_stack* P) {
	t_puntero posicion;
	if(IS_EMPTY(P)) {
		posicion = P->top_index +1;
	} else {
		char x = P->elementos[P->top_index];
		if(x != '\0') {
		posicion = P->top_index + 5;}
	}
	return posicion;}

//No entendi si devuelve la posicion de la variable en la pila
t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;
	t_puntero posicion = calcularPosAsignacion(pila);
	PUSH_SIZE_CHECK(&id,pila,posicion);
	dictionary_put(diccionario,&identificador_variable,0); //dudo del 0
	return posicion;
}

t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {
	return 0;
}

t_valor_variable dereferenciar(t_puntero direccion_variable) {
	return 0;
}

void asignar(t_puntero direccion_variable, t_valor_variable valor) {

}

t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {
	return 0;
}

t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {
	return 0;
}

t_puntero_instruccion irAlLabel(t_nombre_etiqueta etiqueta) {
	return 0;
}

t_puntero_instruccion llamarSinRetorno(t_nombre_etiqueta etiqueta) {
	return 0;
}

t_puntero_instruccion llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {
	return 0;
}

t_puntero_instruccion finalizar() {
	return 0;
}

int imprimir(t_valor_variable valor_mostrar) {
	return 0;
}

int imprimirTexto(char* texto) {
	return 0;
}

int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
	return 0;
}

int wait(t_nombre_semaforo identificador_semaforo) {
	return 0;
}

int signal(t_nombre_semaforo identificador_semaforo) {
	return 0;
}
