/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "primitivasAux.h"
#include "Sockets/socket.h"
#include "primitivas.h"
#include <commons/collections/dictionary.h>
#include <parser/metadata_program.h>

t_dictionary *diccionario;
int sockfd;
int sockAjeno;
int top_index;

char* etiquetas;
t_intructions* codigoo;



t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;
	t_puntero posicion = calcularPosicionAsignacionCPU(top_index);

	//Socket enviando posicion e id para que la UMV pushee
	//En UMV: PUSH_SIZE_CHECK(&id,pila,posicion);
	socket_and_push(sockAjeno,posicion,id);
	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas como asignar


	const char* str=convertirAString(identificador_variable);
	t_elemento* elem = elemento_create(str,posicion);
	dictionary_put(diccionario,elem->name,elem); //Elimino elementos junto con diccio despues

	pcb.program_counter +=1;
	pcb.tamanio_contexto += 1;

	return posicion;
}


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


t_valor_variable dereferenciar(t_puntero direccion_variable) {
	//Socket enviando direccion_variable a UMV para que haga pop
	//En UMV POP_DESREFERENCIAR(pila, direccion_variable)
	socket_and_pop_position(sockAjeno,direccion_variable+1); //sumo uno porque era desreferenciar

	//Socket recibiendo t_valor_variable id
	t_estructura tipo;
	void** estructura2;
	socket_recibir(sockAjeno,&tipo, estructura2);
	struct_char** estructuraAux = (struct_char**)estructura2;
	t_valor_variable id = (*estructuraAux)->letra;
	free(estructura2);
	free(estructuraAux);
	return id;

}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {
	//top_index que se actualiza cada vez que UMV pushea o popea

	//Socket enviando direccion_variable y valor a UMV
	//En UMV PUSH_SIZE_CHECK(&valor,pila,direccion_variable);
	socket_and_push(sockAjeno,direccion_variable,valor);

	if(top_index < direccion_variable) {
		top_index = direccion_variable;
	}

	//Le digo a UMV que actualice su top_index del stack
	//Socket enviando top_index para que UMV haga: pila->top_index = top_index;
	struct_numero* estructura = crear_struct_numero(top_index);
	socket_enviar(sockAjeno,STRUCT_NUMERO,estructura);
	free(estructura);

	pcb.program_counter += 1;
}


t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {
	//Socket mandando a Kernel el nombre de la variable de la que quiero saber el valor
	struct_string* estructura = crear_struct_string(variable);
	socket_enviar(sockAjeno,STRUCT_OBTENER_COMPARTIDA,estructura);
	free(estructura);

	//Socket recibiendo lacopia (no puntero) del valor de la "variable"
	t_estructura tipo = STRUCT_NUMERO;
	void** estructura2;
	socket_recibir(sockAjeno,&tipo, estructura2);
	struct_numero** estructuraAux = (struct_numero**)estructura2;
	t_valor_variable valor = (*estructuraAux)->numero;
	free(estructuraAux);
	free(estructura2);

	return valor;

}


t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {

	//Socket enviando Kernel para que asignNe el "valor" a "variable"
	struct_asignar_compartida* estructura = crear_struct_asignar_compartida(variable, valor);
	socket_enviar(sockAjeno, STRUCT_ASIGNAR_COMPARTIDA,estructura);
	free(estructura);

	pcb.program_counter += 1;
	return valor;
}


void irAlLabel(t_nombre_etiqueta etiqueta) {
	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,pcb.tamanio_indice);

	pcb.program_counter = instruccion;

	//Busco en indice de codigo qué le pido a UMV -----> creo que esto es asi

	//Socket enviando a UMV el start y offset para que me pase la instruccion a ejecutar

	//Socket recibiendo la instruccion a ejecutar de UMV
	//Meto eso en analizador_de_linea... para invocar al parser
}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {
	//HACER ALGO CON TAMAÑO DE CONTEXTO

	reservarContextoSinRetorno();

	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear =  top_index +1;
	pcb.c_stack = posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,pcb.tamanio_indice);
	pcb.program_counter = instruccion;

	//Busco en indice de codigo qué le pido a UMV
	t_intructions inst = codigoo[instruccion];

	//Socket enviando a UMV el start y offset para que me pase la instruccion a ejecutar
	struct_tipo_instruccion* estructura = crear_struct_tipo_instruccion(inst);
	socket_enviar(sockAjeno, STRUCT_TIPO_INSTRUCCION,estructura);
	free(estructura);

	//Socket recibiendo la instruccion a ejecutar de UMV
	t_estructura tipo = STRUCT_STRING;
	void** estructura2;
	socket_recibir(sockAjeno,&tipo, estructura2);
	struct_string** estructuraAux = (struct_string**)estructura2;
	const char* string = (*estructuraAux)->string;
	free(estructuraAux);
	free(estructura2);

	//Meto eso en analizador_de_linea... para invocar al parser
	//void analizadorLinea(string, AnSISOP_funciones* AnSISOP_funciones, AnSISOP_kernel* AnSISOP_funciones_kernel);

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	//definir variables si hay parametros
	//Asignar a parametros
	//HACER ALGO CON TAMAÑO DE CONTEXTO

	reservarContextoConRetorno();
	//Socket recibiendo top_index de pila para actualizar el mio y poder llevar a cabo otras primitivas

	int posicionAPushear = top_index +1;
	pcb.c_stack = posicionAPushear;

	t_puntero_instruccion instruccion;
	instruccion = metadata_buscar_etiqueta(etiqueta,etiquetas,pcb.tamanio_indice);
	pcb.program_counter = instruccion;

	//Busco en indice de codigo qué le pido a UMV
	t_intructions inst = codigoo[instruccion];

	//Socket enviando a UMV el start y offset para que me pase la instruccion a ejecutar
	struct_tipo_instruccion* estructura = crear_struct_tipo_instruccion(inst);
	socket_enviar(sockAjeno, STRUCT_TIPO_INSTRUCCION,estructura);
	free(estructura);

	//Socket recibiendo la instruccion a ejecutar de UMV
	t_estructura tipo = STRUCT_STRING;
	void** estructura2;
	socket_recibir(sockAjeno,&tipo, estructura2);
	struct_string** estructuraAux = (struct_string**)estructura2;
	const char* string = (*estructuraAux)->string;
	free(estructuraAux);
	free(estructura2);

	//Meto eso en analizador_de_linea... para invocar al parser
	//void analizadorLinea(string, AnSISOP_funciones* AnSISOP_funciones, AnSISOP_kernel* AnSISOP_funciones_kernel);


}

void finalizar() {
	t_puntero c_stack = pcb.c_stack;
	t_puntero stack = pcb.stack;

	recuperarPosicionDeDirecciones();
	volverAContextoAnterior();

	int tamanio = calcularTamanioContextoAnterior(c_stack);

	regenerarDiccionario(tamanio);

	if(c_stack == stack) {
		//Hay que hacer funcion para empezar la limpieza para terminar con el programa en ejecucion
	}
}


void retornar(t_valor_variable retorno) {

	recuperarPosicionDeDirecciones();
	t_puntero direccionRetorno = recuperarDireccionRetorno();

	volverAContextoAnterior();
	regenerarDiccionario(pcb.tamanio_contexto);

	socket_and_push(sockAjeno,direccionRetorno+1,retorno);

}

int imprimir(t_valor_variable valor_mostrar) {
	//Envía valor_mostrar al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución.

	//Socket a Kernel enviandole el valor a mostrar
	struct_numero* estructura = crear_struct_numero(valor_mostrar);
	socket_enviar(sockAjeno,STRUCT_NUMERO,estructura);
	free(estructura);

	return 0;
}

int imprimirTexto(char* texto) {
	//Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.

	//Socket a Kernel enviandole texto
	struct_string* estructura = crear_struct_string(texto);
	socket_enviar(sockAjeno,STRUCT_STRING,estructura);
	free(estructura);

	return 0;
}

int entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
	//Informa al Kernel que el Programa actual pretende usar el dispositivo tiempo unidades de tiempo.

	//Socket con mensaje?
	return 0;
}


/****************************** OPERACIONES DE KERNEL ************************************************/

int wait(t_nombre_semaforo identificador_semaforo) {
	//Informa al kernel que ejecute la función wait para el semáforo con el nombre identificador_semaforo.
	//El kernel deberá decidir si bloquearlo o no.

	struct_semaforo* estructura = crear_struct_semaforo(identificador_semaforo,WAIT);
	socket_enviar(sockAjeno,STRUCT_SEMAFORO,estructura);
	free(estructura);

	return 0;
}

int signal(t_nombre_semaforo identificador_semaforo) {
	//Informa al kernel que ejecute la función signal para el semáforo con el nombre identificador_semaforo.
	//El kernel deberá decidir si desbloquear otros procesos o no.

	struct_semaforo* estructura = crear_struct_semaforo(identificador_semaforo,SIGNAL);
	socket_enviar(sockAjeno,STRUCT_SEMAFORO,estructura);
	free(estructura);

	return 0;
}
