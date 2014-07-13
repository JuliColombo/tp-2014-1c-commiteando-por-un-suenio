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

typedef uint8_t t_quantum;
typedef long t_retardo_quantum;

extern t_quantum quantum;
extern t_retardo_quantum retardo;

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
void correrParser();

#endif /* PARSER_H_ */
