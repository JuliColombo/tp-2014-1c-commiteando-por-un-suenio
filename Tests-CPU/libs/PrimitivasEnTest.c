/*
 * PrimitivasEnTest.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "PrimitivasEnTest.h"

#define DONE 5;

int esConRetorno = 0;
int termino;

AnSISOP_funciones funciones_parser = {
			.AnSISOP_definirVariable		= definirVariableTest,
			.AnSISOP_obtenerPosicionVariable= obtenerPosicionVariableTest,
			.AnSISOP_dereferenciar			= dereferenciar,
			.AnSISOP_asignar				= asignarTest,
			.AnSISOP_finalizar				= finalizar,
			.AnSISOP_irAlLabel				= irAlLabel,
			.AnSISOP_llamarSinRetorno		= llamarSinRetorno,
			.AnSISOP_llamarConRetorno		= llamarConRetornoTest,
			.AnSISOP_retornar				= retornar,
			.AnSISOP_imprimir				= imprimir,

};
AnSISOP_kernel funciones_kernel = { };

void proximaInst() {

	t_intructions inst = indiceCodigo[pcb.program_counter];

	buscarEnSegmentoCodigo(inst);

}

void parsear(){

	proximaInst();

	pcb.program_counter += 1;

	analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);

}

/**************************************** DEFINIR VARIABLE ***************************************************/
t_puntero definirVariableTest(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;

	t_puntero posicion = calcularPosicionAsignacion(top_index);

	PUSH_POSITION (&id,pila,posicion);

	top_index = posicion;

	insertarEnDiccionario(identificador_variable, posicion); //Elimino elementos junto con diccio despues

	pcb.tamanio_contexto += 1;

	return posicion;
}

/********************************************* OBTENER POSICION **************************************************/
t_puntero obtenerPosicionVariableTest(t_nombre_variable identificador_variable) {
	t_puntero posicion = 0;
	char* str = convertirAString(identificador_variable);
	if(dictionary_has_key(diccionario,str)) {
		t_elemento* elemento = dictionary_get(diccionario,str);
		posicion = elemento->pos;
	} else {
		posicion = -1;
	}

	return posicion;

}

/******************************************* ASIGNAR *************************************************************/
void asignarTest(t_puntero direccion_variable, t_valor_variable valor) {
	int top = top_index;

	PUSH_POSITION (&valor,pila,direccion_variable+1);

	int posibleTop = direccion_variable + 1;

	if(top < posibleTop){
	top_index = posibleTop;} else {
		top_index = top;
		pila->top_index = top;
	}

}

/****************************************** DESREFERENCIAR ********************************************************/
t_valor_variable dereferenciar(t_puntero direccion_variable) {

	t_valor_variable valor_variable = POP_SIZE_CHECK_POSITION(pila, direccion_variable+1);

	return valor_variable;

}

/***************************************** IR AL LABEL ************************************************************/

void irAlLabel(t_nombre_etiqueta etiqueta) {
	t_puntero_instruccion instruccion = irAIntruccionLabel(etiqueta);

	t_intructions inst = indiceCodigo[instruccion];

	buscarEnSegmentoCodigo(inst);

}

/*************************************** LLAMAR SIN RETORNO *******************************************************/
void llamarSinRetorno(t_nombre_etiqueta etiqueta) {

	reservarContextoSinRetorno();

	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear =  top_index +1;
	*pcb.c_stack = posicionAPushear;

	t_puntero_instruccion instruccion = irAIntruccionLabel(etiqueta);

	//Busco en indice de codigo qué le pido a UMV
	t_intructions inst = indiceCodigo[instruccion];

	buscarEnSegmentoCodigo(inst);

	pcb.tamanio_contexto = 0;

	esConRetorno = 0;

}

/******************************************* LLAMAR CON RETORNO **************************************************/
void llamarConRetornoTest(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	reservarContextoConRetorno(donde_retornar);
	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear = top_index +1;
	//*pcb.c_stack = posicionAPushear;
	*pcb.c_stack = posicionAPushear;

	t_puntero_instruccion instruccion = irAIntruccionLabel(etiqueta);

	//Busco en indice de codigo qué le pido a UMV
	t_intructions inst = indiceCodigo[instruccion];

	buscarEnSegmentoCodigo(inst);

	pcb.tamanio_contexto = 0;
}

/**************************************************** FINALIZAR *******************************************************/
void finalizar() {

	printf("FINALIZAR PIOLA\n");

	t_puntero c_stack_viejo;

	if(!esPrimerContexto()) {

	recuperarPosicionDeDirecciones();
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);

	*pcb.c_stack = c_stack_viejo;

	regenerarDiccionario(tamanio);
	}

	if(esPrimerContexto()) {
		//Hay que hacer funcion para empezar la limpieza para terminar con el programa en ejecucion
		printf("\n\nllegamos al if!\n\n");
		termino = DONE;

	}

}

/************************************************** RETORNAR **********************************************************/

void retornar(t_valor_variable retorno) {
	printf("\nRETORNAR PIOLA\n");

	recuperarPosicionDeDirecciones();

	t_puntero direccionRetorno;
	recuperarDireccionRetorno(&direccionRetorno);

	t_puntero c_stack_viejo;
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);

	*pcb.c_stack = c_stack_viejo;

	regenerarDiccionario(tamanio);

	t_puntero posicionAsignacion = direccionRetorno + 1;

	//socket_and_push(sockUMV,direccionRetorno+1,retorno);
	PUSH_POSITION (&retorno,pila,posicionAsignacion);

	esConRetorno = 0;
}

/*********************************************** PRINT DE MENTIRITAS ***************************************************/
void imprimir(int valor) {
	printf("imprimo %d\n",valor);
}

/***********************************************************************************************************************/
/************************************************ INTEGRACION **********************************************************/
/***********************************************************************************************************************/

//IDEA PARA MANDAR A CORRER EL PARSER EN LA CPU. LLEGARIA SCRIPT POR PARAMETRO.
void integracionCorrerParser() {
	while(1){
		termino = 0;
		parsear();
		if(termino == 5) {
			break;
		}
	}
}

void integracionScriptFacil() {
	while(1){
		parsear();
		if(pila->elementos[1] == 17) {
			break;
		}
	}

}

void integracionConFuncionDoble() {
	while(1){
		parsear();
		if(pila->elementos[3] == 40) {
			break;
		}
	}
}

void integracionFor(){
	while(1){
		parsear();
		if((pila->elementos[3] == 20) && (pila->elementos[5]==0)) {
			break;
		}
	}
}

