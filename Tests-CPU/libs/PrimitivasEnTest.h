/*
 * PrimitivasEnTest.h
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#ifndef PRIMITIVASENTEST_H_
#define PRIMITIVASENTEST_H_

#include "commons/collections/dictionary.h"
#include "stack.h"
#include "parser/metadata_program.h"
#include "funcionesAux.h"
#include <stdint.h>

/*typedef unsigned int t_pid;
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
} t_struct_pcb;*/

extern int top_index;
t_stack* pila;
t_dictionary* diccionario;
char* indiceEtiquetas;
char* segmentoCodigo;
t_intructions* indiceCodigo;
char* proximaInstruccion;
t_struct_pcb pcb;

/*typedef struct{
	char* name;
	t_puntero pos;
}t_elemento;*/

void reservarContextoSinRetorno();
int esPrimerContexto();
void regenerarDiccionario(int tamanio_contexto);
void volverAContextoAnterior(t_puntero* c_stack_viejo);
void recuperarPosicionDeDirecciones();

t_puntero definirVariable(t_nombre_variable identificador_variable);
t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable);
void asignar(t_puntero direccion_variable, t_valor_variable valor);
t_valor_variable dereferenciar(t_puntero direccion_variable);
void irAlLabel(t_nombre_etiqueta etiqueta);
void llamarSinRetorno(t_nombre_etiqueta etiqueta);
void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar);
void finalizar();
void retornar();

#endif /* PRIMITIVASENTEST_H_ */
