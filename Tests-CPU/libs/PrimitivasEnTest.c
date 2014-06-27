/*
 * PrimitivasEnTest.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "PrimitivasEnTest.h"
int esConRetorno = 0;

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
void reservarContextoSinRetorno() {
	int cursor = *(pcb.c_stack);

	t_puntero posicionContextoViejo;
	posicionContextoViejo = calcularPosicionAsignacion(top_index);

	//Pushear cursor de stack
	PUSH_POSITION(&cursor,pila,posicionContextoViejo);
	top_index = posicionContextoViejo +1;

	//Pushear Program Counter de proxima instruccion:
	pcb.program_counter +=1;
	int pc = pcb.program_counter;

	PUSH_POSITION(&pc,pila,top_index);

	//Borrar diccionario y todos los elementos. Cuando lo regenero, los vuelvo a crear.
	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}


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
void reservarContextoConRetorno(t_puntero donde_retornar){

	int retornar = donde_retornar;
	reservarContextoSinRetorno();

	top_index += 1;

	//Socket a UMV para que haga: PUSH_SIZE_CHECK(&posicionVar,pila,posicionAVariable);
	PUSH_POSITION(&retornar,pila,top_index);

	esConRetorno = 1;
}


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

	recuperarPosicionDeDirecciones();
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);

	*pcb.c_stack = c_stack_viejo;

	regenerarDiccionario(tamanio);

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
