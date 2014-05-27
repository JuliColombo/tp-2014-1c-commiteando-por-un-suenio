/*
 * cpu.h
 *
 *  Created on: 24/05/2014
 *      Author: utnso
 */

#ifndef CPU_H_
#define CPU_H_

#include <commons/collections/dictionary.h>
#include "parser/metadata_program.h"
#include "libs/stack.h"
#include <stdio.h>
#include "libs/primitivas.h"
#include "libs/primitivasAux.h"
#include "commons/config.h"
#include "libs/log.h"
#include "pthread.h"
#define PATHLOG "/home/utnso/tp-2014-1c-commiteando-por-un-suenio"

typedef struct{
	int ip_kernel;
	int puerto_kernel;
	int ip_umv;
	int puerto_umv;
}config_cpu;

/************** VARIABLES GLOBALES **************/
extern config_cpu configuracion_CPU;
extern char* PATH;


/*************  PROTOTIPO FUNCIONES  *************/
void inicializarConfiguracion(void);
void leerConfiguracion();
void log_error_socket(void);
void* core_conexion_plp_programas(void);
#endif /* CPU_H_ */
