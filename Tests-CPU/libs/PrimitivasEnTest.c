/*
 * PrimitivasEnTest.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "PrimitivasEnTest.h"
int esConRetorno = 0;

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

};
AnSISOP_kernel funciones_kernel = { };

void proximaInst() {

	t_intructions inst = indiceCodigo[pcb.program_counter];

	pcb.program_counter += 1;

	buscarEnSegmentoCodigo(inst);

}

void parsear(){

	proximaInst();

	analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);

}

/**************************************** DEFINIR VARIABLE ***************************************************/
t_puntero definirVariableTest(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;

	t_puntero posicion = calcularPosicionAsignacion(top_index);

	PUSH_POSITION (&id,pila,posicion);


	top_index = pila->top_index;

	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,posicion);
	dictionary_put(diccionario,elem->name,elem); //Elimino elementos junto con diccio despues

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

	//pcb.program_counter += 1;
}

/****************************************** DESREFERENCIAR ********************************************************/
t_valor_variable dereferenciar(t_puntero direccion_variable) {

	t_valor_variable valor_variable = POP_SIZE_CHECK_POSITION(pila, direccion_variable+1);

	return valor_variable;

}

/***************************************** IR AL LABEL ************************************************************/

void irAlLabel(t_nombre_etiqueta etiqueta) {
	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,indiceEtiquetas,pcb.tamanio_indice);

	pcb.program_counter = instruccion;

	t_intructions inst = indiceCodigo[instruccion];
	//RESERVAR CONTEXTO O ALGO?
	buscarEnSegmentoCodigo(inst);

	//Meto eso en analizador_de_linea... para invocar al parser
	//analizadorLinea(string,funciones_parser, funciones_kernel);
}

/*************************************** LLAMAR SIN RETORNO *******************************************************/
void llamarSinRetorno(t_nombre_etiqueta etiqueta) {

	reservarContextoSinRetorno();

	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear =  top_index +1;
	*pcb.c_stack = posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,indiceEtiquetas,pcb.tamanio_indice);
	pcb.program_counter = instruccion;

	//Busco en indice de codigo qué le pido a UMV
	t_intructions inst = indiceCodigo[instruccion];

	buscarEnSegmentoCodigo(inst);

	pcb.tamanio_contexto = 0;

	esConRetorno = 0;

	//Meto eso en analizador_de_linea... para invocar al parser
	//analizadorLinea(string,funciones_parser, funciones_kernel);

}

/******************************************* LLAMAR CON RETORNO **************************************************/
void llamarConRetornoTest(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	reservarContextoConRetorno(donde_retornar);
	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear = top_index +1;
	//*pcb.c_stack = posicionAPushear;
	*pcb.c_stack = posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,indiceEtiquetas,pcb.tamanio_indice);
	pcb.program_counter = instruccion;

	//Busco en indice de codigo qué le pido a UMV
	t_intructions inst = indiceCodigo[instruccion];

	buscarEnSegmentoCodigo(inst);

	pcb.tamanio_contexto = 0;

	//Meto eso en analizador_de_linea... para invocar al parser
	//analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);
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

/***********************************************************************************************************************/
/************************************************ INTEGRACION **********************************************************/
/***********************************************************************************************************************/


//CREO QUE EN EL FOR VA LA CANTIDAD DE INTRUCCIONES DE LA FUNCION PRINCIPAL

void integracionScriptFacil() {
	int i;
	for(i = 0; i<4;i++) {
		parsear();
	}

}

void integracionConFuncionDoble() {
	int i;
	for(i = 0; i<7;i++) {
			parsear();
	}

}
