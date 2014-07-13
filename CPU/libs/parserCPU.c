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

	analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);

}

void esperar_retardo(int tiempo){
	usleep(tiempo/1000);
}

void hot_plug(int signum) {
	if(signum == SIGUSR1){
		for(i = 0;i<=quantum;i++){ //i no deberia ser igual a 0
				termino = 0;
				parsear();
				esperar_retardo(retardo);

		termino = 5;
		i = 0;
		if(socket_cerrarConexion(sockKernel)==-1){
			log_escribir(archLog,"Conexion",ERROR,"No se pudo conectar al Kernel");}
		}
	}
}

void salir(int termino) {
	switch (termino) {
	case 5: //finalizo
	printf("\ntermino ejecucion \n");
	break;
	case 3: //sale por quantum
	printf("\nsalgo por quantum\n");
	break;
	}
}

void correrParser() {
	signal(SIGUSR1,hot_plug);

	for(i=0;i<=quantum;i++){
		termino = 0;

		parsear();

		esperar_retardo(retardo);

		if (termino != 0) {
			salir(termino);
			termino = 0;
			i = 0;
			break;
		}
	}
}
