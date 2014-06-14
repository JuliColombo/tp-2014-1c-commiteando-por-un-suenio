/*
 * primitivasAux.c
 *
 *  Created on: 22/05/2014
 *      Author: utnso
 */

#include "primitivasAux.h"

//Esto de aca es para probar. Despues se pasan por sockets y esas cosas.
t_dictionary *diccionario;
int sockfd;
int sockUMV;
int top_index;

char* generarIndiceEtiquetas(t_puntero index_etiquetas,t_size etiquetas_size){
	char* indice = malloc(etiquetas_size+1);
	//tendria que aclararle a la umv que es para generar el indice

	t_estructura tipo = STRUCT_STRING;
	void** estructura2;
	socket_recibir(sockUMV,&tipo, estructura2);
	struct_string** estructuraAux = (struct_string**)estructura2;

	indice = (*estructuraAux)->string;

	free(estructura2);
	free(estructuraAux);

	return indice;

}


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
	int* cursor = &pcb.c_stack;

	posicionContextoViejo = calcularPosicionAsignacionCPU(top_index);

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(cursor,pila,posicionContextoViejo);
	socket_and_push(sockUMV,posicionContextoViejo,*cursor);

	//Pushear Program Counter de proxima instruccion:
	int posicionPC;
	pcb.program_counter +=1;
	int pc = pcb.program_counter;
	posicionPC = calcularPosicionAsignacionCPU(top_index);

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(&pc,pila,posicionPC);
	socket_and_push(sockUMV,posicionPC,pc);
	//Borrar diccionario y todos los elementos. Cuando lo regenero, los vuelvo a crear.
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

void reservarContextoConRetorno(){
	t_puntero posicionAVariable;
	int posicionVar = top_index;

	reservarContextoSinRetorno();

	posicionAVariable = calcularPosicionAsignacionCPU(top_index);

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);
	socket_and_push(sockUMV,posicionAVariable,posicionVar);
}


/*Lo que hago con esta funcion es:
 * estoy en la posicion de cursor contexto.
 * Pongo en el diccionario el id y su posicion.
 * Disminuyo el top_index en 2 para llegar al nombre de una variable (si restara uno, obtendria el valor) la proxima vez que la use.
 * Invoco esta funcion tantas veces como el tamaño del contexto sea.
 */

void guardarAlternado () {
	//Socket a UMV para que haga: pila->top_index = top_index;
	struct_modificar_top_index* estructura = crear_struct_modificar_top_index(top_index);
	socket_enviar(sockUMV,STRUCT_MODIFICAR_TOP_INDEX,estructura);
	free(estructura);
	//Socket a UMV para que haga TOP(pila)
	socket_enviarSignal(sockUMV, TOP);

	//Socket de UMV para que me pase lo ultimo que hay pusheado en la pila, y yo hacer: t_nombre_variable identificador_variable = TOP(pila);
	t_estructura tipo = STRUCT_CHAR;
	void** estructura1;
	socket_recibir(sockUMV,&tipo, estructura1);
	struct_char** estructuraAux= (struct_char**)estructura1;
	t_valor_variable identificador_variable = (*estructuraAux)->letra;
	free(estructura1);
	free(estructuraAux);

	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,top_index);
	dictionary_put(diccionario,elem->name,elem);

	top_index -=2;

}

uint32_t calcularTamanioContextoAnterior(t_puntero direccion_contexto_actual) {
	uint32_t diferencia = direccion_contexto_actual - pcb.c_stack;
	return (diferencia/5); //Divido esa cantidad de bytes por 5 (1 byte de id de variable, y 4 bytes del valor) para saber cuantas variables habia.
}


//Una vez que regenere el diccionario, pongo el top_index en la posicion del valor de la ultima variable, cosa que si se quieren definir nuevas con la funcion
//calcularPosicion, no tire error.
void regenerarDiccionario(int tamanio_contexto) {
	int i = 0;
	int top = top_index - 1;
	while (i < tamanio_contexto) {
		guardarAlternado();
		i++;
	}

	//Socket a UMV para que haga: pila->top_index = top;
	struct_modificar_top_index* estructura = crear_struct_modificar_top_index(top);
		socket_enviar(sockUMV,STRUCT_MODIFICAR_TOP_INDEX,estructura);
		free(estructura);

}

t_puntero recuperarDireccionRetorno() {
	socket_and_pop_position(sockUMV,pcb.stack - 1); //resto uno porque era retornar

	//QUIERO LO DE ABAJO¡???
	t_puntero* retorno;
	t_estructura tipo = STRUCT_NUMERO;
	void** estructura2;
	socket_recibir(sockUMV,&tipo, estructura2);
	struct_numero** estructuraAux = (struct_numero**)estructura2;
	*retorno = (*estructuraAux)->numero;
	free(estructura2);
	free(estructuraAux);

	return *retorno;
}

void recuperarPosicionDeDirecciones() {
	struct_modificar_top_index* estructura = crear_struct_modificar_top_index(pcb.c_stack-1);
	socket_enviar(sockUMV,STRUCT_MODIFICAR_TOP_INDEX,estructura);
	free(estructura);
}

void recuperarProgramCounter(t_puntero* program_counter) {
	//Socket a UMV para que haga: t_puntero program_counter = POP(pila);
	t_estructura tipo = STRUCT_NUMERO;
	socket_enviarSignal(sockUMV, POP);

	void** estructura2;
	socket_recibir(sockUMV,&tipo, estructura2);
	struct_numero** estructuraAux = (struct_numero**)estructura2;
	*program_counter = (*estructuraAux)->numero;
	free(estructura2);
	free(estructuraAux);
}

void recuperarCursorAnterior(t_puntero* cursor_stack_viejo) {
	//Socket a UMV para que haga: t_puntero cursor_stack_viejo = POP(pila);
	t_estructura tipo = STRUCT_NUMERO;
	socket_enviarSignal(sockUMV, POP);

	void** estructura3;
	socket_recibir(sockUMV,&tipo, estructura3);
	struct_numero** estructuraAux1 = (struct_numero**)estructura3;
	*cursor_stack_viejo = (*estructuraAux1)->numero;
	free(estructura3);
	free(estructuraAux1);
}

//Popea dos veces en el stack para recuperar el valor del program_counter y el de c_stack al volver al contexto anterior
void volverAContextoAnterior() {
	t_puntero program_counter, cursor_stack_viejo;

	recuperarProgramCounter(&program_counter);
	recuperarCursorAnterior(&cursor_stack_viejo);

	//Socket de UMV para que yo actualice el top_index

	pcb.program_counter = program_counter;
	pcb.c_stack = cursor_stack_viejo;

	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

