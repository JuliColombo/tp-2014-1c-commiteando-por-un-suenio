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
			.AnSISOP_finalizar				= finalizar,
			.AnSISOP_irAlLabel				= irAlLabel,
			.AnSISOP_llamarSinRetorno		= llamarSinRetorno,
			.AnSISOP_llamarConRetorno		= llamarConRetorno,
			.AnSISOP_retornar				= retornar,
			.AnSISOP_imprimir				= imprimir,

};
AnSISOP_kernel funciones_kernel = { };

void proximaInst() {
	//Le mando intruccion a UMV para que busque en su indice de codigo y me devuelva la instruccion a parsear

	/*t_intructions inst = indiceCodigo[pcb.program_counter];

	buscarEnSegmentoCodigo(inst);*/



}

void parsear(){

	proximaInst();

	pcb.program_counter += 1;

	analizadorLinea(strdup(proximaInstruccion),&funciones_parser, &funciones_kernel);

}

void esperar_retardo(int tiempo){
	usleep(tiempo*1000);
}

void hot_plug(int signum) {
	if(signum == SIGUSR1){
		if (i==quantum){
		termino = 5;
		printf("******************* ACA DEBERIA CERRAR EL SOCKET DE LA CPU *******************************\n");
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

		//parsear();

		esperar_retardo(retardo);

		if (termino != 0) {
			salir(termino);
			termino = 0;
			break;
		}
	}
}
