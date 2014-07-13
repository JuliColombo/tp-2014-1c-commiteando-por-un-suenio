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

t_quantum quantum;
t_retardo_quantum retardo;

typedef struct{
	char* ip_kernel;
	int puerto_kernel;
	char* ip_umv;
	int puerto_umv;
}config_cpu;

/************** VARIABLES GLOBALES **************/
extern config_cpu configuracion_CPU;
extern char* PATH;
int termino;

//VA ACA EL QUANTUM Y SU RETARDO?

/*************  PROTOTIPO FUNCIONES  *************/

void inicializarConfiguracion(void);
void leerConfiguracion();
void imprimirConfiguracion(void);
void log_error_socket(void);
void* core_conexion_kernel(void);
void* core_conexion_umv(void);
void recibir_quantum(int sockKernel);
void recibir_retardo_quantum(int sockKernel);


#endif /* FUNCIONES_CPU_H_ */
