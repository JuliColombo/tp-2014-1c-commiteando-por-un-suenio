/*
 * parserCPU.h
 *
 *  Created on: 12/07/2014
 *      Author: utnso
 */

#ifndef PARSERCPU_H_
#define PARSERCPU_H_

#include "primitivas.h"
#include "primitivasAux.h"
#include "log.h"
#include <signal.h>
#include "estructuras.h"

typedef unsigned int t_quantum;
typedef long int t_retardo_quantum;

extern t_quantum quantum;
extern t_retardo_quantum retardo;
extern t_dictionary* diccionario;


int i;
char* proximaInstruccion;
int termino;
int sockUMV;
int sockKernel;

void proximaInst();
void parsear();
void esperar_retardo(int tiempo);
void hot_plug(int signum);
void salir(int termino);
void correrParser(t_pcb* pcb);
void continuarHastaQuantum();
void destruirEstructuras();
void mostrarEstadoVariables();
void salirPorFinalizacion();
void darValoresDeStackYCursor(t_pcb* pcb);

#endif /* PARSERCPU_H_ */
