/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "primitivas.h"

t_dictionary *diccionario;
int sockfd;
int sockUMV;
int sockKernel;
int top_index;

char* indiceEtiquetas;
t_intructions* indiceCodigo;

char* proximaInstruccion;

extern int termino; //tiene que ser extern??
int esConRetorno = 0;


t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;

	t_puntero posicion = calcularPosicionAsignacion(top_index);

	socket_and_push(sockUMV,posicion,id);

	top_index = posicion;

	insertarEnDiccionario(identificador_variable, posicion); //Elimino elementos junto con diccio despues

	pcb->tamanio_contexto += 1;

	log_escribir(archLog, "Ejecucion", INFO, "Se definio variable %c",identificador_variable);

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

	log_escribir(archLog, "Ejecucion", INFO, "Se obtuvo posicion de variable %c",identificador_variable);

	return posicion;
}


t_valor_variable dereferenciar(t_puntero direccion_variable) {
	socket_and_pop_position(sockUMV,direccion_variable + 1);

	//Socket recibiendoo t_valor_variable id
	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	t_valor_variable valor_variable = estructura->numero;

	log_escribir(archLog, "Ejecucion", INFO, "Se desreferencio la direccion de variable %d",direccion_variable);

	return valor_variable;
}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {

	int top = top_index;

	socket_and_push(sockUMV,direccion_variable,valor);

	int posibleTop = direccion_variable + 1;

	if(top < posibleTop) {
		top_index = posibleTop;
	} else {
		top_index = top;
	}

	log_escribir(archLog, "Ejecucion", INFO, "Se asigno valor %d a direccion de variable %d",valor, direccion_variable);

}


t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {

	socket_and_obtener_compartida(sockKernel, variable);

	//Socket recibiendo lacopia (no puntero) del valor de la "variable"

	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	t_valor_variable valor = estructura->numero;

	log_escribir(archLog, "Ejecucion", INFO, "Se obtuvo valo de variable compartida %s",variable);

	return valor;

}


t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {

	//Socket enviando Kernel para que asignNe el "valor" a "variable"
	socket_and_asignar_compartida(sockKernel,variable,valor);

	log_escribir(archLog, "Ejecucion", INFO, "Se asigno valor %d a variable compartida %s",valor, variable);

	return valor;
}

void irAlLabel(t_nombre_etiqueta etiqueta) {

	t_puntero_instruccion instruccion = irAIntruccionLabel(etiqueta);

	t_intructions inst = instruccionParaBuscarEnIndiceCodigo(instruccion);

	socket_and_instruccion(sockUMV,inst);

	recibirProximaInstruccion(sockUMV);

	log_escribir(archLog, "Ejecucion", INFO, "Se va al label %s y se obtiene el numero de su primera instruccion ejecutable",etiqueta);
}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {

	reservarContextoSinRetorno();

	int posicionAPushear =  top_index +1;
	*pcb->c_stack = posicionAPushear;

	irAlLabel(etiqueta);

	pcb->tamanio_contexto = 0;

	esConRetorno = 0;

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	reservarContextoConRetorno(donde_retornar);

	int posicionAPushear = top_index +1;
	*pcb->c_stack = posicionAPushear;

	irAlLabel(etiqueta);

	pcb->tamanio_contexto = 0;
}


void finalizar() {
	t_puntero c_stack_viejo;

	if(!esPrimerContexto()) {

	recuperarPosicionDeDirecciones();
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);

	*pcb->c_stack = c_stack_viejo;

	regenerarDiccionario(tamanio);
	}

	if(esPrimerContexto()) {
		//Hay que hacer funcion para empezar la limpieza para terminar con el programa en ejecucion
		printf("\n\nllegamos al if!\n\n");
		termino = DONE;

		log_escribir(archLog, "Ejecucion", INFO, "Finalizo ejecucion");

	}
}


void retornar(t_valor_variable retorno) {

	recuperarPosicionDeDirecciones();

	t_puntero direccionRetorno;
	recuperarDireccionRetorno(&direccionRetorno);

	t_puntero c_stack_viejo;
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);

	*pcb->c_stack = c_stack_viejo;

	regenerarDiccionario(tamanio);

	t_puntero posicionAsignacion = direccionRetorno + 1;

	socket_and_push(sockUMV,posicionAsignacion,retorno);

	esConRetorno = 0;

	log_escribir(archLog, "Ejecucion", INFO, "Se retorna el valor %d",retorno);

}

void imprimir(t_valor_variable valor_mostrar) {
	//Envía valor_mostrar al Kernnel, para que termine siendo mostrado en la consola del Programa en ejecución.

	socket_and_number(sockKernel, valor_mostrar);

	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %d para imprimirlo por pantalla",valor_mostrar);

}

void imprimirTexto(char* texto) {
	//Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.

	socket_and_string(sockKernel,texto);

	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %s para imprimirlo por pantalla",texto);

}

void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
	//Informa al Kernel que el Programa actual pretende usar el dispositivo tiempo unidades de tiempo.
	socket_and_io(sockKernel,dispositivo,tiempo);

	log_escribir(archLog, "Ejecucion", INFO, "Se envia conecto %s por %d tiempo",dispositivo,tiempo);

}


/****************************** OPERACIONES DE KERNEL ************************************************/

void wait_ansisop(t_nombre_semaforo identificador_semaforo) {

	socket_and_wait(sockKernel,identificador_semaforo);

	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (wait)",identificador_semaforo);

}

void signal_ansisop(t_nombre_semaforo identificador_semaforo) {

	socket_and_signal_semaforo(sockKernel,identificador_semaforo);

	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (signal)",identificador_semaforo);
}
