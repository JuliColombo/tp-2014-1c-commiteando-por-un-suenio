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

extern char* indiceEtiquetasBis;

extern char* proximaInstruccion;


int inicializar();
int inicializarConIndices();
int inicializarConIndices2();
int inicializarConIndices3();
int inicializarConIndices4();
int limpiar();
int limpiarConIndices();
int limpiarConIndices2();
void crearPrograma ();
void crearPrograma2 ();
void crearPrograma3 ();
void crearPrograma4 ();



void elemento_delete(t_elemento* elemento);

#endif /* TEST_H_ */
