/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "primitivasAux.h"
#include "Sockets/socket.h"
#include "primitivas.h"
#include <commons/collections/dictionary.h>
#include <parser/metadata_program.h>


/*typedef struct{
	t_pid pid;								//Identificador único del Programa en el sistema
	t_segmento_codigo codigo;				//Dirección del primer byte en la UMV del segmento de código
	t_segmento_stack stack;					//Dirección del primer byte en la UMV del segmento de stack
	t_cursor_stack c_stack;					//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
	t_index_codigo index_codigo;			//Dirección del primer byte en la UMV del Índice de Código
	t_index_etiquetas index_etiquetas;		//Dirección del primer byte en la UMV del Índice de Etiquetas
	t_program_counter	program_counter;	//Número de la próxima instrucción a ejecutar
	t_tamanio_contexto tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
	t_tamanio_indice tamanio_indice;		//Cantidad de bytes que ocupa el Índice de etiquetas
} t_pcb;*/


t_dictionary *diccionario;
int sockfd;
int sockAjeno;
int top_index;

//t_pcb pcb:
int stack_base;
int c_stack;
t_puntero_instruccion program_counter;
int tamanio_contexto;
char* etiquetas;
t_size etiquetas_size;
t_intructions* index_codigo;



t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;
	t_puntero posicion = calcularPosicionAsignacionCPU(top_index);

	//Socket enviando posicion e id para que la UMV pushee
	//En UMV: PUSH_SIZE_CHECK(&id,pila,posicion);
	struct_push* estructura = crear_struct_push(posicion,id);
	socket_enviar(sockAjeno,STRUCT_PUSH,estructura);

	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas como asignar


	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,posicion);
	dictionary_put(diccionario,elem->name,elem); //Elimino elementos junto con diccio despues

	program_counter +=1;
	tamanio_contexto += 1;

	return posicion;
}


t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {
	t_puntero posicion = 0;
	char* str = convertirAString(identificador_variable);
	if(dictionary_has_key(diccionario,str)) {
		t_elemento* elemento = dictionary_get(diccionario,str);
		posicion = elemento->pos;
	} else {
		posicion = -1;
	}
	return posicion;
}


t_valor_variable dereferenciar(t_puntero direccion_variable) {
	//Socket enviando direccion_variable a UMV para que haga pop
	//En UMV POP_DESREFERENCIAR(pila, direccion_variable)
	struct_pop_desreferenciar* estructura = crear_struct_pop_desreferenciar(direccion_variable);
	socket_enviar(sockAjeno,STRUCT_POP_DESREFERENCIAR,estructura);

	//Socket recibiendo t_valor_variable id
	//return id;
	return 0; //no va
}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {
	//top_index que se actualiza cada vez que UMV pushea o popea

	//Socket enviando direccion_variable y valor a UMV
	//En UMV PUSH_SIZE_CHECK(&valor,pila,direccion_variable);
	struct_push* estructura = crear_struct_push(direccion_variable,valor);
	socket_enviar(sockAjeno,STRUCT_PUSH,estructura);

	if(top_index < direccion_variable) {
		top_index = direccion_variable;
	}

	//Le digo a UMV que actualice su top_index del stack
	//Socket enviando top_index para que UMV haga: pila->top_index = top_index;
	program_counter += 1;
}


t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {

	//Socket recibiendo la copia (no puntero) del valor de la "variable"

	return 0;
}


t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {

	//Socket enviando Kernel para que asigne el "valor" a "variable"

	program_counter += 1;
	return 0;
}


void irAlLabel(t_nombre_etiqueta etiqueta) {

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);

	program_counter = instruccion;

	//Busco en indice de codigo qué le pido a UMV -----> creo que esto es asi

	//Socket enviando a UMV el start y offset para que me pase la instruccion a ejecutar

	//Socket recibiendo la instruccion a ejecutar de UMV
	//Meto eso en analizador_de_linea... para invocar al parser


}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {
	//definir variables si hay parametros
	//Asignar a parametros
	//HACER ALGO CON TAMAÑO DE CONTEXTO

	reservarContextoSinRetorno();

	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear =  top_index +1;
	c_stack = posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);
	program_counter = instruccion;
	t_intructions inst = index_codigo[instruccion];

	//Busco en indice de codigo qué le pido a UMV

	//Socket enviando a UMV el start y offset para que me pase la instruccion a ejecutar

	//Socket recibiendo la instruccion a ejecutar de UMV
	//Meto eso en analizador_de_linea... para invocar al parser
}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	//definir variables si hay parametros
	//Asignar a parametros
	//HACER ALGO CON TAMAÑO DE CONTEXTO

	reservarContextoConRetorno();
	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear = top_index +1;
	c_stack = posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);
	program_counter = instruccion;
	t_intructions inst = index_codigo[instruccion];

	//Busco en indice de codigo qué le pido a UMV

	//Socket enviando a UMV el start y offset para que me pase la instruccion a ejecutar

	//Socket recibiendo la instruccion a ejecutar de UMV
	//Meto eso en analizador_de_linea... para invocar al parser


}

void finalizar() {

	if(c_stack /= stack_base) {

	volverAContextoAnterior();
	regenerarDiccionario(tamanio_contexto);
	} else {

		volverAContextoAnterior();
		regenerarDiccionario(tamanio_contexto);
		//Hay que hacer funcion para empezar la limpieza para terminar con el programa en ejecucion
	}
}

void retornar(t_valor_variable retorno){
	//Socket de UMV para yo darle un valor a posicionVariable ---> t_puntero posicionVariable = POP_RETORNAR(pila, c_stack);
	//Socket de UMV para actualizar mi top_index

	t_valor_variable* ret = &retorno;
	//Socket a UMV para que haga: PUSH_SIZE_CHECK(ret,pila,posicionVariable);
	//Socket de UMV para actualizar mi top_index
	volverAContextoAnterior();
	regenerarDiccionario(tamanio_contexto);

}

int imprimir(t_valor_variable valor_mostrar) {
	//Envía valor_mostrar al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución.

	//Socket a Kernel enviandole el valor a mostrar

	return 0;
}

int imprimirTexto(char* texto) {
	//Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.

	//Socket a Kernel enviandole texto
	return 0;
}

int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
	//Informa al Kernel que el Programa actual pretende usar el dispositivo tiempo unidades de tiempo.
	return 0;
}


/****************************** OPERACIONES DE KERNEL ************************************************/

int wait(t_nombre_semaforo identificador_semaforo) {
	//Informa al kernel que ejecute la función wait para el semáforo con el nombre identificador_semaforo.
	//El kernel deberá decidir si bloquearlo o no.
	return 0;
}

int signal(t_nombre_semaforo identificador_semaforo) {
	//Informa al kernel que ejecute la función signal para el semáforo con el nombre identificador_semaforo.
	//El kernel deberá decidir si desbloquear otros procesos o no.
	return 0;
}
