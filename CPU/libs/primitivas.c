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
int cursor;
int stack;


t_puntero definirVariable(t_nombre_variable identificador_variable) {
	t_valor_variable id = identificador_variable;

	t_puntero posicion = calcularPosicionAsignacion(top_index);

	t_struct_push* estructura = malloc(sizeof(t_struct_push));
	estructura->posicion=posicion;
	estructura->valor = id;
	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
	free(estructura);

	chequearSiHuboSF();

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

	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion=direccion_variable +1;
	socket_enviar(sockUMV, D_STRUCT_POP, estructura);
	free(estructura);

	chequearSiHuboSF();

	t_valor_variable valor_variable;

	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_numero* k = ((t_struct_numero*)structRecibida);
		valor_variable= k->numero;
		free(k);
	}

	chequearSiHuboSF();

	log_escribir(archLog, "Ejecucion", INFO, "Se desreferencio la direccion de variable %d",direccion_variable);

	return valor_variable;
}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {

	int top = top_index;

	t_struct_push* estructura = malloc(sizeof(t_struct_push));
	estructura->posicion=direccion_variable;
	estructura->valor = valor;
	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
	free(estructura);

	chequearSiHuboSF();

	int posibleTop = direccion_variable + 1;

	if(top < posibleTop) {
		top_index = posibleTop;
	} else {
		top_index = top;
	}

	log_escribir(archLog, "Ejecucion", INFO, "Se asigno valor %d a direccion de variable %d",valor, direccion_variable);

}


t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {

	t_struct_string* estructura = malloc(sizeof(t_struct_string));
	estructura->string = variable;
	socket_enviar(sockKernel, D_STRUCT_OBTENERCOMPARTIDA, estructura);
	free(estructura);

	t_valor_variable valor;

	t_tipoEstructura tipoRecibido;
	void* structRecibida;
	int j=socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
	if(j==1){
		t_struct_numero* k = ((t_struct_numero*)structRecibida);
		valor= k->numero;
		free(k);
	}


	log_escribir(archLog, "Ejecucion", INFO, "Se obtuvo valo de variable compartida %s",variable);

	return valor;

}


t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {

	//Socket enviando Kernel para que asignNe el "valor" a "variable"
	socket_and_asignar_compartida(sockKernel,variable,valor);
	t_struct_asignar_compartida* estructura = malloc(sizeof(t_struct_asignar_compartida));
	estructura->nombre = variable;
	estructura->valor = valor;
	socket_enviar(sockKernel, D_STRUCT_ASIGNARCOMPARTIDA, estructura);
	free(estructura);

	log_escribir(archLog, "Ejecucion", INFO, "Se asigno valor %d a variable compartida %s",valor, variable);

	return valor;
}

void irAlLabel(t_nombre_etiqueta etiqueta) {

	t_puntero_instruccion instruccion = irAIntruccionLabel(etiqueta);

	t_intructions inst = instruccionParaBuscarEnIndiceCodigo(instruccion);

	socket_and_instruccion(sockUMV,inst);
	t_struct_instruccion* estructura = malloc(sizeof(t_struct_instruccion));
	estructura->inst = inst;
	socket_enviar(sockUMV, D_STRUCT_INSTRUCCION, estructura);
	free(estructura);

	chequearSiHuboSF();

	recibirProximaInstruccion(sockUMV);

	log_escribir(archLog, "Ejecucion", INFO, "Se va al label %s y se obtiene el numero de su primera instruccion ejecutable",etiqueta);
}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {

	reservarContextoSinRetorno();

	int posicionAPushear =  top_index +1;
	//*pcb->c_stack = posicionAPushear;
	cursor = posicionAPushear;

	irAlLabel(etiqueta);

	pcb->tamanio_contexto = 0;

	esConRetorno = 0;

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {

	reservarContextoConRetorno(donde_retornar);

	int posicionAPushear = top_index +1;
	//*pcb->c_stack = posicionAPushear;
	cursor = posicionAPushear;

	irAlLabel(etiqueta);

	pcb->tamanio_contexto = 0;
}


void finalizar() {
	t_puntero c_stack_viejo;

	if(!esPrimerContexto()) {

	recuperarPosicionDeDirecciones();
	volverAContextoAnterior(&c_stack_viejo);

	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);

	//*pcb->c_stack = c_stack_viejo;
	cursor = c_stack_viejo;

	regenerarDiccionario(tamanio);
	}

	if(esPrimerContexto()) {
		//Hay que hacer funcion para empezar la limpieza para terminar con el programa en ejecucion
		printf("\n\nllegamos al if!\n\n");
		termino = DONE;
		pcb->c_stack += cursor;
		pcb->stack += stack;

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

	//*pcb->c_stack = c_stack_viejo;
	cursor = c_stack_viejo;

	regenerarDiccionario(tamanio);

	t_puntero posicionAsignacion = direccionRetorno + 1;

	t_struct_push* estructura = malloc(sizeof(t_struct_push));
	estructura->posicion=posicionAsignacion;
	estructura->valor = retorno;
	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
	free(estructura);

	chequearSiHuboSF();

	esConRetorno = 0;

	log_escribir(archLog, "Ejecucion", INFO, "Se retorna el valor %d",retorno);

}

void imprimir(t_valor_variable valor_mostrar) {
	//Envía valor_mostrar al Kernnel, para que termine siendo mostrado en la consola del Programa en ejecución.

	socket_and_number(sockKernel, valor_mostrar);
	t_struct_numero* estructura = malloc(sizeof(t_struct_numero));
	estructura->numero = valor_mostrar;
	socket_enviar(sockKernel, D_STRUCT_NUMERO, estructura);
	free(estructura);


	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %d para imprimirlo por pantalla",valor_mostrar);

}

void imprimirTexto(char* texto) {
	//Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.

	t_struct_string* estructura = malloc(sizeof(t_struct_string));
	estructura->string = texto;
	socket_enviar(sockKernel, D_STRUCT_STRING, estructura);
	free(estructura);


	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %s para imprimirlo por pantalla",texto);

}

void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {

	t_struct_io* estructura = malloc(sizeof(t_struct_io));
	estructura->dispositivo = dispositivo;
	estructura->pid = pcb->pid;
	estructura->tiempo = tiempo;
	socket_enviar(sockKernel, D_STRUCT_IO, estructura);
	free(estructura);


	log_escribir(archLog, "Ejecucion", INFO, "Se envia conecto %s por %d tiempo",dispositivo,tiempo);

}

/****************************** OPERACIONES DE KERNEL ************************************************/

void wait_ansisop(t_nombre_semaforo identificador_semaforo) {

	t_struct_semaforo* estructura = malloc(sizeof(t_struct_semaforo));
	estructura->nombre_semaforo = identificador_semaforo;
	socket_enviar(sockKernel, D_STRUCT_WAIT, estructura);
	free(estructura);

	//ACA AGREGO UN SOCKET DONDE ME LLEGA EL VALOR DEL SEMAFORO
	controlarBloqueo(sockKernel, termino);

	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (wait)",identificador_semaforo);

}

void signal_ansisop(t_nombre_semaforo identificador_semaforo) {

	t_struct_semaforo* estructura = malloc(sizeof(t_struct_semaforo));
	estructura->nombre_semaforo = identificador_semaforo;
	socket_enviar(sockKernel, D_STRUCT_SIGNALSEMAFORO, estructura);
	free(estructura);

	controlarBloqueo(sockKernel, termino);

	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (signal)",identificador_semaforo);
}
