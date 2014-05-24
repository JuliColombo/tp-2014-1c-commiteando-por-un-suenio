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
#include "commons/config.h"


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
void leerConfiguracion(char*);
#endif /* CPU_H_ */
