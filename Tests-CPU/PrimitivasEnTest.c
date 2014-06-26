/*
 * PrimitivasEnTest.c
 *
 *  Created on: 20/06/2014
 *      Author: utnso
 */

#include "PrimitivasEnTest.h"
int esConRetorno = 0;

/**************************************** FUNCIONES AUXILIARES ***************************************************/
/************** SOLO PARA TEST **********************/
char* buscarEnSegmentoCodigo(t_intructions instruccion) {
	memcpy(proximaInstruccion,&segmentoCodigo[instruccion.start],instruccion.offset);
	return proximaInstruccion;
}
/****************************************************/

t_puntero calcularPosicionAsignacionCPU(int top_index) {
	t_puntero posicion=0;
	if(top_index == -1) {
		posicion = top_index +1;
	} else {
		posicion = top_index + 2;
	}
	return posicion;}


int esPrimerContexto() {
	int a = (*pcb.c_stack == *pcb.stack );
	return (a);
}

int esPar(int numero){
	return (numero % 2 == 0);
}

t_puntero calcularPosicionAsignacion(int top_index) {
	t_puntero posicion;
	if(esConRetorno) {
		if(esPar(top_index)) {
				posicion = top_index + 1;
			} else {
				posicion = calcularPosicionAsignacionCPU(top_index);
			}
	} else {
		if(esPar(top_index)) {
				posicion = calcularPosicionAsignacionCPU(top_index);
			} else {
				posicion = top_index +1;
			}
	}
	return posicion;
}

char* convertirAString(t_nombre_variable c) {
	static char str[2];
	str[0]= c;
	str[1]= '\0';
	return str;
}

t_elemento* elemento_create(const char* name, t_puntero pos){
	t_elemento *new = malloc(sizeof(t_elemento));
	new->name = strdup(name);
	new->pos = pos;
	return new;
}

void elemento_delete(t_elemento* elemento) {
	free(elemento->name);
	free(elemento);
}

/******************** RECUPERAR CONTEXTO ***********************/

void recuperarPosicionDeDirecciones() {
	//socket_and_modificar_top_index(sockUMV,*(pcb.c_stack)-1);
	pila->top_index = *pcb.c_stack -1;

	//Esto va
	top_index = *pcb.c_stack -1;
}

void recuperarProgramCounter(t_puntero* program_counter) {
	//Socket a UMV para que haga: t_puntero program_counter = POP_POSITION(pila,top_index);
	//t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	//*program_counter = estructura->numero;

	*program_counter = pila->elementos[top_index];
	top_index -= 1;
}

void recuperarCursorAnterior(t_puntero* cursor_stack_viejo) {
	//Socket a UMV para que haga: t_puntero program_counter = POP(pila);
	//t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	//*cursor_stack_viejo = estructura->numero;

	*cursor_stack_viejo = pila->elementos[top_index];
	top_index -= 1;
}

void volverAContextoAnterior(t_puntero* c_stack_viejo) {
	t_puntero program_counter;

	recuperarProgramCounter(&program_counter);
	recuperarCursorAnterior(c_stack_viejo);

	//Socket de UMV para que yo actualice el top_index

	pcb.program_counter = program_counter;

	dictionary_clean_and_destroy_elements(diccionario,(void*)elemento_delete);

}

/*Lo que hago con esta funcion es:
 * estoy en la posicion de cursor contexto.
 * Pongo en el diccionario el id y su posicion.
 * Disminuyo el top_index en 2 para llegar al nombre de una variable (si restara uno, obtendria el valor) la proxima vez que la use.
 * Invoco esta funcion tantas veces como el tamaño del contexto sea.
 */

void guardarAlternado () {
	//Socket a UMV para que haga: pila->top_index = top_index;
	//socket_and_modificar_top_index(sockUMV,top_index);
	pila->top_index = top_index;
	//Socket a UMV para que haga TOP(pila)
	//socket_enviarSignal(sockUMV, TOP);

	//Socket de UMV para que me pase lo ultimo que hay pusheado en la pila, y yo hacer: t_nombre_variable identificador_variable = TOP(pila);
	//t_struct_char* estructura =(t_struct_char*)socket_recibir_estructura(sockUMV);
	//t_valor_variable identificador_variable = estructura->letra;

	t_valor_variable identificador_variable = pila->elementos[top_index];

	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,top_index);
	dictionary_put(diccionario,elem->name,elem);

	top_index -=2;

}

//Una vez que regenere el diccionario, pongo el top_index en la posicion del valor de la ultima variable, cosa que si se quieren definir nuevas con la funcion
//calcularPosicion, no tire error.
void regenerarDiccionario(int tamanio_contexto) {
	int i = 0;
	int top = top_index - 1;
	while (i < tamanio_contexto) {
		guardarAlternado();
		i++;
	}

	//Socket a UMV para que haga: pila->top_index = top;
	//socket_and_modificar_top_index(sockUMV,top);
	top_index = top;
	pila->top_index = top;

}

uint32_t calcularTamanioContextoAnterior(t_puntero direccion_contexto_actual) {
	uint32_t diferencia = direccion_contexto_actual - (*pcb.c_stack);
	printf("diferencia %d\n",diferencia/8);
	return (diferencia/8); //Divido esa cantidad de bytes por 5 (1 byte de id de variable, y 4 bytes del valor) para saber cuantas variables habia.
}


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
	t_puntero c_stack = *(pcb.c_stack);
	printf("el pcb.c_stack vale %d\n", *(pcb.c_stack));
	t_puntero stack = *(pcb.stack);
	printf("el pcb.stack vale %d\n", *(pcb.stack));
	t_puntero c_stack_viejo;

	recuperarPosicionDeDirecciones();
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack);

	regenerarDiccionario(tamanio);

	if(c_stack == stack) {
		//Hay que hacer funcion para empezar la limpieza para terminar con el programa en ejecucion
		printf("llegamos al if! \n");
	}
}
