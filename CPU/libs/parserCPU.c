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

void proximaInst() {
	//Le mando intruccion a UMV para que busque en su indice de codigo y me devuelva la instruccion a parsear

	t_intructions inst = instruccionParaBuscarEnIndiceCodigo(pcb.program_counter);

	socket_and_instruccion(sockUMV,inst);

	recibirProximaInstruccion(sockUMV);

}

void parsear(){

	proximaInst();

	pcb.program_counter += 1;

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
			log_escribir(archLog,"Conexion",ERROR,"No se pudo cerrar conexion");
		}
	}
}

void destruirEstructuras(){
	//SOCKET A UMV PARA QUE DESTRUYA STACK Y/O INDICE DE CODIGO Y/O INDICE DE ETIQUETAS
	dictionary_destroy_and_destroy_elements(diccionario,(void*)elemento_delete);
}

void closureMostrarEstado(char* key, int posicion) {
	socket_and_pop_position(sockUMV,posicion + 1);

	t_struct_numero* estructura =(t_struct_numero*)socket_recibir_estructura(sockUMV);
	t_valor_variable valor_variable = estructura->numero;

	imprimir(valor_variable);
}

void mostrarEstadoVariables(){
	dictionary_iterator(diccionario,(void*)closureMostrarEstado);
}

void salirPorFinalizacion(){
	mostrarEstadoVariables();
	destruirEstructuras();
	socket_and_pcb(sockKernel,pcb);//enviar por socket pcb a kernel para notificar que termine
}

void salir(int termino) {
	switch (termino) {

	case DONE:
	salirPorFinalizacion();
	printf("\ntermino ejecucion \n");
	break;

	case QUANTUM:
	socket_and_pcb(sockKernel,pcb);//enviar por socket pcb a kernel para notificar que termino quantum y me tiene que mandar otro
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
		}

		if (termino != CONTINUES) {
			salir(termino);
			termino = CONTINUES;
			i = 0;
			break;
		}
	}
}
