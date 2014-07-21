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

void chequearSiHuboSF(){
	t_signal* senial;
	socket_recibirSignal(sockUMV,senial);

	if(*senial == D_STRUCT_SEGFAULT){
		termino = SEG_FAULT;
	} else {
		termino = CONTINUES;
	}
}

void controlarBloqueo(int sockKernel, int termino) {
	unsigned int valor_semaforo;
	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j = socket_recibir(sockKernel, &tipoRecibido, &structRecibida);
	if (j == 1) {
		t_struct_numero* k = ((t_struct_numero*) structRecibida);
		valor_semaforo = k->numero;
		free(k);
	}
	if (valor_semaforo == 1) {
		termino = BLOQUEADO;
		//mando un pcbSem a kernel
	}
}

void insertarEnDiccionario(t_nombre_variable identificador_variable,t_puntero posicion) {
	const char* str = convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str, posicion);
	dictionary_put(diccionario, elem->name, elem);
}

t_puntero_instruccion irAIntruccionLabel(t_nombre_etiqueta etiqueta) {
	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta, indiceEtiquetas,pcb->tamanio_indice);
	pcb->program_counter = instruccion;
	return instruccion;
}

t_intructions instruccionParaBuscarEnIndiceCodigo(t_puntero_instruccion instruccion) {

	t_struct_numero* estructura = malloc(sizeof(t_struct_numero));
	estructura->numero = instruccion;
	socket_enviar(sockUMV, D_STRUCT_NUMERO, estructura);
	free(estructura);

	chequearSiHuboSF();

	t_intructions inst;

	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_instruccion* k = ((t_struct_instruccion*)structRecibida);
		inst= k->inst;
		free(k);
	}

	chequearSiHuboSF();

	return inst;
}

void recibirProximaInstruccion(int sockUMV) {

	char* string;

	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_string* k = ((t_struct_string*)structRecibida);
		string= k->string;
		free(k);
	}

	chequearSiHuboSF();

	proximaInstruccion = strdup(string); //que onde el const?
}

/*int esPrimerContexto() {
	int a = (*pcb->c_stack == *pcb->stack );
	return (a);
}*/
int esPrimerContexto(){
	int a = cursor == stack;
	return a;
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
	//int cursor = *(pcb->c_stack);
	int cursorrr = cursor;

	t_puntero posicionContextoViejo;
	posicionContextoViejo = calcularPosicionAsignacion(top_index);

	//Pushear cursor de stack
	t_struct_push* estructura = malloc(sizeof(t_struct_push));
	estructura->posicion = posicionContextoViejo;
	estructura->valor = cursorrr;
	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
	free(estructura);

	chequearSiHuboSF();

	top_index = posicionContextoViejo +1;

	//Pushear Program Counter de proxima instruccion:
	int pc  = pcb->program_counter + 1;

	estructura = malloc(sizeof(t_struct_push));
	estructura->posicion = top_index;
	estructura->valor = pc;
	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
	free(estructura);

	//Borrar diccionario y todos los elementos. Cuando lo regenero, los vuelvo a crear.
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

void reservarContextoConRetorno(t_puntero donde_retornar){

	int retornar = donde_retornar;
	reservarContextoSinRetorno();

	top_index += 1;

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);

	t_struct_push* estructura = malloc(sizeof(t_struct_push));
	estructura->posicion = top_index;
	estructura->valor = retornar;
	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
	free(estructura);

	chequearSiHuboSF();

	esConRetorno = 1;
}

/******************** RECUPERAR CONTEXTO SIN RETORNO***********************/

void recuperarPosicionDeDirecciones() {
	//top_index = *pcb->c_stack -1;
	top_index = cursor -1;
}

void recuperarProgramCounter(t_puntero* program_counter) {

	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion = top_index;
	socket_enviar(sockUMV, D_STRUCT_POP, estructura);
	free(estructura);

	chequearSiHuboSF();

	t_tipoEstructura tipoRecibido;
		void* structRecibida;
		int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
		if(j==1){
			t_struct_numero* k = ((t_struct_numero*)structRecibida);
			*program_counter= k->numero;
			free(k);
		}

	chequearSiHuboSF();

	top_index -= 1;
}

void recuperarCursorAnterior(t_puntero* cursor_stack_viejo) {

	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion = top_index;
	socket_enviar(sockUMV, D_STRUCT_POP, estructura);
	free(estructura);

	chequearSiHuboSF();

	t_tipoEstructura tipoRecibido;
		void* structRecibida;
		int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
		if(j==1){
			t_struct_numero* k = ((t_struct_numero*)structRecibida);
			*cursor_stack_viejo= k->numero;
			free(k);
		}

	chequearSiHuboSF();

	top_index -= 1;

}

void volverAContextoAnterior(t_puntero* c_stack_viejo) {
	t_puntero program_counter;

	recuperarProgramCounter(&program_counter);
	recuperarCursorAnterior(c_stack_viejo);

	pcb->program_counter = program_counter;

	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

void guardarAlternado () {

	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion = top_index;
	socket_enviar(sockUMV, D_STRUCT_POP, estructura);
	free(estructura);

	chequearSiHuboSF();

	t_valor_variable identificador_variable;

	t_tipoEstructura tipoRecibido;
		void* structRecibida;
		int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
		if(j==1){
			t_struct_numero* k = ((t_struct_numero*)structRecibida);
			identificador_variable= k->numero;
			free(k);
		}

	chequearSiHuboSF();

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

	pcb->tamanio_contexto = tamanio_contexto;

}

uint32_t calcularTamanioContextoAnterior(t_puntero direccion_contexto_viejo) {
	//uint32_t diferencia = (*pcb->c_stack) - direccion_contexto_viejo;
	uint32_t diferencia = cursor - direccion_contexto_viejo;

	float dif=(float)(diferencia / 2.5);
	int enteraDeDif = (int)dif;

	return enteraDeDif; //Divido esa cantidad de bytes por 5 (1 byte de id de variable, y 4 bytes del valor) para saber cuantas variables habia.
}

/******************** RECUPERAR CONTEXTO CON RETORNO***********************/
void recuperarDireccionRetorno(t_puntero* direccion_retorno) {

	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion = top_index;
	socket_enviar(sockUMV, D_STRUCT_POP, estructura);
	free(estructura);

	chequearSiHuboSF();

	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_numero* k = ((t_struct_numero*)structRecibida);
		*direccion_retorno= k->numero;
		free(k);
	}

	chequearSiHuboSF();

	top_index -= 1;
}
