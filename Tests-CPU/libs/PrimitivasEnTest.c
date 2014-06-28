/*
 * PrimitivasEnTest.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "PrimitivasEnTest.h"
int esConRetorno = 0;

AnSISOP_funciones funciones_parser = {
			.AnSISOP_definirVariable		= definirVariable,
			.AnSISOP_obtenerPosicionVariable= obtenerPosicionVariable,
			.AnSISOP_dereferenciar			= dereferenciar,
			.AnSISOP_asignar				= asignar,
			.AnSISOP_finalizar				= finalizar,
			.AnSISOP_irAlLabel				= irAlLabel,
			.AnSISOP_llamarSinRetorno		= llamarSinRetorno,
			.AnSISOP_llamarConRetorno		= llamarConRetorno,
			.AnSISOP_retornar				= retornar,

};
AnSISOP_kernel funciones_kernel = { };

/**************************************** DEFINIR VARIABLE ***************************************************/
t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;

	t_puntero posicion = calcularPosicionAsignacion(top_index);

	PUSH_POSITION (&id,pila,posicion);


	top_index = pila->top_index;

	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,posicion);
	dictionary_put(diccionario,elem->name,elem); //Elimino elementos junto con diccio despues


	pcb.program_counter +=1;
	pcb.tamanio_contexto += 1;

	printf("\ndefinir variable %c\n",identificador_variable);

	return posicion;
}

/********************************************* OBTENER POSICION **************************************************/
t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {
	t_puntero posicion = 0;
	char* str = convertirAString(identificador_variable);
	if(dictionary_has_key(diccionario,str)) {
		t_elemento* elemento = dictionary_get(diccionario,str);
		posicion = elemento->pos;
	} else {
		posicion = -1;
	}

	printf("\n\nobtener posicion de %c\n\n",identificador_variable);

	return posicion;

}


/******************************************* ASIGNAR *************************************************************/
void asignar(t_puntero direccion_variable, t_valor_variable valor) {
	int top = top_index;

	PUSH_POSITION (&valor,pila,direccion_variable+1);

	int posibleTop = direccion_variable + 1;

	if(top < posibleTop){
	top_index = posibleTop;} else {
		top_index = top;
		pila->top_index = top;
	}

	pcb.program_counter += 1;

	printf("\n\nasignar %d a %d\n\n",valor,direccion_variable);
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
void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

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
	//analizadorLinea(string,funciones_parser, funciones_kernel);
}

/**************************************************** FINALIZAR *******************************************************/
void finalizar() {

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
	recuperarPosicionDeDirecciones();

	t_puntero direccionRetorno;
	recuperarDireccionRetorno(&direccionRetorno);

	t_puntero c_stack_viejo;
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);

	*pcb.c_stack = c_stack_viejo;

	regenerarDiccionario(tamanio);

	//socket_and_push(sockUMV,direccionRetorno+1,retorno);
	PUSH_POSITION (&retorno,pila,direccionRetorno + 1);

	esConRetorno = 0;
}

/***********************************************************************************************************************/
/************************************************ INTEGRACION **********************************************************/
/***********************************************************************************************************************/

void parsear(){
	t_intructions inst = indiceCodigo[pcb.program_counter];

	buscarEnSegmentoCodigo(inst);

	printf("la proxima instruccion es: %s\n", proximaInstruccion);

	analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);

}

void integracion() {
	parsear();
	parsear();
	parsear();
	parsear();

}
