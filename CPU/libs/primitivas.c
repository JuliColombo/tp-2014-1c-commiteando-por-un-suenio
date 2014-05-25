/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "primitivasAux.h"
#include "socket.h"
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


t_stack* pila;
t_dictionary *diccionario;
int sockfd;

//t_pcb pcb:

int stack_base;
int c_stack;
int program_counter;
int tamanio_contexto;
char* etiquetas;
t_size etiquetas_size;



t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;

	t_puntero posicion = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(&id,pila,posicion);

	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,posicion);
	dictionary_put(diccionario,elem->name,elem); //Elimino elementos junto con diccio

	//pcb->program_counter +=1;
	//pcb->tamanio_contexto += 1;

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
	return POP_DESREFERENCIAR(pila, direccion_variable);
}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {
	int top_index = pila->top_index;
	PUSH_SIZE_CHECK(&valor,pila,direccion_variable);
	if(top_index < direccion_variable) {
		pila->top_index = direccion_variable;
	} else {
	pila->top_index= top_index; }
	//pcb->program_counter += 1;
}


t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {
	//OBTENER VALOR de una variable COMPARTIDA
	//pide al kernel el valor (copia, no puntero) de la variable compartida por parametro.

	return 0;
}


t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {
	//ASIGNAR VALOR a variable COMPARTIDA
	//pide al kernel asignar el valor a la variable compartida.
	//devuelve el valor asignado.


	//pcb->program_counter += 1;
	return 0;
}


void irAlLabel(t_nombre_etiqueta etiqueta) {
	//cambia la linea de ejecucion a la correspondiente de la etiqueta buscada.

	//reservarContextoSinRetorno(); Tambien?

	int posicionAPushear =  pila->top_index +1;
	//pcb->cursor_stack = &posicionAPushear;  entonces esto no, por lo de arriba

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);

	//Cambiar Program Counter

}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {
	//Preserva el contexto de ejecución actual para poder retornar luego al mismo.
	//Modifica las estructuras correspondientes para mostrar un nuevo contexto vacío.
	//Los parámetros serán definidos luego de esta instrucción de la misma manera que una variable local, con identificadores numéricos empezando por el 0.

	reservarContextoSinRetorno();

	int posicionAPushear =  pila->top_index +1;
	//pcb->cursor_stack = &posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);

	//Ejecutar instruccion

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {
	//Preserva el contexto de ejecución actual para poder retornar luego al mismo, junto con la posicion de la variable entregada por donde_retornar.
	//Modifica las estructuras correspondientes para mostrar un nuevo contexto vacío.
	//Los parámetros serán definidos luego de esta instrucción de la misma manera que una variable local, con identificadores numéricos empezando por el 0.

	//definir variables si hay parametros
	//Asignar a parametros
	//Hacer algo con el tamaño de contexto

	reservarContextoConRetorno();
	int posicionAPushear =  pila->top_index +1;
	//pcb->cursor_stack = &posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);

	//Ejecutar instruccion

}

void finalizar() {
	//Cambia el Contexto de Ejecución Actual para volver al Contexto anterior al que se está ejecutando, recuperando el Cursor de Contexto Actual y el Program Counter previamente apilados en el Stack.
	//En caso de estar finalizando el Contexto principal (el ubicado al inicio del Stack), deberá finalizar la ejecución del programa.

	if(c_stack == stack_base) {

	volverAContextoAnterior();
	regenerarDiccionario(pila,tamanio_contexto);
	} else {

		volverAContextoAnterior();
		regenerarDiccionario(pila,tamanio_contexto);
		//Hay que hacer funcion para empezar la limpieza para terminar con el programa en ejecucion
	}
}

void retornar(t_valor_variable retorno){
	//Cambia el Contexto de Ejecución Actual para volver al Contexto anterior al que se está ejecutando, recuperando el Cursor de Contexto Actual, el Program Counter y la direccion donde retornar, asignando el valor de retorno en esta, previamente apilados en el Stack.


	t_puntero posicionVariable = POP_RETORNAR(pila, c_stack);
	t_valor_variable* ret = &retorno;
	PUSH_SIZE_CHECK(ret,pila,posicionVariable);
	volverAContextoAnterior();
	regenerarDiccionario(pila,tamanio_contexto);

}

int imprimir(t_valor_variable valor_mostrar) {
	//Envía valor_mostrar al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución.

	return 0;
}

int imprimirTexto(char* texto) {
	//Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.
	return 0;
}

int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
	//Informa al Kernel que el Programa actual pretende usar el dispositivo tiempo unidades de tiempo.
	return 0;
}

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
