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
		stack = 0;								//CON ASTERISCOS
	} else {
		stack = 0;
		cursor = pcb->c_stack - pcb->stack;
	}
}

void proximaInst() {
	//Le mando intruccion a UMV para que busque en su indice de codigo y me devuelva la instruccion a parsear
	int j;
	t_intructions inst = instruccionParaBuscarEnIndiceCodigo(pcb->program_counter);

	t_struct_seg_codigo* estructura = malloc(sizeof(t_struct_seg_codigo));
	estructura->inst = inst;
	estructura->seg_codigo = pcb->codigo;
	if((j=socket_enviar(sockUMV, D_STRUCT_SEGCODIGO, estructura))==1){
		printf("se mando la instruccion \n");
		free(estructura);}

	recibirProximaInstruccion(sockUMV);

}

void parsear(){

	printf("parseo piola\n");

	proximaInst();

	pcb->program_counter += 1;

	log_escribir(archLog, "Ejecucion", INFO, "Se ejecuta nueva instruccion");

	analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);

}

void esperar_retardo(int tiempo){
	usleep(tiempo/1000);
	log_escribir(archLog, "Ejecucion", INFO, "Retardo de %d ms",tiempo);
}

void seg_fault(int signum){
	if(signum == SIGUSR2){
		printf("ME LLEGO SIGUSR2 Y TERMINO VALE %d!!!!!!!!!!\n",termino);
		salir(termino);
		termino = CONTINUES;
	}
}

void continuarHastaQuantum() {
	for (i; i <= quantum; i++) {
		termino = CONTINUES;
		parsear(pcb);
		esperar_retardo(retardo);
	}
}

void hot_plug(int signum) {
	if(signum == SIGUSR1){
		continuarHastaQuantum();
		i = 0;

		t_struct_pcb* pcbQ;
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
		free(pcb);


		if(socket_cerrarConexion(sockKernel)==-1){
			log_escribir(archLog,"Conexion",ERROR,"No se pudo cerrar conexion del Kernel");
		if(socket_cerrarConexion(sockKernel)==-1){
			log_escribir(archLog,"Conexion",ERROR,"No se pudo cerrar conexion de UMV");
			}
		}
	}
}

void destruirEstructuras(){
	dictionary_destroy_and_destroy_elements(diccionario,(void*)elemento_delete); //NO DEBERIA PONER ELIMINAR DICCIONARIO EN CPU.C ENTONCES NO?
	free(variables);
}

void closureMostrarEstado(char* key, t_elemento* elem) {
	t_struct_pop* estructura = malloc(sizeof(t_struct_pop));
	estructura->posicion = elem->pos+1;
	estructura->stack_base = pcb->stack;
	estructura->tamanio = sizeof(t_valor_variable);
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

	char* var = strdup(variables);
	char* barraN = "\n";
	char* str = string_itoa(valor_variable);
	string_append(&str,barraN);
	string_append(&var,str);
	variables = strdup(var);
	free(var);

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

	//*pcb->c_stack += cursor;
	//*pcb->stack += stack;

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
	free(pcb);
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
	free(pcb);
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
	free(pcb);
	break;

	case IO:
		break;
	case BLOQUEADO:
		log_escribir(archLog,"Ejecucion",INFO,"Se bloquea");

		t_struct_pcb* pcb_actualizada;
		int tam = sizeof(t_struct_pcb);
		pcb_actualizada = malloc(tam);
		pcb_actualizada->pid=pcb->pid;
		pcb_actualizada->c_stack=pcb->c_stack;
		pcb_actualizada->codigo=pcb->codigo;
		pcb_actualizada->index_codigo=pcb->index_codigo;
		pcb_actualizada->index_etiquetas=pcb->index_etiquetas;
		pcb_actualizada->program_counter=pcb->program_counter;
		pcb_actualizada->stack=pcb->stack;
		pcb_actualizada->tamanio_contexto=pcb->tamanio_contexto;
		pcb_actualizada->tamanio_indice=pcb->tamanio_indice;
		int i = socket_enviar(sockKernel,D_STRUCT_PCBSEM,pcb_actualizada);
		if(i==1){
			free(pcb_actualizada);
			free(pcb);
			}
		break;
}
}

void correrParser(t_pcb* pcb_llego) {
	dictionary_create(diccionario);
	termino = CONTINUES;
	signal(SIGUSR2,seg_fault);
	signal(SIGUSR1,hot_plug);
	pcb = malloc(sizeof(t_pcb));
	pcb = pcb_llego;

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
	free(pcb_llego);
}
