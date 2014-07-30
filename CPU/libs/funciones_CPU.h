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
#include <semaphore.h>

typedef struct{
	char* ip_kernel;
	int puerto_kernel;
	char* ip_umv;
	int puerto_umv;
	int retardo
}config_cpu;

/************** VARIABLES GLOBALES **************/
extern t_config* config;
extern config_cpu configuracion_CPU;
extern char* PATH;
extern int termino;
//extern t_struct_pcb* pcb;
extern t_retardo_quantum retardo;
extern int sockKernel;
extern int sockUMV;
extern char* indiceEtiquetas;
extern sem_t* sem_kernel;

/*************  PROTOTIPO FUNCIONES  *************/

void inicializarConfiguracion(void);
void leerConfiguracion();
void imprimirConfiguracion(void);
void log_error_socket(void);
void core_conexion_kernel(void);
void recupero_diccionario(t_dictionary* diccionario, int tamanio_contexto);
void rutina(int n);
void core_conexion_umv(void);
void excepcion_UMV(int i);





#endif /* FUNCIONES_CPU_H_ */
