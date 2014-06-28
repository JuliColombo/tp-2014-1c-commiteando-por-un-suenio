/*
 * tests-primitivas.h
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#ifndef TESTS_PRIMITIVAS_H_
#define TESTS_PRIMITIVAS_H_

#include "CUnit/Basic.h"
#include <stdio.h>
#include <stdlib.h>
#include "libs/PrimitivasEnTest.h"
#include "Test.h"

t_stack* pila;
int top_index;
char* proximaInstruccion;
t_dictionary* diccionario;
extern t_struct_pcb pcb;

void crearPcb();
void liberarPcb();

void testReservarSinRetorno();

void testDefinirVariable();
void testObtenerPosicionVariable();
void testAsignar();
void testDesreferenciar();
void testIrALabel();
void testLlamarSinRetorno();
void testLlamarConRetorno();
void testFinalizar();
void testRetornar();
void testIntegracion();


#endif /* TESTS_PRIMITIVAS_H_ */
