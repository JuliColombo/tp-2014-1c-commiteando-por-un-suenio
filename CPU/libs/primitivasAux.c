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
int sockfd;
int top_index;

//t_pcb pcb:

int stack_base;
int c_stack;
int program_counter;
int tamanio_contexto;
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

t_puntero calcularPosicionAsignacionCPU(int top_index) {
	t_puntero posicion=0;
	if(top_index == -1) {
		posicion = top_index +1;
	} else {
		posicion = top_index + 2;
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
	int* cursor = &c_stack;

	posicionContextoViejo = calcularPosicionAsignacionCPU(top_index);

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(cursor,pila,posicionContextoViejo);

	//Pushear Program Counter de proxima instruccion:
	int posicionPC;
	program_counter +=1;
	int pc = program_counter;
	posicionPC = calcularPosicionAsignacionCPU(top_index);
	//Socket a UMV para que haga: PUSH_SIZE_CHECK(&pc,pila,posicionPC);

	//Borrar diccionario y todos los elementos. Cuando lo regenero, los vuelvo a crear.
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

void reservarContextoConRetorno(){
	t_puntero posicionAVariable;
	int posicionVar = top_index;

	reservarContextoSinRetorno();

	posicionAVariable = calcularPosicionAsignacionCPU(top_index);

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);

}


/*Lo que hago con esta funcion es:
 * estoy en la posicion de cursor contexto. Asi que disminuyo el top_index en 2 para llegar al nombre de una variable (si restara uno, obtendria el valor)
 * Pongo en el diccionario el id y su posicion. Lo hago tantas veces como el tamaño del contexto sea.
 */
void guardarAlternado (t_stack* pila) {
	top_index -=2;
	//Socket a UMV para que haga: pila->top_index = top_index;

	//Socket de UMV para que me pase lo ultimo que hay pusheado en la pila, y yo hacer: t_nombre_variable identificador_variable = TOP(pila);

	//const char* str=convertirAString(identificador_variable);
	//t_elemento* elem = elemento_create(str,top_index);
	//dictionary_put(diccionario,elem->name,elem);

}


//Una vez que regenere el diccionario, pongo el top_index en la posicion del valor de la ultima variable, cosa que si se quieren definir nuevas con la funcion
//calcularPosicion, no tire error.
void regenerarDiccionario(int tamanio_contexto) {
	int i = 0;
	int top = top_index - 1;
	while (i < tamanio_contexto) {
		guardarAlternado(pila);
		i++;
	}

	//Socket a UMV para que haga: pila->top_index = top;
}

void volverAContextoAnterior() {
	//en realidad usaria pcb->tamanio_contexto
	//Usaria pcb->c_stack. O sea, que vaya a donde comienza mi contexto, para empezar a popear.

	//Socket a UMV para que haga: t_puntero posicionVariable = POP_RETORNAR(pila, c_stack);
	//Socket a UMV para que haga: t_puntero program_counter = POP(pila);
	//Socket a UMV para que haga: t_puntero cursor_stack_viejo = POP(pila);

	//Socket de UMV para que yo actualice el top_index

	//pcb->program_counter = program_counter;
	//pcb->c_stack = c_stack;

	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);
}


