/*
 * primitivasAux.c
 *
 *  Created on: 22/05/2014
 *      Author: utnso
 */

#include "primitivasAux.h"

//Esto de aca es para probar. Despues se pasan por sockets y esas cosas.
t_stack* pila;
t_dictionary *diccionario;
char* etiquetas;
t_size etiquetas_size;



int comprendidoEntre(int m, int n, int i) {
	int menor = (i>=m);
	int mayor = (i<=n);
	return (menor && mayor);
}


t_valor_variable nombreParametro(int i) {
	t_valor_variable nombre=0;
	if(comprendidoEntre(0,9,i)) {
		nombre = i;
	} else {
		nombre = -1;
	}
	return nombre;
}


t_puntero calcularPosicionAsignacion(t_stack* P) {
	t_puntero posicion=0;
	if(IS_EMPTY(P)) {
		posicion = P->top_index +1;
	} else {
		posicion = P->top_index + 2;
	}
	return posicion;}

char* convertirAString(t_nombre_variable c) {
	static char str[2];
	str[0]= c;
	str[1]= '\0';
	return str;
}

t_elemento* elemento_create(const char* name, t_puntero pos){
	t_elemento *new = malloc(sizeof(t_elemento));
	new->name = strdup(name);
	new->pos = pos;
	return new;
}

void reservarContextoSinRetorno() {
	t_puntero posicionContextoViejo;
	int* cursor = pila->cursor_stack;

	posicionContextoViejo = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(cursor,pila,posicionContextoViejo);

	//Pushear Program Counter de proxima instruccion
}

void reservarContextoConRetorno(){
	t_puntero posicionAVariable;
	int posicionVar = pila->top_index;

	reservarContextoSinRetorno();

	posicionAVariable = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);

}
