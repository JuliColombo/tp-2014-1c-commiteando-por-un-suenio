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
#include "parser/parser.h"
#include "funcionesAux.h"
#include <stdint.h>
#include <unistd.h>
#include <signal.h>

extern int top_index;
t_stack* pila;
t_dictionary* diccionario;
char* indiceEtiquetas;
char* indiceEtiquetasBis;
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

t_puntero definirVariableTest(t_nombre_variable identificador_variable);
t_puntero obtenerPosicionVariableTest(t_nombre_variable identificador_variable);
void asignarTest(t_puntero direccion_variable, t_valor_variable valor);
t_valor_variable dereferenciar(t_puntero direccion_variable);
void irAlLabel(t_nombre_etiqueta etiqueta);
void llamarSinRetorno(t_nombre_etiqueta etiqueta);
void llamarConRetornoTest(t_nombre_etiqueta etiqueta, t_puntero donde_retornar);
void finalizar();
void retornar();
void imprimir();
void integracionScriptFacil();
void integracionConFuncionDoble();
void integracionFor();
void integracionCorrerParser();

/*AnSISOP_kernel funciones_kernel;
AnSISOP_funciones funciones_parser;*/

#endif /* PRIMITIVASENTEST_H_ */
