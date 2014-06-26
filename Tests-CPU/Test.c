/*
 * Test.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "Test.h"

int main() {
	CU_initialize_registry();

	CU_pSuite prueba1 = CU_add_suite("Suite de prueba 1",inicializar, limpiar);
	CU_add_test(prueba1,"definir variable",testDefinirVariable);
	CU_add_test(prueba1,"obtener posicion variable",testObtenerPosicionVariable);
	CU_add_test(prueba1,"asignar", testAsignar);
	CU_add_test(prueba1,"desreferenciar", testDesreferenciar);


	CU_pSuite prueba2 = CU_add_suite("Suite de prueba 2",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba2,"irAlLabel",testIrALabel);

	CU_pSuite prueba3 = CU_add_suite("Suite de prueba 3",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba3,"reservar sin retorno",testReservarSinRetorno);

	CU_pSuite prueba4 = CU_add_suite("Suite de prueba 4",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba4,"llamar sin retorno",testLlamarSinRetorno);

	CU_pSuite prueba5 = CU_add_suite("Suite de prueba 5",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba5,"llamar con retorno",testLlamarConRetorno);

	CU_pSuite prueba6 = CU_add_suite("Suite de prueba 6",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba6,"finalizar", testFinalizar);


	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}

int inicializar() {
	pila = CREATE_STACK(100);
	top_index = pila->top_index;
	diccionario = dictionary_create();
	return 0;
}

int inicializarConIndices() {
	pila = CREATE_STACK(100);
	top_index = pila->top_index;
	proximaInstruccion = malloc(50);
	indiceEtiquetas=malloc(1000);
	indiceCodigo = malloc(500);

	crearPrograma();

	return 0;
}

int limpiar() {
	DESTROY_STACK(pila);
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

	return 0;
}

int limpiarConIndices() {
	DESTROY_STACK(pila);
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);
	free(segmentoCodigo);
	free(indiceEtiquetas);
	free(indiceCodigo);
	free(proximaInstruccion);
	/*free(pcb.c_stack);
	free(pcb.stack);*/

	return 0;
}

void crearPrograma () {
	segmentoCodigo = malloc(400);
	strcpy(segmentoCodigo,"begin \n # primero declaro las variables \n variables a, b \n a = 20 \n a <- doble\n print a \n end \n function doble\n variables f \n f = $0 + $ 0\n return f \n end\n");

	t_medatada_program *datos;
	datos = metadatada_desde_literal(segmentoCodigo);

	memcpy(indiceEtiquetas,datos->etiquetas,datos->etiquetas_size);

	memcpy(indiceCodigo,datos->instrucciones_serializado,300);

	//crearPcb(datos);

	free(datos->etiquetas);
	free(datos->instrucciones_serializado);
	free(datos);
}


