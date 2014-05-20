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

int main (int argc, char **argv){
	leerConfiguracion();
	imprimirConfiguracion();
	//Acceder a archConfig y obtener datos
	MP=crearMP();
	//Crear estructuras administrativas

	int thread_consola = pthread_create(&CONSOLA, NULL, (void*) &core_consola, NULL);
//	pthread_create(&KERNEL, NULL, (void*) &core_kernel, NULL);
/*	pthread_create(&CPU, NULL, (void*) &core_cpu, NULL);

	pthread_join(KERNEL,NULL);
	pthread_join(CPU,NULL);
*/
	pthread_join(thread_consola,NULL);
	return 0;
}




