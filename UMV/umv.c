/*
 * umv.c
 *
 *  Created on: 30/04/2014
 *      Author: utnso
 */

#include "libs/funciones_UMV.h"


int* MP;
int algor=firstfit;
char* PATH;
t_config_UMV configuracion_UMV;
pthread_t CONSOLA, KERNEL, CPU;
log_t* archLog;
pthread_mutex_t* mutex;

int main (int argc, char **argv){
	PATH = argv[1];
	inicializarConfiguracion(PATH);
	//Acceder a archConfig y obtener datos
	MP=crearMP();
	//Crear estructuras administrativas

	inicializarHilos();
	esperarHilos();

	free(MP);
	return 0;
}




