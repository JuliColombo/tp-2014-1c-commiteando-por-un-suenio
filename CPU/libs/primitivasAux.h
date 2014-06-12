/*
 * primitivasAux.h
 *
 *  Created on: 22/05/2014
 *      Author: utnso
 */

#ifndef PRIMITIVASAUX_H_
#define PRIMITIVASAUX_H_

#include <parser/metadata_program.h>
#include <commons/collections/dictionary.h>
#include "Sockets/socket.h"

typedef struct{
	char* name;
	t_puntero pos;
}t_elemento;

typedef struct{
	unsigned int pid;									//Identificador único del Programa en el sistema

	t_puntero codigo;									//Dirección del primer byte en la UMV del segmento de código
	t_puntero index_codigo;		    					//Dirección del primer byte en la UMV del Índice de Código

	t_puntero stack;									//Dirección del primer byte en la UMV del segmento de stack
	t_puntero c_stack;									//Dirección del primer byte en la UMV del Contexto de Ejecución Actual

	t_size tamanio_indice;								//Cantidad de bytes que ocupa el Índice de etiquetas
	t_puntero index_etiquetas;							//Dirección del primer byte en la UMV del Índice de Etiquetas

	t_puntero_instruccion	program_counter;			//Número de la próxima instrucción a ejecutar
	t_size tamanio_contexto;							//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
} t_pcb;

t_pcb pcb;

t_puntero calcularPosicionAsignacionCPU(int top_index);

void reservarContextoSinRetorno();

void reservarContextoConRetorno();

char* convertirAString(t_nombre_variable c);

t_elemento* elemento_create(const char* name, t_puntero pos);

void elemento_delete(t_elemento* elemento);

void regenerarDiccionario(int tamanio_contexto);

void guardarAlternado ();

void volverAContextoAnterior();


#endif /* PRIMITIVASAUX_H_ */
