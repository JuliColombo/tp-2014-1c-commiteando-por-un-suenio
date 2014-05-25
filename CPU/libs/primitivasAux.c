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

void elemento_delete(t_elemento* elemento) {
	free(elemento->name);
	free(elemento);
}

void reservarContextoSinRetorno() {
	t_puntero posicionContextoViejo;
	int* cursor = pila->cursor_stack;

	posicionContextoViejo = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(cursor,pila,posicionContextoViejo);

	//Pushear Program Counter de proxima instruccion:
	//int posicionPC;
	//pcb->program_counter +=1;
	//int pc = pcb->program_counter;
	//posicionPC = calcularPosicionAsignacion(pila);
	//PUSH_SIZE_CHECK(&pc,pila,posicionPC);

	//Borrar diccionario y todos los elementos. Cuando lo regenero, los vuelvo a crear.
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);
}

void reservarContextoConRetorno(){
	t_puntero posicionAVariable;
	int posicionVar = pila->top_index;

	reservarContextoSinRetorno();

	posicionAVariable = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);

}
/*Lo que hago con esta funcion es:
 * estoy en la posicion de cursor contexto. Asi que disminuyo el top_index en 2 para llegar al nombre de una variable (si restara uno, obtendria el valor)
 * Pongo en el diccionario el id y su posicion. Lo hago tantas veces como el tamaño del contexto sea.
 */
void guardarAlternado (t_stack* pila) {
	pila->top_index -= 2;
	t_nombre_variable identificador_variable = TOP(pila);
	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,pila->top_index);
	dictionary_put(diccionario,elem->name,elem);

}
//Una vez que regenere el diccionario, pongo el top_index en la posicion del valor de la ultima variable, cosa que si se quieren definir nuevas con la funcion
//calcularPosicion, no tire error.
void regenerarDiccionario(t_stack* pila, int tamanio_contexto) {
	int i = 0;
	int top_index = pila->top_index - 1;
	while (i < tamanio_contexto) {
		guardarAlternado(pila);
		i++;
	}
	pila->top_index = top_index;
}

