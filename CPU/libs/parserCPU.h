/*
 * parser.h
 *
 *  Created on: 12/07/2014
 *      Author: utnso
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "primitivas.h"
#include "primitivasAux.h"
#include "Sockets/socket.h"
#include <signal.h>

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
