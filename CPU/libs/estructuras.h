/*
 * estructuras.h
 *
 *  Created on: Jul 16, 2014
 *      Author: utnso
 */

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_


#include <stdint.h>

typedef unsigned int t_pid;
typedef int t_program_counter;
typedef int t_tamanio_contexto;
typedef int t_tamanio_indice;
typedef int t_segmento_codigo;
typedef int t_segmento_stack;
typedef int t_cursor_stack;
typedef int t_index_codigo;
typedef int t_index_etiquetas;

typedef uint8_t t_quantum;
typedef long t_retardo_quantum;

typedef struct{
	t_pid pid;								//Identificador único del Programa en el sistema
	t_segmento_codigo codigo;				//Dirección del primer byte en la UMV del segmento de código
	t_segmento_stack stack;					//Dirección del primer byte en la UMV del segmento de stack
	t_cursor_stack c_stack;					//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
	t_index_codigo index_codigo;			//Dirección del primer byte en la UMV del Índice de Código
	t_index_etiquetas index_etiquetas;		//Dirección del primer byte en la UMV del Índice de Etiquetas
	t_program_counter	program_counter;	//Número de la próxima instrucción a ejecutar
	t_tamanio_contexto tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
	t_tamanio_indice tamanio_indice;		//Cantidad de bytes que ocupa el Índice de etiquetas
} t_pcb;

enum {
	NORMAL = 0,
	FIN = 1,
	IO = 2,
	SF = 3,
} estado_pcb;

#endif /* ESTRUCTURAS_H_ */
