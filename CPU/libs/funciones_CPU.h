/*
 * funciones_CPU.h
 *
 *  Created on: 13/06/2014
 *      Author: utnso
 */

#ifndef FUNCIONES_CPU_H_
#define FUNCIONES_CPU_H_

#include "commons/config.h"
#include <pthread.h>
#include "parserCPU.h"
#include "estructuras.h"

typedef struct{
	char* ip_kernel;
	int puerto_kernel;
	char* ip_umv;
	int puerto_umv;
}config_cpu;

/************** VARIABLES GLOBALES **************/
extern config_cpu configuracion_CPU;
extern char* PATH;
extern int termino;
//extern t_struct_pcb* pcb;
extern t_quantum quantum;
extern t_retardo_quantum retardo;
extern int sockKernel;
extern int sockUMV;

/*************  PROTOTIPO FUNCIONES  *************/

void inicializarConfiguracion(void);
void leerConfiguracion();
void imprimirConfiguracion(void);
void log_error_socket(void);
void core_conexion_kernel(void);
void core_conexion_umv(void);



#endif /* FUNCIONES_CPU_H_ */
