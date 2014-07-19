/*
 * parser.c
 *
 *  Created on: 12/07/2014
 *      Author: utnso
 */
#include "parserCPU.h"
char* variables = "";

AnSISOP_funciones funciones_parser = {
			.AnSISOP_definirVariable		= definirVariable,
			.AnSISOP_obtenerPosicionVariable= obtenerPosicionVariable,
			.AnSISOP_dereferenciar			= dereferenciar,
			.AnSISOP_asignar				= asignar,
			.AnSISOP_asignarValorCompartida = asignarValorCompartida,
			.AnSISOP_obtenerValorCompartida = obtenerValorCompartida,
			.AnSISOP_irAlLabel				= irAlLabel,
			.AnSISOP_llamarSinRetorno		= llamarSinRetorno,
			.AnSISOP_llamarConRetorno		= llamarConRetorno,
			.AnSISOP_retornar				= retornar,
			.AnSISOP_finalizar				= finalizar,
			.AnSISOP_imprimir				= imprimir,
			.AnSISOP_imprimirTexto			= imprimirTexto,
			.AnSISOP_entradaSalida			= entradaSalida,

};
AnSISOP_kernel funciones_kernel = {
			.AnSISOP_signal					= signal_ansisop,
			.AnSISOP_wait					= wait_ansisop,
};

void darValoresDeStackYCursor(t_pcb* pcb){
	if(pcb->c_stack == pcb->stack){
		cursor = 0;
		stack = 0;								//CON O SIN ASTERISCOS?
	} else {
		stack = 0;
		cursor = pcb->c_stack - pcb->stack;
	}
}

void proximaInst() {
	//Le mando intruccion a UMV para que busque en su indice de codigo y me devuelva la instruccion a parsear

	t_intructions inst = instruccionParaBuscarEnIndiceCodigo(pcb->program_counter);

	t_struct_instruccion* estructura = malloc(sizeof(t_struct_instruccion));
	estructura->inst = inst;
	socket_enviar(sockUMV, D_STRUCT_INSTRUCCION, estructura);
	free(estructura);

	recibirProximaInstruccion(sockUMV);

}

void parsear(){

	proximaInst();

	pcb->program_counter += 1;

	log_escribir(archLog, "Ejecucion", INFO, "Se ejecuta nueva instruccion");

	analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);

}

void esperar_retardo(int tiempo){
	usleep(tiempo/1000);
	log_escribir(archLog, "Ejecucion", INFO, "Retardo de %d ms",tiempo);
}

void continuarHastaQuantum() {
	for (i; i <= quantum; i++) {
		termino = CONTINUES;
		parsear();
		esperar_retardo(retardo);
	}
}

void hot_plug(int signum) {
	if(signum == SIGUSR1){
		continuarHastaQuantum();
		termino = DONE;
		i = 0;
		if(socket_cerrarConexion(sockKernel)==-1){
			log_escribir(archLog,"Conexion",ERROR,"No se pudo cerrar conexion del Kernel");
		if(socket_cerrarConexion(sockKernel)==-1){
			log_escribir(archLog,"Conexion",ERROR,"No se pudo cerrar conexion de UMV");
			}
		}
	}
}

void destruirEstructuras(){
	dictionary_destroy_and_destroy_elements(diccionario,(void*)elemento_delete);
}

void closureMostrarEstado(char* key, t_elemento* elem) {
	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion = elem->pos+1;
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

	char* str = string_itoa(valor_variable);
	string_append(&variables,str);

}

void mostrarEstadoVariables(){
	dictionary_iterator(diccionario,(void*)closureMostrarEstado);
}

void salirPorFinalizacion(){
	mostrarEstadoVariables();
	destruirEstructuras();
	log_escribir(archLog,"Ejecucion",INFO,"Termino de ejecutarse el programa");

}

void salir(int termino) {
	t_struct_pcb_fin* pcbF;
	t_struct_pcb* pcbQ;
	t_struct_pcb_fin* pcbSF;

	switch (termino) {
	case DONE:
	pcbF =malloc(sizeof(t_struct_pcb_fin));
	salirPorFinalizacion();

	pcbF->c_stack=pcb->c_stack;
	pcbF->codigo=pcb->codigo;
	pcbF->index_codigo=pcb->index_codigo;
	pcbF->index_etiquetas=pcb->index_etiquetas;
	pcbF->pid=pcb->pid;
	pcbF->program_counter=pcb->program_counter;
	pcbF->stack=pcb->stack;
	pcbF->tamanio_contexto=pcb->tamanio_contexto;
	pcbF->tamanio_indice=pcb->tamanio_indice;
	pcbF->variables = variables;
	socket_enviar(sockKernel,D_STRUCT_PCBFIN,pcbF);

	free(pcbF);

	printf("\ntermino ejecucion \n");
	break;

	case QUANTUM:

	log_escribir(archLog,"Ejecucion",INFO,"Sale por quantum");

	pcbQ = malloc(sizeof(t_struct_pcb));
	pcbQ->c_stack=pcb->c_stack;
	pcbQ->codigo=pcb->codigo;
	pcbQ->index_codigo=pcb->index_codigo;
	pcbQ->index_etiquetas=pcb->index_etiquetas;
	pcbQ->pid=pcb->pid;
	pcbQ->program_counter=pcb->program_counter;
	pcbQ->stack=pcb->stack;
	pcbQ->tamanio_contexto=pcb->tamanio_contexto;
	pcbQ->tamanio_indice=pcb->tamanio_indice;

	socket_enviar(sockKernel,D_STRUCT_PCB,pcbQ);

	free(pcbQ);

	printf("\nsalgo por quantum\n");
	break;

	case SEG_FAULT:

	log_escribir(archLog,"Ejecucion",ERROR,"Segmentation Fault. No se puede seguir con la ejecucion");

	pcbSF =malloc(sizeof(t_struct_pcb));

	pcbSF->c_stack=pcb->c_stack;
	pcbSF->codigo=pcb->codigo;
	pcbSF->index_codigo=pcb->index_codigo;
	pcbSF->index_etiquetas=pcb->index_etiquetas;
	pcbSF->pid=pcb->pid;
	pcbSF->program_counter=pcb->program_counter;
	pcbSF->stack=pcb->stack;
	pcbSF->tamanio_contexto=pcb->tamanio_contexto;
	pcbSF->tamanio_indice=pcb->tamanio_indice;
	socket_enviar(sockKernel,D_STRUCT_PCBSF,pcbSF);

	free(pcbSF);

	printf("\nsalgo por segmentation fault\n");
	break;

	case IO:
		break;
}
}

void correrParser() {
	termino = CONTINUES;

	signal(SIGUSR1,hot_plug);

	for(i=0;i<=quantum;i++){

		parsear();

		esperar_retardo(retardo);

		if((i == quantum) && (termino == CONTINUES)){
			termino = QUANTUM;
			pcb->c_stack += cursor;
			pcb->stack += stack;
		}

		if (termino != CONTINUES) {
			salir(termino);
			termino = CONTINUES;
			i = 0;
			break;
		}

	}
}
