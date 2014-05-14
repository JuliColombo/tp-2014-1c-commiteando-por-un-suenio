/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "parser/metadata_program.h"
#include "stack.h"
#include "commons/collections/dictionary.h"


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
		posicion = P->top_index++;
	} else {
		posicion = P->top_index + 5;
	}
	return posicion;}


void reservarContexto(){
	t_puntero posicionContextoViejo,posicionAVariable;
	int posicionVar = pila->top_index;
	int* cursor = pila->cursor_stack;

	posicionContextoViejo = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(cursor,pila,posicionContextoViejo);

	//Pushear Program Counter de proxima instruccion

	posicionAVariable = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);
}


//No entendi si devuelve la posicion de la variable en la pila
t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;
	t_puntero posicion = calcularPosicionAsignacion(pila);
	PUSH_SIZE_CHECK(&id,pila,posicion);
	dictionary_put(diccionario,&identificador_variable,&posicion); //en data va la posicion, no?
	return posicion;
}


t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {
	t_puntero* posicion = 0;
	if(dictionary_has_key(diccionario,&identificador_variable)) {
		posicion = dictionary_get(diccionario,&identificador_variable);
	} else {
		*posicion = -1;
	}
	return *posicion;
}


t_valor_variable dereferenciar(t_puntero direccion_variable) {
	//Obtiene el valor resultante de leer a partir de direccion_variable, sin importar cual fuera el contexto actual
	//Le pido a UMV los 4 bytes a partir del offset de direccion_variable.

	return 0;
}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {
	//Inserta una copia del valor en la variable ubicada en direccion_variable.
	//Almaceno en la UMV, en direccion_variable, el valor.
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
	return 0;
}


t_puntero_instruccion irAlLabel(t_nombre_etiqueta etiqueta) {
	//cambia la linea de ejecucion a la correspondiente de la etiqueta buscada.

	return 0;
}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {
	//Preserva el contexto de ejecución actual para poder retornar luego al mismo.
	//Modifica las estructuras correspondientes para mostrar un nuevo contexto vacío.
	//Los parámetros serán definidos luego de esta instrucción de la misma manera que una variable local, con identificadores numéricos empezando por el 0.

	reservarContexto();
	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {
	//Preserva el contexto de ejecución actual para poder retornar luego al mismo, junto con la posicion de la variable entregada por donde_retornar.
	//Modifica las estructuras correspondientes para mostrar un nuevo contexto vacío.
	//Los parámetros serán definidos luego de esta instrucción de la misma manera que una variable local, con identificadores numéricos empezando por el 0.

	//definir variables si hay parametros
	//Asignar a parametros
	//Hacer algo con el tamaño de contexto

	reservarContexto();
	int posicionVar = pila->top_index;
	PUSH_SIZE_CHECK(&posicionVar,pila,donde_retornar);
	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,etiquetas_size);


}

t_puntero_instruccion finalizar() {
	//Cambia el Contexto de Ejecución Actual para volver al Contexto anterior al que se está ejecutando, recuperando el Cursor de Contexto Actual y el Program Counter previamente apilados en el Stack.
	//En caso de estar finalizando el Contexto principal (el ubicado al inicio del Stack), deberá finalizar la ejecución del programa devolviendo.
	return 0;
}

t_puntero retornar(){
	//Cambia el Contexto de Ejecución Actual para volver al Contexto anterior al que se está ejecutando, recuperando el Cursor de Contexto Actual, el Program Counter y la direccion donde retornar, asignando el valor de retorno en esta, previamente apilados en el Stack.
	return 0;
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
