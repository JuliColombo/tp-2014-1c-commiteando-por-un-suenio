/*
 * primitivas.c
 *
 *  Created on: 06/05/2014
 *      Author: utnso
 */

#include "primitivas.h"




t_puntero definirVariable(t_nombre_variable identificador_variable) {
	if (SEG_flag == 1)
		return 0;
	printf("definirVariable\n");
	int recepcion;
	//Se arma la estructura de definir_variable con lo recibido
	t_struct_env_bytes * definir_variable = malloc(sizeof(t_struct_env_bytes));
	definir_variable->base = var_seg_stack;
	definir_variable->offset = temp_cursor_stack + 5 * var_tamanio_contexto;
	definir_variable->tamanio = sizeof(t_nombre_variable) + sizeof(t_valor_variable);
	//Se agrega la variable en el diccionario
	char key = identificador_variable;
	char keyABuscar[2] = { key, '\0' };

	definir_variable->buffer = &(keyABuscar[0]);
	key = *((char*) definir_variable->buffer);
	socket_enviar(sockUMV, D_STRUCT_ENV_BYTES, definir_variable);

	int* posicion_variable = malloc(sizeof(int));
	*posicion_variable = var_tamanio_contexto + 1;

	dictionary_put(dicc_variables, keyABuscar, posicion_variable);

	/*probamos si el valor que guardo en el diccionario de variables sea el correcto
	 int direccion;
	 void* posicion2=malloc(sizeof(int));
	 posicion2=dictionary_get(dicc_variables,&identificador_variable);
	 memcpy(&direccion,posicion2,sizeof(int));
	 printf("la variable definida esta en la posicion # :%d\n",direccion);
	 */

	free(definir_variable);
	//free(posicion_variable);
	void * structRecibido;
	t_tipoEstructura tipoStruct;

	socket_recibir(sockUMV, &tipoStruct, &structRecibido);

	if (tipoStruct != D_STRUCT_NUMERO) {
		printf("Respuesta en desreferenciar incorrecta\n");
		return 0;
	}
	recepcion = ((t_struct_numero*) structRecibido)->numero;
	excepcion_UMV(recepcion);
	var_tamanio_contexto++; //aumentamos el tamanio de contexto
	free(structRecibido);
	return obtenerPosicionVariable(keyABuscar[0]); //mejor que devuelva el desplazamiento dentro del stack y no la direccion por que el stack esta en otro proceso
}


t_puntero obtenerPosicionVariable(t_nombre_variable identificador_variable) {
	if (SEG_flag == 1)
			return 0;
		//Busca la variable en el diccionario
		printf("obtenerPosicionVariable\n");
		char key = identificador_variable;
		char keyABuscar[2] = { key, '\0' };

		int posicion_variable;
		//void* posicion=malloc(sizeof(int));
		int * posicion = dictionary_get(dicc_variables, keyABuscar);
	//	posicion_variable=(temp_cursor_stack + orden_var*5)-4; // aca sacamos el desplazamiento dentro del stack para el valor de una variable

		posicion_variable = ((*posicion) * 5) - 4;

		if (posicion_variable >= 0)
			return posicion_variable + temp_cursor_stack;
		else
			return -1;
}


t_valor_variable dereferenciar(t_puntero direccion_variable) {

//	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
//	estructura->posicion=direccion_variable +1;
//	estructura->stack_base = pcb->stack;
//	estructura->tamanio = sizeof(t_valor_variable);
//	socket_enviar(sockUMV, D_STRUCT_POP, estructura);
//	free(estructura);
//
//	chequearSiHuboSF();
//
//	t_valor_variable valor_variable;
//
//	t_tipoEstructura tipoRecibido;
//	void* structRecibida;
//	int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
//	if(j==1){
//		t_struct_numero* k = ((t_struct_numero*)structRecibida);
//		valor_variable= k->numero;
//		free(k);
//	}
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se desreferencio la direccion de variable %d",direccion_variable);
//
//	return valor_variable;
}


void asignar(t_puntero direccion_variable, t_valor_variable valor) {

//	//int top = top_index;
//
//	t_struct_push* estructura = malloc(sizeof(t_struct_push));
//	estructura->posicion=direccion_variable+1;
//	estructura->valor = valor;
//	estructura->stack_base = pcb->stack;
//	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
//	free(estructura);
//
//	chequearSiHuboSF();
//
//	/*int posibleTop = direccion_variable + 4;
//
//	if(top < posibleTop) {
//		top_index = posibleTop;
//	} else {
//		top_index = top;
//	}*/
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se asigno valor %d a direccion de variable %d",valor, direccion_variable);

}


t_valor_variable obtenerValorCompartida(t_nombre_compartida variable) {

//	t_struct_string* estructura = malloc(sizeof(t_struct_string));
//	estructura->string = variable;
//	socket_enviar(sockKernel, D_STRUCT_OBTENERCOMPARTIDA, estructura);
//	free(estructura);
//
//	t_valor_variable valor;
//
//	t_tipoEstructura tipoRecibido;
//	void* structRecibida;
//	int j=socket_recibir(sockKernel,&tipoRecibido,&structRecibida);
//	if(j==1){
//		t_struct_numero* k = ((t_struct_numero*)structRecibida);
//		valor= k->numero;
//		free(k);
//	}
//
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se obtuvo valo de variable compartida %s",variable);
//
//	return valor;

}


t_valor_variable asignarValorCompartida(t_nombre_compartida variable, t_valor_variable valor) {

//	//Socket enviando Kernel para que asignNe el "valor" a "variable"
//	t_struct_asignar_compartida* estructura = malloc(sizeof(t_struct_asignar_compartida));
//	estructura->nombre = variable;
//	estructura->valor = valor;
//	socket_enviar(sockKernel, D_STRUCT_ASIGNARCOMPARTIDA, estructura);
//	free(estructura);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se asigno valor %d a variable compartida %s",valor, variable);
//
//	return valor;
}

void irAlLabel(t_nombre_etiqueta etiqueta) {
//
//	t_puntero_instruccion instruccion = irAIntruccionLabel(etiqueta);
//
//	t_intructions inst = instruccionParaBuscarEnIndiceCodigo(instruccion);
//
//	t_struct_seg_codigo* estructura = malloc(sizeof(t_struct_seg_codigo));
//	estructura->inst = inst;
//	estructura->seg_codigo = pcb->codigo;
//	socket_enviar(sockUMV, D_STRUCT_SEGCODIGO, estructura);
//	free(estructura);
//
//	chequearSiHuboSF();
//
//	recibirProximaInstruccion(sockUMV);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se va al label %s y se obtiene el numero de su primera instruccion ejecutable",etiqueta);
}


void llamarSinRetorno(t_nombre_etiqueta etiqueta) {
//
//	reservarContextoSinRetorno();
//
//	int posicionAPushear =  top_index +1;
//	//*pcb->c_stack = posicionAPushear;
//	cursor = posicionAPushear;
//
//	irAlLabel(etiqueta);
//
//	pcb->tamanio_contexto = 0;
//
//	esConRetorno = 0;

}


void llamarConRetorno(t_nombre_etiqueta etiqueta, t_puntero donde_retornar) {
//
//	reservarContextoConRetorno(donde_retornar);
//
//	int posicionAPushear = top_index +1;
//	//*pcb->c_stack = posicionAPushear;
//	cursor = posicionAPushear;
//
//	irAlLabel(etiqueta);
//
//	pcb->tamanio_contexto = 0;
}


void finalizar() {
//	t_puntero c_stack_viejo;
//
//	if(!esPrimerContexto()) {
//
//	recuperarPosicionDeDirecciones();
//	volverAContextoAnterior(&c_stack_viejo);
//
//	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);
//
//	//*pcb->c_stack = c_stack_viejo;
//	cursor = c_stack_viejo;
//
//	regenerarDiccionario(tamanio);
//	}
//
//	if(esPrimerContexto()) {
//		termino = DONE;
//
//		log_escribir(archLog, "Ejecucion", INFO, "Finalizo ejecucion");
//
//	}
}


void retornar(t_valor_variable retorno) {

//	recuperarPosicionDeDirecciones();
//
//	t_puntero direccionRetorno;
//	recuperarDireccionRetorno(&direccionRetorno);
//
//	t_puntero c_stack_viejo;
//	volverAContextoAnterior(&c_stack_viejo);
//
//	int tamanio = calcularTamanioContextoAnterior(c_stack_viejo);
//
//	//*pcb->c_stack = c_stack_viejo;
//	cursor = c_stack_viejo;
//
//	regenerarDiccionario(tamanio);
//
//	t_puntero posicionAsignacion = direccionRetorno + 1;
//
//	t_struct_push* estructura = malloc(sizeof(t_struct_push));
//	estructura->posicion=posicionAsignacion;
//	estructura->valor = retorno;
//	estructura->stack_base = pcb->stack;
//	socket_enviar(sockUMV, D_STRUCT_PUSH, estructura);
//	free(estructura);
//
//	chequearSiHuboSF();
//
//	esConRetorno = 0;
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se retorna el valor %d",retorno);

}

void imprimir(t_valor_variable valor_mostrar) {
	//Envía valor_mostrar al Kernnel, para que termine siendo mostrado en la consola del Programa en ejecución.

//	//DESPUES SE HACE FREE DE ESTO?
//	char* valor_variable = string_itoa(valor_mostrar);
//
//	t_struct_nombreMensaje* estructura = malloc(sizeof(t_struct_nombreMensaje));
//	estructura->mensaje = valor_variable;
//	estructura->pid = pcb->pid;
//	socket_enviar(sockKernel,D_STRUCT_NOMBREMENSAJE,estructura);
//	free(estructura);
//
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %d para imprimirlo por pantalla",valor_mostrar);

}

void imprimirTexto(char* texto) {
	//Envía mensaje al Kernel, para que termine siendo mostrado en la consola del Programa en ejecución. mensaje no posee parámetros, secuencias de escape, variables ni nada.
//
//	t_struct_nombreMensaje* estructura = malloc(sizeof(t_struct_nombreMensaje));
//	estructura->mensaje = texto;
//	estructura->pid = pcb->pid;
//	socket_enviar(sockKernel,D_STRUCT_NOMBREMENSAJE,estructura);
//	free(estructura);
//
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se envia a kernel %s para imprimirlo por pantalla",texto);

}

void entradaSalida(t_nombre_dispositivo dispositivo, int tiempo) {
//
//	termino = IO;
//
//	t_struct_pcb_io* pcb_actualizada=malloc(sizeof(t_struct_pcb_io));
//
//	pcb_actualizada->c_stack=pcb->c_stack;
//	pcb_actualizada->codigo=pcb->codigo;
//	pcb_actualizada->index_codigo=pcb->index_codigo;
//	pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
//	pcb_actualizada->pid=pcb->pid;
//	pcb_actualizada->program_counter=pcb->program_counter;
//	pcb_actualizada->stack=pcb->stack;
//	pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
//	pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
//	pcb_actualizada->tiempo = tiempo;
//	pcb_actualizada->dispositivo = dispositivo;
//
//	socket_enviar(sockKernel,D_STRUCT_PCBIO,pcb_actualizada);
//
//	free(pcb_actualizada);
//	free(pcb);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se conecto %s por %d tiempo",dispositivo,tiempo);

}

/****************************** OPERACIONES DE KERNEL ************************************************/

void wait_ansisop(t_nombre_semaforo identificador_semaforo) {

//	t_struct_semaforo* estructura = malloc(sizeof(t_struct_semaforo));
//	estructura->nombre_semaforo = identificador_semaforo;
//	socket_enviar(sockKernel, D_STRUCT_WAIT, estructura);
//	free(estructura);
//
//	controlarBloqueo(sockKernel, termino);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (wait)",identificador_semaforo);

}

void signal_ansisop(t_nombre_semaforo identificador_semaforo) {
//
//	t_struct_semaforo* estructura = malloc(sizeof(t_struct_semaforo));
//	estructura->nombre_semaforo = identificador_semaforo;
//	socket_enviar(sockKernel, D_STRUCT_SIGNALSEMAFORO, estructura);
//	free(estructura);
//
//	controlarBloqueo(sockKernel, termino);
//
//	log_escribir(archLog, "Ejecucion", INFO, "Se solicito semaforo %s (signal)",identificador_semaforo);
}

