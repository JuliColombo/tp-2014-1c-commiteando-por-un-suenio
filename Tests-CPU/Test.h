/*
 * Test.h
 *
 *  Created on: 22/06/2014
 *      Author: utnso
 */

#ifndef TEST_H_
#define TEST_H_
#include "tests-primitivas.h"

extern t_dictionary* diccionario;
extern t_stack* pila;
int top_index;
extern char* indiceEtiquetas;
extern char* segmentoCodigo;
extern t_intructions* indiceCodigo;

extern char* proximaInstruccion;


int inicializar();
int inicializarConIndices();
int limpiar();
int limpiarConIndices();
void crearPrograma ();
void crearPcb ();



void elemento_delete(t_elemento* elemento);

#endif /* TEST_H_ */
