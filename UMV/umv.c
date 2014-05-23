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

int main (int argc, char **argv){
	PATH = argv[1];
	inicializarConfiguracion(PATH);
	//Acceder a archConfig y obtener datos
	MP=crearMP();
	//Crear estructuras administrativas

	pthread_create(&CONSOLA, NULL, core_consola, NULL);
//	pthread_create(&KERNEL, NULL, (void*) &core_kernel, NULL);
/*	pthread_create(&CPU, NULL, (void*) &core_cpu, NULL);

	pthread_join(KERNEL,NULL);
	pthread_join(CPU,NULL);
*/
	pthread_join(CONSOLA,NULL);
	free(MP);
	return 0;
}




