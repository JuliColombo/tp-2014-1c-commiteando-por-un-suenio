/*
 * parser.c
 *
 *  Created on: 12/07/2014
 *      Author: utnso
 */
#include "parserCPU.h"

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
		stack = 0;
	} else {
		stack = 0;
		cursor = pcb->c_stack - pcb->stack;
	}
}

void proximaInst() {
	//Le mando intruccion a UMV para que busque en su indice de codigo y me devuelva la instruccion a parsear

	t_intructions inst = instruccionParaBuscarEnIndiceCodigo(pcb->program_counter);

	socket_and_instruccion(sockUMV,inst);

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
	socket_and_pop_position(sockUMV,elem->pos + 1);

	t_valor_variable valor_variable;

	t_tipoEstructura tipoRecibido;
		void* structRecibida;
		int j=socket_recibir(sockUMV,&tipoRecibido,&structRecibida);
		if(j==1){
			t_struct_numero* k = ((t_struct_numero*)structRecibida);
			valor_variable= k->numero;
			free(k);
		}

	//imprimir(valor_variable);
	char* str = convertirAStringNumero(valor_variable);
	strcat(variables,str);

}

void mostrarEstadoVariables(){
	dictionary_iterator(diccionario,(void*)closureMostrarEstado);
}

void salirPorFinalizacion(){
	mostrarEstadoVariables();
	destruirEstructuras();

}

void salir(int termino) {
	t_struct_pcb* pcb_actualizada=malloc(sizeof(t_struct_pcb));

	switch (termino) {
	case DONE:
	salirPorFinalizacion();

	pcb_actualizada->c_stack=pcb->c_stack;
	pcb_actualizada->codigo=pcb->codigo;
	pcb_actualizada->index_codigo=pcb->index_codigo;
	pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
	pcb_actualizada->pid=pcb->pid;
	pcb_actualizada->program_counter=pcb->program_counter;
	pcb_actualizada->stack=pcb->stack;
	pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
	pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
	socket_enviar(sockKernel,D_STRUCT_PCB,pcb_actualizada);

	free(pcb_actualizada);

	printf("\ntermino ejecucion \n");
	break;

	case QUANTUM:

	pcb_actualizada->c_stack=pcb->c_stack;
	pcb_actualizada->codigo=pcb->codigo;
	pcb_actualizada->index_codigo=pcb->index_codigo;
	pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
	pcb_actualizada->pid=pcb->pid;
	pcb_actualizada->program_counter=pcb->program_counter;
	pcb_actualizada->stack=pcb->stack;
	pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
	pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
	socket_enviar(sockKernel,D_STRUCT_PCB,pcb_actualizada);

	free(pcb_actualizada);

	printf("\nsalgo por quantum\n");
	break;
	}
}

void correrParser() {
	signal(SIGUSR1,hot_plug);

	for(i=0;i<=quantum;i++){
		termino = CONTINUES;

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
