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

char* proximaInstruccion;
char* indiceEtiquetas;
int esConRetorno;


void generarIndiceEtiquetas(t_puntero index_etiquetas,t_size etiquetas_size){

	socket_and_indice_etiquetas(sockUMV,index_etiquetas,etiquetas_size);

	t_struct_string* estructura = socket_recibir_estructura(sockUMV);
	proximaInstruccion = estructura->string;
	//SE DEBERIA HACER UN MALLOC EN PROXIMAINSTRUCCION Y DESPUES ACA HACER UN MEMCPY?
}


void insertarEnDiccionario(t_nombre_variable identificador_variable,t_puntero posicion) {
	const char* str = convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str, posicion);
	dictionary_put(diccionario, elem->name, elem);
}

t_puntero_instruccion irAIntruccionLabel(t_nombre_etiqueta etiqueta) {
	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta, indiceEtiquetas,pcb.tamanio_indice);
	pcb.program_counter = instruccion;
	return instruccion;
}

t_intructions instruccionParaBuscarEnIndiceCodigo(t_puntero_instruccion instruccion) {
	socket_and_number(sockUMV,instruccion);

	t_struct_instruccion* estructura =(t_struct_instruccion*)socket_recibir_estructura(sockUMV);
	t_intructions inst = estructura->inst;
	return inst;
}

void recibirProximaInstruccion(int sockUMV) {
	t_struct_string* estructura = socket_recibir_estructura(sockUMV);
	char* const string = estructura->string;
	proximaInstruccion = strdup(string);
}

int esPrimerContexto() {
	int a = (*pcb.c_stack == *pcb.stack );
	return (a);
}

t_puntero calcularPosicionAsignacionCPU(int top_index) {
	t_puntero posicion=0;
	if(top_index == -1) {
		posicion = top_index +1;
	} else {
		posicion = top_index + 2;
	}
	return posicion;}


int esPar(int numero){
	return (numero % 2 == 0);
}


t_puntero calcularPosicionAsignacion(int top_index) {
	t_puntero posicion;
	if(esConRetorno) {
		if(esPar(top_index)) {
				posicion = top_index + 1;
			} else {
				posicion = calcularPosicionAsignacionCPU(top_index);
			}
	} else {
		if(esPar(top_index)) {
				posicion = calcularPosicionAsignacionCPU(top_index);
			} else {
				posicion = top_index +1;
			}
	}
	return posicion;
}

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

/******************************************** RESERVAR CONTEXTO ***********************************************************/

void reservarContextoSinRetorno() {
	int cursor = *(pcb.c_stack);

	t_puntero posicionContextoViejo;
	posicionContextoViejo = calcularPosicionAsignacion(top_index);

	//Pushear cursor de stack
	socket_and_push(sockUMV,posicionContextoViejo,cursor);
	top_index = posicionContextoViejo +1;

	//Pushear Program Counter de proxima instruccion:
	int pc  = pcb.program_counter + 1;
	socket_and_push(sockUMV,top_index,pc);

	//Borrar diccionario y todos los elementos. Cuando lo regenero, los vuelvo a crear.
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

void reservarContextoConRetorno(t_puntero donde_retornar){

	int retornar = donde_retornar;
	reservarContextoSinRetorno();

	top_index += 1;

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);
	socket_and_push(sockUMV,retornar,top_index);

	esConRetorno = 1;
}

/******************** RECUPERAR CONTEXTO SIN RETORNO***********************/

void recuperarPosicionDeDirecciones() {
	//Esto va?
	socket_and_modificar_top_index (sockUMV,*(pcb.c_stack)-1);

	top_index = *pcb.c_stack -1;
}

void recuperarProgramCounter(t_puntero* program_counter) {

	socket_and_pop_position(sockUMV,top_index);

	//Socket a UMV para que haga POP_POSITION(pila,top_index);
	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	*program_counter = estructura->numero;

	top_index -= 1;

	//esto va?
	socket_and_modificar_top_index (sockUMV,top_index);
}

void recuperarCursorAnterior(t_puntero* cursor_stack_viejo) {

	socket_and_pop_position(sockUMV,top_index);

	//Socket a UMV para que haga POP_POSITION(pila,top_index);
	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	*cursor_stack_viejo = estructura->numero;

	top_index -= 1;

	//esto va?
	socket_and_modificar_top_index (sockUMV,top_index);

}

void volverAContextoAnterior(t_puntero* c_stack_viejo) {
	t_puntero program_counter;

	recuperarProgramCounter(&program_counter);
	recuperarCursorAnterior(c_stack_viejo);

	pcb.program_counter = program_counter;

	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

void guardarAlternado () {
	//Socket a UMV para que haga TOP(pila) HACEEEEEEEERRRRRR
	//socket_enviarSignal(sockUMV, TOP);

	t_struct_char* estructura =(t_struct_char*)socket_recibir_estructura(sockUMV);
	t_valor_variable identificador_variable = estructura->letra;

	insertarEnDiccionario(identificador_variable, top_index);

	top_index -=2;

}

void regenerarDiccionario(int tamanio_contexto) {
	int i = 0;
	int top = top_index;

	top_index -=1;
	while (i < tamanio_contexto) {
		guardarAlternado();
		i++;
	}

	top_index = top;
	socket_and_modificar_top_index (sockUMV,top);

	pcb.tamanio_contexto = tamanio_contexto;

}

uint32_t calcularTamanioContextoAnterior(t_puntero direccion_contexto_viejo) {
	uint32_t diferencia = (*pcb.c_stack) - direccion_contexto_viejo;

	float dif=(float)(diferencia / 2.5);
	int enteraDeDif = (int)dif;

	return enteraDeDif; //Divido esa cantidad de bytes por 5 (1 byte de id de variable, y 4 bytes del valor) para saber cuantas variables habia.
}

/******************** RECUPERAR CONTEXTO CON RETORNO***********************/
void recuperarDireccionRetorno(t_puntero* direccion_retorno) {
	socket_and_pop_position(sockUMV,top_index);
	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	*direccion_retorno = estructura->numero;

	top_index -= 1;
	socket_and_modificar_top_index (sockUMV,top_index);
}
