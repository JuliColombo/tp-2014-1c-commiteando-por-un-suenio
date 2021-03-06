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
	CU_add_test(prueba2,"ir al label",testIrALabel);

	CU_pSuite prueba3 = CU_add_suite("Suite de prueba 3",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba3,"reservar sin retorno",testReservarSinRetorno);

	CU_pSuite prueba4 = CU_add_suite("Suite de prueba 4",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba4,"llamar sin retorno",testLlamarSinRetorno);

	CU_pSuite prueba5 = CU_add_suite("Suite de prueba 5",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba5,"llamar con retorno",testLlamarConRetorno);

	CU_pSuite prueba6 = CU_add_suite("Suite de prueba 6",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba6,"finalizar", testFinalizar);

	CU_pSuite prueba7 = CU_add_suite("Suite de prueba 7",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba7,"retornar", testRetornar);

	CU_pSuite prueba8 = CU_add_suite("Suite de prueba 8",inicializarConIndices2, limpiarConIndices2);
	CU_add_test(prueba8,"integracion script facil", testIntegracionScriptFacil);

	CU_pSuite prueba9 = CU_add_suite("Suite de prueba 9",inicializarConIndices, limpiarConIndices);
	CU_add_test(prueba9,"integracion script con funcion doble", testIntegracionConFuncionDoble);

	CU_pSuite prueba10 = CU_add_suite("Suite de prueba 10",inicializarConIndices3, limpiarConIndices2);
	CU_add_test(prueba10,"integracion script con for", testIntegracionFor);

	CU_pSuite prueba11 = CU_add_suite("Suite de prueba 11",inicializarConIndices4, limpiarConIndices2);
	CU_add_test(prueba11,"integracion script desreferencias", testIntegracionDesreferencia);


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

int inicializarConIndices2() {
	pila = CREATE_STACK(100);
	top_index = pila->top_index;
	proximaInstruccion = malloc(50);
	indiceEtiquetasBis=malloc(1000);
	indiceCodigo = malloc(500);

	crearPrograma2();

	return 0;
}

int inicializarConIndices3() {
	pila = CREATE_STACK(1000);
	top_index = pila->top_index;
	proximaInstruccion = malloc(50);
	indiceEtiquetasBis=malloc(1000);
	indiceCodigo = malloc(500);

	crearPrograma3();

	return 0;
}

int inicializarConIndices4() {
	pila = CREATE_STACK(1000);
	top_index = pila->top_index;
	proximaInstruccion = malloc(50);
	indiceEtiquetasBis=malloc(1000);
	indiceCodigo = malloc(500);

	crearPrograma4();

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

	return 0;
}

int limpiarConIndices2() {
	DESTROY_STACK(pila);
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);
	free(segmentoCodigo);
	free(indiceEtiquetasBis);
	free(indiceCodigo);
	free(proximaInstruccion);

	return 0;
}

/************************************************************** CREACION SCRIPTS **************************************************************************************************/
void auxiliarCrearPrograma() {
	t_medatada_program* datos;
	datos = metadatada_desde_literal(segmentoCodigo);
	memcpy(indiceEtiquetas, datos->etiquetas, datos->etiquetas_size);
	memcpy(indiceCodigo, datos->instrucciones_serializado, 300);

	printf("CANTIDAD DE INSTRUCCIONES %d\n", datos->instrucciones_size);

	free(datos->etiquetas);
	free(datos->instrucciones_serializado);
	free(datos);
}

void pasarScript(char* str) {
	segmentoCodigo = malloc(strlen(str));
	strcpy(segmentoCodigo, str);
}

void crearPrograma () {
	pasarScript("# primero declaro las variables \nbegin \n		variables a, b \n	a = 20 \n	b <- doble a \n		print b \n	 end \n function doble \n variables f \n		f = $0 + $0 \n		return f \n end\n");
	auxiliarCrearPrograma();
}

void crearPrograma2 (){
	pasarScript("#!/usr/bin/ansisop \nbegin \n	variables a, b \n	a = 3 \n	b = 5 \n 	a = b + 12 \n end \n");
	auxiliarCrearPrograma();
}

void crearPrograma3 () {
	pasarScript("#!/usr/bin/ansisop \nbegin \n		variables f, i, t \n		#`f`: Hasta donde contar \n			f=20 \n			i=0 \n			:inicio \n			#`i`: Iterador \n			i=i+1			#Imprimir el contador \n			print i \n			#`t`: Comparador entre `i` y `f` \n			t=f-i \n			#De no ser iguales, salta a inicio \n			jnz t inicio \n	end \n");
	auxiliarCrearPrograma();
}

void crearPrograma4 () {
	pasarScript("#!/usr/bin/ansisop \nbegin \n		variables a, p \n			p = &a \n			a = 19 \n			print *p \n			print 30+*p \n			print *p+30 \n			p = &a+30 \n			print p \n			p = 30+&a \n			print p \n end \n");
	auxiliarCrearPrograma();
}
