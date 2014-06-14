/*
 * umv.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "libs/funciones_UMV.h"

int* MP;
int tamanioMP;
char* PATH=PATHCONFIG;
int retardo;
tablaSeg* tablaDeSegmentos;
t_config_UMV configuracion_UMV;
pthread_t CONSOLA, KERNEL, CPU;
log_t* archLog;
pthread_mutex_t* mutex;
pthread_mutex_t* mutex_log;
int sock_kernel;
int sock_cpu;
int procesoEnUso;

int main (int argc, char **argv){

	inicializarConfiguracion();
	//Acceder a archConfig y obtener datos
	MP=crearMP();

	//Crear estructuras administrativas

	inicializarSemaforos();
	inicializarHilos();

	esperarHilos();

	return EXIT_SUCCESS;
}




