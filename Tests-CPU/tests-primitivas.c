/*
 * tests-primitivas.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "tests-primitivas.h"
int esConRetorno;

void crearPcb() {
	pcb.pid = 4;
	pcb.index_codigo = 0;
	pcb.index_etiquetas = 0;
	pcb.stack = malloc(sizeof(int));
	*pcb.stack = 0;
	pcb.c_stack = malloc(sizeof(int));
	*pcb.c_stack = 0;
	pcb.codigo = 0;
	pcb.program_counter = 0;
	pcb.tamanio_contexto = 0;
	pcb.tamanio_indice = 50;

	esConRetorno = 0;
}


void testDefinirVariable(){
	crearPcb();
	definirVariable('a');
	definirVariable('b');
	definirVariable('c');
	CU_ASSERT_EQUAL(pila->elementos[0],'a');
	CU_ASSERT_EQUAL(pila->elementos[2],'b');
	CU_ASSERT_EQUAL(pila->elementos[4],'c');
	CU_ASSERT_EQUAL(top_index,4);
	CU_ASSERT_FALSE(dictionary_is_empty(diccionario));
}

void testObtenerPosicionVariable(){
	int pos1 = obtenerPosicionVariable('a');
	int pos2 = obtenerPosicionVariable('b');
	int pos3 = obtenerPosicionVariable('c');
	CU_ASSERT_EQUAL(pos1, 0);
	CU_ASSERT_EQUAL(pos2, 2);
	CU_ASSERT_EQUAL(pos3, 4);
}

void testAsignar(){
	CU_ASSERT_EQUAL(top_index,4);
	asignar(4,2048);
	asignar(0,5);
	asignar(2,-19);
	CU_ASSERT_EQUAL(pila->elementos[1],5);
	CU_ASSERT_EQUAL(pila->elementos[3],-19);
	CU_ASSERT_EQUAL(pila->elementos[5],2048);
	CU_ASSERT_EQUAL(top_index,5);

}

void testDesreferenciar(){
	int valor_variable;
	valor_variable = dereferenciar(4);
	CU_ASSERT_EQUAL(valor_variable,2048);
	valor_variable = dereferenciar(0);
	CU_ASSERT_EQUAL(valor_variable,5);
	valor_variable = dereferenciar(2);
	CU_ASSERT_EQUAL(valor_variable,-19);
}

void testIrALabel(){
	crearPcb();
	irAlLabel("doble");

	CU_ASSERT_EQUAL(pcb.program_counter, 5);
	CU_ASSERT_STRING_EQUAL(proximaInstruccion," variables f \n");

}

void testReservarSinRetorno() {
	crearPcb();
	definirVariable('a');
	reservarContextoSinRetorno();

	CU_ASSERT_EQUAL(pcb.program_counter, 2);
	CU_ASSERT_EQUAL(*pcb.c_stack, 0);
	CU_ASSERT_EQUAL(pila->elementos[2],0);
	CU_ASSERT_EQUAL(pila->elementos[3],2);
	CU_ASSERT_EQUAL(pcb.tamanio_contexto, 1);
	CU_ASSERT_EQUAL(pila->top_index,top_index);
	CU_ASSERT_TRUE(dictionary_is_empty(diccionario));

}

void testLlamarSinRetorno() {
	crearPcb();
	definirVariable('a');
	asignar(0,5);
	CU_ASSERT_EQUAL(pcb.tamanio_contexto, 1);
	CU_ASSERT_EQUAL(*pcb.c_stack, 0);

	llamarSinRetorno("doble");
	CU_ASSERT_EQUAL(pcb.tamanio_contexto, 0);
	CU_ASSERT_EQUAL(pcb.program_counter, 5);
	CU_ASSERT_EQUAL(*pcb.c_stack, 4);
	CU_ASSERT_EQUAL(pila->top_index,top_index);
	CU_ASSERT_STRING_EQUAL(proximaInstruccion," variables f \n");

}

void testLlamarConRetorno() {
	crearPcb();
	definirVariable('a');
	definirVariable('b');
	asignar(0,5);
	CU_ASSERT_EQUAL(pcb.tamanio_contexto, 2);
	CU_ASSERT_EQUAL(*pcb.c_stack, 0);

	llamarConRetorno("doble",obtenerPosicionVariable('b'));
	CU_ASSERT_EQUAL(pcb.tamanio_contexto, 0);
	CU_ASSERT_EQUAL(pcb.program_counter, 5);
	CU_ASSERT_EQUAL(*pcb.c_stack, 7);
	CU_ASSERT_EQUAL(pila->elementos[4],0);
	CU_ASSERT_EQUAL(pila->elementos[5],4);
	CU_ASSERT_EQUAL(pila->elementos[6],2);
	CU_ASSERT_EQUAL(pila->top_index,6);
	CU_ASSERT_STRING_EQUAL(proximaInstruccion," variables f \n");

	definirVariable('a');
	definirVariable('b');
	asignar(7,19);

	CU_ASSERT_TRUE(dictionary_has_key(diccionario, "a"));
	CU_ASSERT_TRUE(dictionary_has_key(diccionario, "b"));
	CU_ASSERT_EQUAL(pila->elementos[7],'a');
	CU_ASSERT_EQUAL(pila->elementos[9],'b');
	CU_ASSERT_EQUAL(pila->elementos[8],19);
	CU_ASSERT_EQUAL(pcb.tamanio_contexto, 2);
	CU_ASSERT_EQUAL(*pcb.c_stack,7);
	CU_ASSERT_EQUAL(pila->top_index,top_index);
	esConRetorno = 0;
}

void testFinalizar() {
	crearPcb();
	definirVariable('a');
	asignar(0,5);
	definirVariable('c');
	asignar(2,-19);
	definirVariable('d');
	asignar(4,2048);

	llamarSinRetorno("doble");
	definirVariable('b');

	finalizar();

	CU_ASSERT_EQUAL(top_index,5);
	CU_ASSERT_EQUAL(pila->top_index,top_index);

	CU_ASSERT_EQUAL(dictionary_size(diccionario),3);
	CU_ASSERT_TRUE(dictionary_has_key(diccionario, "a"));
	CU_ASSERT_TRUE(dictionary_has_key(diccionario, "c"));
	CU_ASSERT_TRUE(dictionary_has_key(diccionario, "d"));
	CU_ASSERT_FALSE(dictionary_has_key(diccionario, "b"));

	CU_ASSERT_EQUAL(pcb.tamanio_contexto, 3);
	CU_ASSERT_EQUAL(*pcb.c_stack,0);
	}


