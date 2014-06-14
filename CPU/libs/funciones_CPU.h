/*
 * funciones_CPU.h
 *
 *  Created on: 13/06/2014
 *      Author: utnso
 */

#ifndef FUNCIONES_CPU_H_
#define FUNCIONES_CPU_H_
#define PATHCONFIG "/home/utnso/git/tp-2014-1c-commiteando-por-un-suenio/CPU/configuracion_CPU.cfg"

#include <commons/collections/dictionary.h>
#include "parser/metadata_program.h"
#include <stdio.h>
#include "primitivas.h"
#include "primitivasAux.h"
#include "commons/config.h"
#include "log.h"
#include <pthread.h>
#include "Sockets/socket.h"

typedef struct{
	char* ip_kernel;
	int puerto_kernel;
	char* ip_umv;
	int puerto_umv;
}config_cpu;

/************** VARIABLES GLOBALES **************/
extern config_cpu configuracion_CPU;
extern char* PATH;


/*************  PROTOTIPO FUNCIONES  *************/

void inicializarConfiguracion(void);
void leerConfiguracion();
void imprimirConfiguracion(void);
void log_error_socket(void);
void* core_conexion_kernel(void);
void* core_conexion_umv(void);


#endif /* FUNCIONES_CPU_H_ */
