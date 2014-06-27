/*
 * funcionesAux.h
 *
 *  Created on: 27/06/2014
 *      Author: utnso
 */

#ifndef FUNCIONESAUX_H_
#define FUNCIONESAUX_H_

#include <stdint.h>
#include <stdlib.h>
#include "stack.h"
#include "parser/metadata_program.h"

typedef unsigned int t_pid;
typedef int* t_segmento_codigo;
typedef int* t_segmento_stack;
typedef int* t_cursor_stack;
typedef int* t_index_codigo;
typedef int* t_index_etiquetas;
typedef int t_program_counter;
typedef int t_tamanio_contexto;
typedef int t_tamanio_indice;

typedef struct struct_pcb{
		t_pid pid;								//Identificador único del Programa en el sistema
		t_segmento_codigo codigo;				//Dirección del primer byte en la UMV del segmento de código
		t_segmento_stack stack;					//Dirección del primer byte en la UMV del segmento de stack
		t_cursor_stack c_stack;					//Dirección del primer byte en la UMV del Contexto de Ejecución Actual
		t_index_codigo index_codigo;			//Dirección del primer byte en la UMV del Índice de Código
		t_index_etiquetas index_etiquetas;		//Dirección del primer byte en la UMV del Índice de Etiquetas
		t_program_counter	program_counter;	//Número de la próxima instrucción a ejecutar
		t_tamanio_contexto tamanio_contexto;	//Cantidad de variables (locales y parámetros) del Contexto de Ejecución Actual
		t_tamanio_indice tamanio_indice;		//Cantidad de bytes que ocupa el Índice de etiquetas
} t_struct_pcb;

t_dictionary* diccionario;
char* indiceEtiquetas;
char* segmentoCodigo;
t_intructions* indiceCodigo;
char* proximaInstruccion;
t_struct_pcb pcb;
extern int top_index;
t_stack* pila;

typedef struct{
	char* name;
	t_puntero pos;
}t_elemento;


char* buscarEnSegmentoCodigo(t_intructions instruccion);
t_puntero calcularPosicionAsignacionCPU(int top_index);
int esPrimerContexto();
int esPar(int numero);
t_puntero calcularPosicionAsignacion(int top_index);
char* convertirAString(t_nombre_variable c);
t_elemento* elemento_create(const char* name, t_puntero pos);
void elemento_delete(t_elemento* elemento);
void recuperarPosicionDeDirecciones();
void recuperarProgramCounter(t_puntero* program_counter);
void recuperarCursorAnterior(t_puntero* cursor_stack_viejo);
void volverAContextoAnterior(t_puntero* c_stack_viejo);
void guardarAlternado ();
void regenerarDiccionario(int tamanio_contexto);
uint32_t calcularTamanioContextoAnterior(t_puntero direccion_contexto_viejo);
void recuperarDireccionRetorno(t_puntero* direccion_retorno);


#endif /* FUNCIONESAUX_H_ */
